#include <LiquidCrystal.h>

  const int buttonForward = 9;
  const int buttonBackward = 8;
  int totale;   //somma delle diverse puntate dei giocatori
  int meta;   //valore della meta
  bool appoggio;  //appoggio per i vari loop del programma
  int puntata;  //valore della puntata
  int turno;   //turno
  LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
   


void setup() {
  // put your setup code here, to run once:
  totale = 0;
  meta = 30;
  appoggio = false;
  puntata = 0;
  turno = 0;
  pinMode(buttonForward, INPUT);
  pinMode(buttonBackward, INPUT);
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

void getMetaValue()
{
  bool finito = false;
  while(!finito)
  {
    if(digitalRead(buttonForward) == HIGH)  //bottone "avanti" premuto
    {
      meta = meta + 1;
      finito = true;    //PROVA! VA TOLTO!
    }
    if(digitalRead(buttonBackward == HIGH))  //bottone "indietro" premuto
    {
      meta = meta - 1; 
    }
    //METTO IL BOTTONE CONFERMA
  }
}

void checkFirstTurn(int puntataScelta)         //accetta la puntata in input del primo turno
{
      //int puntataScelta = Serial.readString().toInt();
      if(puntataScelta > -1 && puntataScelta < 7)         //puntata compresa tra 0 e 6 se è il primo turno
      {
        puntata = puntataScelta;
        totale = totale + puntataScelta;
        appoggio = true;
        turno = turno + 1;
      }
      else
      {
        firstWagerError();              //stampa l'errore commesso e che valori utilizzare per prima puntata
      }
}

void checkTurn(int puntataScelta)        //acceta la puntata in input di tutti i turni eccetto il primo
{
  //int puntataScelta = Serial.readString().toInt();
      if(puntataScelta > 0 && puntataScelta < 7 && puntataScelta != puntata && puntataScelta != 7-puntata)
      {
        puntata = puntataScelta;
        totale = totale + puntataScelta;
        appoggio = true;
        turno = turno + 1;
      }
      else
      {
        genericWagerError();
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
      int puntataScelta = Serial.readString().toInt();
      checkFirstTurn(puntataScelta);
    }
    if (Serial.available() > 0 && turno != 0)
    {
        int puntataScelta = Serial.readString().toInt();
        checkTurn(puntataScelta); 
    }
  }
  appoggio = false;
}

void check()      //controlla se il gioco è finito, se si ha raggiunto o superato la meta
{
  gameOverOutput((turno%2) + 1, totale == meta);
}
//----------------------------------------------------------------------------------------------------
//METODI PER L'OUTPUT LCD
void firstWagerError()
{
  Serial.print("\r\n");            
  Serial.print("La prima puntata deve essere compresa tra 0 e 6");
}

void genericWagerError()
{
  Serial.print("\r\n");
  Serial.print("La puntata deve essere compresa tra 1 e 6, diversa dalla puntata dell'utente precedente e al suo complementare a 7");
}
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
  int leftOrRight = 0;          //0 == left, 1 == right
  while(true)
  {
    for(int i = 0; i < 5; i++)
    {
      if(leftOrRight == 0)
      {
        lcd.scrollDisplayLeft();
        delay(400);
      }
      else
      {
        lcd.scrollDisplayRight();
        delay(400);
      }
    }
    if(leftOrRight == 0){ leftOrRight = 1;}
    else{leftOrRight = 0;}
    delay(1600);
  }
}

void gameOverOutput(int utente, bool controlToken)           //controlToken == false -> si ha superato il valore della meta
{                                                           //controlToken == true -> il valore del totale corrispone al valore della meta
  if(controlToken == false)
  {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Hai superato la");
      lcd.setCursor(0, 1);
      lcd.print("meta!");
      delay(2000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Vincitore: utente " + utente);
      lcd.setCursor(0, 1);
      lcd.print("Durata match: " + turno);
  }
  else
  {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Hai raggiunto la");
      lcd.setCursor(0, 1);
      lcd.print("meta!");
      delay(2000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Vincitore: utente " + utente);
      lcd.setCursor(0, 1);
      lcd.print("Durata match: " + turno);
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
