  int totale = 0;   //somma delle diverse puntate dei giocatori
  int meta = 0;   //valore della meta
  bool appoggio = false;  //appoggio per i vari loop del programma
  int puntata = 0;  //valore della puntata
  int turno = 0;    //turno


void inserisciMeta()
{
  Serial.print("Inserire la meta compresa tra 30 e 100");
  while(appoggio == false)
  {
    if(Serial.available() > 0)
        {
          if(Serial.readString().toInt() > 30 && Serial.readString().toInt() < 100)
          {
            meta = Serial.readString().toInt();
            Serial.println(meta);
            appoggio = true;
          }
          else
          {
            Serial.println("Meta non compresa tra 30 e 100!");
          }
        }
  }
  appoggio = false;
}

void attendiPuntata()
{
  Serial.println("Inserire la puntata");
  while(appoggio == false)
  {
    if(Serial.available() > 0 && turno == 0)
    {
      int puntataScelta = Serial.readString().toInt();
      if(puntataScelta > -1 && puntataScelta < 7)         //puntata compresa tra 0 e 6 se è il primo turno
      {
        puntata = puntataScelta;
        totale = totale + puntata;
        appoggio = true;
        turno = turno + 1;
      }
      else
      {
        Serial.println("La puntata deve essere compresa tra 0 e 6");
      }
    }
    else if(Serial.available() > 0)
    {
      int puntataScelta = Serial.readString().toInt();
      if(puntataScelta > 0 && puntataScelta < 7 && puntataScelta != puntata && puntataScelta != 7-puntata)
      {
        puntata = puntataScelta;
      }
    }
  }
  appoggio = false;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
      inserisciMeta();
    }
