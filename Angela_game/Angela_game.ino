//Luca Giacometti 2019

#include <LiquidCrystal.h>
//variabili globali costanti
const int buttonForward = 8;
const int buttonBackward = 9;
const int ledUtente1 = 2;
const int ledUtente2 = 3;
const int Pir = 10;
const int delay_ = 2000;

//variabili globali
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
  pinMode(buttonForward, INPUT_PULLUP);
  pinMode(buttonBackward, INPUT_PULLUP);
  pinMode(ledUtente1, OUTPUT);
  pinMode(ledUtente2, OUTPUT);
  pinMode(Pir, INPUT);
  digitalWrite(ledUtente1, LOW);
  digitalWrite(ledUtente2, LOW);
  lcd.begin(16, 2);
  lcd.clear();
  Greeting();
}

void loop() {
  // put your main code here, to run repeatedly:
  inserisciMeta();
  while (appoggio == false)
  {
    attendiPuntata();
    check();
  }
  delay(delay_-500);
  while (appoggio == true)
  {
    playNewGame();
  }
}

//****************************************************************************************************************************************************************************************
//METODI
void inserisciMeta()
{
  metaInfo();
  getMetaValue();
}

void getMetaValue()
{
  bool finito = false;
  bool valueChanged = false;
  while (!finito)
  {
    if (digitalRead(buttonForward) == LOW && meta < 99) //bottone "avanti" premuto
    {
      meta++;
      valueChanged = true;
    }
    if (digitalRead(buttonBackward) == LOW && meta > 30) //bottone "indietro" premuto
    {
      meta--;
      valueChanged = true;
    }
    if (valueChanged)
    {
      valueChanged = false;
      updateMeta();
      delay(delay_/10);
    }
    if (digitalRead(Pir) == HIGH)
    {
      confirmMessage();
      delay(delay_/2);
      updateMeta();
      finito = true;
    }
  }
}

void checkFirstTurn(int puntataScelta)         //accetta la puntata in input del primo turno e aggiorna il valore della puntata totale
{
  puntata = puntataScelta;
  totale = puntataScelta;
  appoggio = true;
  turno = turno + 1;
  confirmedMessage();
}

void checkTurn(int puntataScelta)        //acceta la puntata in input di tutti i turni eccetto il primo e aggiorna il valore della puntata totale
{
  if (puntataScelta > 0 && puntataScelta != puntata && puntataScelta != 7-puntata)
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
    lcd.clear();
    String message = "Puntata = ";
    lcd.print(message + (String)puntataScelta);
  }
}

void refreshLed()
{
  if (turno % 2 == 0)
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
  delay(delay_-1000);
  int chosenWager = 0;
  bool wagerUpdated = false;
  pirActivated = false;
  while (!appoggio)
  {
    if (digitalRead(buttonForward) == LOW && chosenWager < 6) //bottone "avanti" premuto
    {
      chosenWager++;
      delay(delay_-1650);
      wagerUpdated = true;
    }
    if (digitalRead(buttonBackward) == LOW && chosenWager > 0) //bottone "indietro" premuto
    {
      chosenWager--;
      delay(delay_-1650);
      wagerUpdated = true;
    }
    if (wagerUpdated)
    {
      updateWager(chosenWager);
      wagerUpdated = false;
    }
    if (digitalRead(Pir) == HIGH)                                          
    {
      if (!pirActivated)
      {
        if (turno == 0)
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
  if (totale > meta)
  {
    gameOverOutput((turno % 2)+1, false);
  }
  else if (totale == meta)
  {
    int utente = (turno % 2) + 1;
    if(utente == 1)
    {
      utente = 2;
    }
    else
    {
      utente = 1;
    }
    gameOverOutput(utente, true);
  }
  else {
    appoggio = false;  //altrimenti continua il gioco
    pirActivated = false;
  }
}

void playNewGame()
{
  ANIM_M1();
    if (digitalRead(Pir) == HIGH)
  {
    appoggio = false;
  }
  for(int i=0; i<2;i++)
  { ANIM_CLOSE(); }
    if (digitalRead(Pir) == HIGH)
  {
    appoggio = false;
  }
  ANIM_1();
    if (digitalRead(Pir) == HIGH)
  {
     appoggio = false;
  }
  if(!appoggio)
  {
    delay(delay_);
    refreshing();
    setup();
  }
}

//****************************************************************************************************************************************************************************************
//METODI PER L'OUTPUT LCD
void insertWager()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Inserire la");
  lcd.setCursor(0, 1);
  String message = "puntata    ";
  String message1 = (String)totale;
  String message2 = "/";
  String message3 = (String)meta;
  lcd.print(message + message1 + message2 + message3);
}

void confirmedMessage()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Confermato");
  lcd.setCursor(11, 1);
  String message = (String)totale;
  String message1 = "/";
  String message2 = (String)meta;
  lcd.print(message + message1 + message2);
  delay(delay_-500);
}

void genericWagerError()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("La puntata non");
  lcd.setCursor(0, 1);
  lcd.print("e' accettabile!");
  delay(delay_/2);
}

void Greeting()
{
  lcd.setCursor(0,0);
  lcd.print("  Angela game");
  lcd.setCursor(0,1);
  lcd.print("    by Luca");
  delay(delay_);
  for(int i = 0; i < 17; i++)
  {
    lcd.scrollDisplayRight();
    delay(50);
  }
}

void updateMeta()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  String message = "Meta = ";
  lcd.print(message + meta);
}

void updateWager(int chosenWager)
{
  lcd.clear();
  lcd.setCursor(0, 0);
  String message = "Puntata = ";
  lcd.print(message + chosenWager);
}

void confirmMessage()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Confermato!");
  lcd.setCursor(0, 1);
  String message = "Meta = ";
  lcd.print(message + (String)meta);
}

void metaInfo() {
  lcd.clear();
  lcd.print("Inserire la meta");
  lcd.setCursor(0, 1);
  lcd.print("  tra 30 e 100");
  delay(delay_/2);
}

void gameOverOutput(int utente, bool controlToken)           //controlToken == false -> si ha superato il valore della meta
{ //controlToken == true -> il valore del totale corrispone al valore della meta
  digitalWrite(ledUtente1, LOW);
  digitalWrite(ledUtente2, LOW);
  if (!controlToken)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Hai superato la");
    lcd.setCursor(0, 1);
    lcd.print("      meta!");
    delay(delay_);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("   Vincitore:");
    lcd.setCursor(0,1);
    lcd.print("    Utente " + (String)utente);
    delay(delay_);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" Durata match:");
    lcd.setCursor(4,1);
    lcd.print((String)turno + " turni");
  }
  else
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Hai raggiunto la");
    lcd.setCursor(0, 1);
    lcd.print("meta!");
    delay(delay_);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("   Vincitore:");
    lcd.setCursor(0,1);
    lcd.print("    Utente " + (String)utente);
    delay(delay_);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" Durata match:");
    lcd.setCursor(4,1);
    lcd.print((String)turno + " turni");
  }
  delay(delay_ - 500);
  lcd.clear();
}

void refreshing()
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" .");
  delay(delay_/5);
  lcd.clear();
  lcd.print(" .   .");
  delay(delay_/5);
  lcd.clear();
  lcd.print(" .   .   .");
  delay(delay_/5);
  lcd.clear();
  lcd.print(" .   .   .   .");
  delay(delay_/5);
  lcd.clear();
  /*
  lcd.print(".  .  .  .  .");
  delay(delay_/5);
  lcd.clear();
  lcd.print(".  .  .  .  .  .");
  delay(delay_/5);
  lcd.clear();*/
}
//****************************************************************************************************************************************************************************************

