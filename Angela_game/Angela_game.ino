#include <LiquidCrystal.h>

  const int buttonForward = 8;
  const int buttonBackward = 9;
  const int ledUtente1 = 2;
  const int ledUtente2 = 3;
  const int Pir = 10;
  //secondo utente
  int totale;   //somma delle diverse puntate dei giocatori
  int meta;   //valore della meta
  bool appoggio;  //appoggio per i vari loop del programma
  bool pirActivated;  //gestire l'attivazione del sensore di movimento
  int puntata;  //valore della puntata
  int turno;   //turno
  LiquidCrystal lcd(12, 11, 7, 6, 5, 4);
   


void setup() {
  // put your setup code here, to run once:
  totale = 0;
  meta = 30;
  appoggio = false;
  pirActivated = false;
  puntata = 0;
  turno = 0;
  pinMode(buttonForward, INPUT);
  pinMode(buttonBackward, INPUT);
  pinMode(ledUtente1, OUTPUT);
  pinMode(ledUtente2, OUTPUT);
  pinMode(Pir, INPUT);
  lcd.begin(16, 2);
  lcd.clear();
}

void inserisciMeta()
{
  metaInfo();
  getMetaValue();
}

void getMetaValue()
{
  bool finito = false;
  bool valueChanged = false;
  while(!finito)
  {
    if(digitalRead(buttonForward) == HIGH)  //bottone "avanti" premuto
    {
      meta++;
      delay(250);
      valueChanged = true;
    }
    if(digitalRead(buttonBackward) == HIGH)  //bottone "indietro" premuto
    {
      meta--;
      delay(250);
      valueChanged = true; 
    }
    if(valueChanged)
    {
      valueChanged = false;
      updateMeta();
    }
    if(digitalRead(Pir) == HIGH)
    {
        if(!pirActivated)
        {
          confirmMessage();
          pirActivated = true;
          if(meta > 29 && meta < 100)
          {
            delay(1000);
            updateMeta();
            finito = true;
          }
          else
          {
            metaInfo();
            delay(1000);
            pirActivated = false;
          }
        }
    }
  }
}

void confirmMessage()
{
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Confermato");
}

void checkFirstTurn(int puntataScelta)         //accetta la puntata in input del primo turno e aggiorna il valore della puntata totale
{
      //int puntataScelta = Serial.readString().toInt();
      if(puntataScelta > -1 && puntataScelta < 7)         //puntata compresa tra 0 e 6 se è il primo turno
      {
        puntata = puntataScelta;
        totale = puntataScelta;
        appoggio = true;
        turno = turno + 1;
        confirmedMessage();
      }
      else
      {
        firstWagerError();              //stampa l'errore commesso e che valori utilizzare per prima puntata
      }
}

void checkTurn(int puntataScelta)        //acceta la puntata in input di tutti i turni eccetto il primo e aggiorna il valore della puntata totale
{
      if(puntataScelta > 0 && puntataScelta < 7 && puntataScelta != puntata && puntataScelta != 7-puntata)
      {
        puntata = puntataScelta;
        totale = totale + puntataScelta;
        appoggio = true;
        turno = turno + 1;
        confirmedMessage(); 
      }
      else
      {
        genericWagerError();
      }
}

void bool puntataAccettabilePrimoTurno(int valore)
{
  if(!(valore > -1 && valore <7))
  {
    return false;
  }
  else
  {
    
  }
}

void refreshLed()
{
   if(turno%2 == 0)
  {
    digitalWrite(ledUtente1, HIGH);
    digitalWrite(ledUtente2, LOW);
  }
  else
  {
    digitalWrite(ledUtente2, HIGH);
    digitalWrite(ledUtente1, LOW);
  }
}

void attendiPuntata()
{
  insertWager();
  refreshLed();
  delay(1500);
  int chosenWager = 0;
  bool wagerUpdated = false;
  pirActivated = false;
  while(!appoggio)
  {
    if(digitalRead(buttonForward) == HIGH)  //bottone "avanti" premuto
    {
      chosenWager++;
      delay(250);
      wagerUpdated = true;
    }
    if(digitalRead(buttonBackward) == HIGH)  //bottone "indietro" premuto
    {
      chosenWager--;
      delay(250);
      wagerUpdated = true; 
    }
    if(wagerUpdated)
    {
      updateWager(chosenWager);
      wagerUpdated = false;
    }
    if(digitalRead(Pir) == HIGH)                                           //PER PROVA, IMPLEMENTAREEEEEE
    {
      if(!pirActivated)
      {
        if(turno == 0)
        {
          checkFirstTurn(chosenWager);
        }
        else
        {
          checkTurn(chosenWager); 
        }
      }
    }
    
  }
}

void check()      //controlla se il gioco è finito, se si ha raggiunto o superato la meta
{
  if(totale > meta)
  {
    gameOverOutput((turno%2) + 1, false);
  }
  else if(totale == meta)
  {
    gameOverOutput((turno%2) + 1, true);
  }
  else{appoggio = false; pirActivated = false;}         //altrimenti continua il gioco
}
//----------------------------------------------------------------------------------------------------
//METODI PER L'OUTPUT LCD
void insertWager()
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Inserire la");
  lcd.setCursor(0,1);
  lcd.print("puntata");
}

void firstWagerError()
{
  lcd.clear();
  delay(10);
  lcd.setCursor(0,0);            
  lcd.print("La prima puntata");
  lcd.setCursor(0,1);
  lcd.print("deve essere tra 0 e 6");
  int counter = 0;
  int leftOrRight = 0;
  while(counter <4)
  {
    scrollLcd(leftOrRight, 5);
    counter++;
    if(leftOrRight == 0){leftOrRight = 1;}
    else{leftOrRight = 0;}
    delay(350);
  }
  lcd.clear();
}

void confirmedMessage()
{
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Confermato");
      lcd.setCursor(0,1);
      String message = (String)totale;
      String message1 = " / ";
      String message2 = (String)meta;
      lcd.print(message + message1 + message2);   //iwreogbs<
      delay(1000);
      pirActivated = true;
}

void genericWagerError()
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("La puntata non");
  lcd.setCursor(0,1);
  lcd.print("e' concessa");
  delay(2000);
}

void updateMeta()
{
  lcd.clear();
  lcd.setCursor(0,0);
  String message = "Meta = ";
  lcd.print(message + meta);
}

void updateWager(int chosenWager)
{
  lcd.clear();
  lcd.setCursor(0,0);
  String message = "Puntata = ";
  lcd.print(message + chosenWager);
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
  lcd.print("  tra 30 e 100");
  delay(1000);
}

void scrollLcd(int leftOrRight, int numberOfScrolls)    //permette di spostare a destra o sinistra un messaggio su LCD per un numero di volte scelto (numberOfScrolls)
{                                                       //0-->verso sinistra  1--> verso destra
  for(int i = 0; i < numberOfScrolls; i++)
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
}

void gameOverOutput(int utente, bool controlToken)           //controlToken == false -> si ha superato il valore della meta
{                                                           //controlToken == true -> il valore del totale corrispone al valore della meta
  if(!controlToken)
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

void playNewGame()
{
  if(digitalRead(Pir) == HIGH)
  {
    setup(); 
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
      delay(1500);
      while(appoggio == true)
      {
        playNewGame();
      }
    }
