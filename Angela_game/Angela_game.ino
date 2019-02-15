#include <LiquidCrystal.h>
  
  int totale;   //somma delle diverse puntate dei giocatori
  int meta;   //valore della meta
  bool appoggio;  //appoggio per i vari loop del programma
  int puntata;  //valore della puntata
  int turno;   //turno
  LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
   


void setup() {
  // put your setup code here, to run once:
  totale = 0;
  meta = 0;
  appoggio = false;
  puntata = 0;
  turno = 0;
  lcd.begin(16, 2);
  lcd.clear();
}

void inserisciMeta()
{
  metaInfo();
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
            indexError();
          }
        }
  }
  appoggio = false;
}


void checkFirstTurn()         //accetta la puntata in input del primo turno
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

void checkTurn()        //acceta la puntata in input di tutti i turni eccetto il primo
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

void attendiPuntata()
{
  Serial.print("\r\n");
  Serial.print("Inserire la puntata");
  while(appoggio == false)
  {
    if(Serial.available() > 0 && turno == 0)
    {
      checkFirstTurn();
    }
    if (Serial.available() > 0 && turno != 0)
    {
        checkTurn(); 
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
//----------------------------------------------------------------------------------------------------
//METODI PER L'OUTPUT SERIALE LCD
void indexError()
{
  lcd.clear();
    lcd.print("Non è compreso");
    lcd.setCursor(0,1);
    lcd.print("tra 30 e 100!");
    delay(1500);
    lcd.setCursor(0,0);
    lcd.print("Reinserire la");
    lcd.setCursor(0,1);
    lcd.print("meta");
}

void metaInfo(){
  lcd.clear();
  lcd.print("Inserire la meta");
  lcd.setCursor(0,1);
  lcd.print("compresa tra 30 e 100");
  delay(1000);
  for(int i = 0; i < 5; i++)
  {
  lcd.scrollDisplayLeft();
  delay(400);
  }
}
//----------------------------------------------------------------------------------------------------
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
