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
          int input = Serial.readString().toInt();
          if(input > 30 && input < 100)
          {
            meta = input;
            Serial.print("\r\n");
            Serial.print(meta);
            appoggio = true;
          }
          else
          {
            Serial.print("\r\n");
            Serial.print("Meta non compresa tra 30 e 100!");
            Serial.print("\r\n");
            Serial.print("Inserire la meta compresa tra 30 e 100");
          }
        }
  }
  appoggio = false;
}

void attendiPuntata()
{
  Serial.print("\r\n");
  Serial.print("Inserire la puntata");
  while(appoggio == false)
  {
    if(Serial.available() > 0 && turno == 0)
    {
      int puntataScelta = Serial.readString().toInt();
      if(puntataScelta > -1 && puntataScelta < 7)         //puntata compresa tra 0 e 6 se è il primo turno
      {
        puntata = puntataScelta;
        totale = totale + puntataScelta;
        appoggio = true;
        turno = turno + 1;
      }
      else
      {
        Serial.print("\r\n");            
        Serial.print("La prima puntata deve essere compresa tra 0 e 6");
      }
    }
    if (Serial.available() > 0 && turno != 0)
    {
      int puntataScelta = Serial.readString().toInt();
      if(puntataScelta > 0 && puntataScelta < 7 && puntataScelta != puntata && puntataScelta != 7-puntata)
      {
        puntata = puntataScelta;
        totale = totale + puntataScelta;
        appoggio = true;
        turno = turno + 1;
      }
      else
      {
        Serial.print("\r\n");
        Serial.print("La puntata deve essere compresa tra 1 e 6, diversa dalla puntata dell'utente precedente e al suo complementare a 7");
      }
    }
  }
  appoggio = false;
}

void check()      //controlla se il gioco è finito, se si ha raggiunto o superato la meta
{
  if(totale > meta)
  {
    if(turno%2 == 0) //se è un numero pari è il turno del secondo utente
    {
      String firstMessage = "Hai superato la meta, ha vinto il secondo utente in ";
      String secondMessage = firstMessage + turno;
      String message = secondMessage + " turni!";
      Serial.print("\r\n");
      Serial.print(message);
    }
    else
    {
      String firstMessage = "Hai superato la meta, ha vinto il primo utente in ";
      String secondMessage = firstMessage + turno;
      String message = secondMessage + " turni!";
      Serial.print("\r\n");
      Serial.print(message);
    }
    appoggio = true;
  }
  else if(totale == meta)
  {
    if(turno%2 == 0)
    {
        String firstMessage = "Hai raggiunto la meta! Vince il secondo utente in ";
        String secondMessage = firstMessage + turno;
        String message = secondMessage + " turni!";
        Serial.print("\r\n");
        Serial.print(message);
    }
    else
    {
        String firstMessage = "Hai raggiunto la meta! Vince il primo utente in ";
        String secondMessage = firstMessage + turno;
        String message = secondMessage + " turni!";
        Serial.print("\r\n");
        Serial.print(message);
    }
    appoggio = true;
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
      inserisciMeta();
      while(appoggio == false)
      {
      attendiPuntata();
      check();
      }
      while(appoggio == true)
      {
        //per adesso blocco il programma in questo loop, dopo con arduino verrà implementato un bottone reset per iniziare una nuova partita
      }
    }
