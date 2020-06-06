#include <Arduino.h>
#include <Wire.h> // I2C Bibleothek
#include <LiquidCrystal_I2C.h>  //Bibleothek für I2C Displayansteuerung
#include <LowPower.h> // Bibleothek für Low Power Mode
#include <avr/wdt.h>  //Bibleothek für Watchdogtimer

LiquidCrystal_I2C lcd(0x27,20,4);  // LCD Adresse ist 0x27, 16 Zeichen und 2 Zeilen

//
//Variablen Deklaration
//
int InterruptPin = PB2; //Externer Interrupt an Pin PB2 wird verwendet
int val[8], volt[8], pin=34, Zustand=0,i=0;
static int AnalogAnzahl=6,AnalogStartPin=32;
static double Referenzspannung=5.0, Maximalspannung=15.0, Maximalstrom=6.0;
bool Starttaster=false, Fehler=false;

void InitializeTimers()
{
  wdt_disable();  // Watchdog ausschalten
  
  //
  // Ausgabepins initialisieren
  //
  pinMode(PD5,OUTPUT);
  pinMode(PB6,OUTPUT);
  digitalWrite(PD5,LOW);
  digitalWrite(PB6,LOW);

  //
  // Timer 1 (16bit) für PWM mit variabler Frequenz initialisieren
  //
  TCCR1A = 0b10000010;  //  OC1A löschen, bei Compare Match (Fast PWM Mode 14, mit Top = ICR1)
  TCNT1=0;  // Timer 1 zurücksetzen
  ICR1 = 100;  // Zählerstand für Periodendauer (79,21kHz)
  OCR1A = 70; // Compare Wert für Impulsdauer (70%)

  //
  // Timer 3 (16bit)  für PWM mit variaabler Frequenz initialisieren
  //
  TCCR3A = 0b10000010;  //  OC3A löschen, bei Compare Match (Fast PWM Mode 14, mit Top = ICR3)
  TCNT3=0;  // Timer 3 zurücksetzen
  ICR3 = 100;  // Zählerstand für Periodendauer (79,21kHz)
  OCR3A = 70; // Compare Wert für Impulsdauer (70%)
}

void InitializeDisplay()
{
  Serial.begin(9600); //Baudrate der seriellen Schnittstelle wird auf 9600 gesetzt
  //
  // Initialisierung des Displays
  //
  lcd.init(); // LCD Display wird initialisiert 
  lcd.backlight();  // Hintergrundbeleuchtung wird angeschalten
  lcd.setCursor(0,0); // Coursor wird auf erstes Zeichen in der ersetn Zeile gesetzt
  lcd.print("Solar Laderegler"); // Auf dem Display wird "Hello, world!" ausgegeben
  lcd.setCursor(0,1);
  lcd.print("Gruppe 2");
  delay(1000);
}

void InitializePorts()
{
  //
  // Analogeingänge konfigurieren
  //
  for(int i=0;i<AnalogAnzahl;i++)
  {
    pinMode(pin,INPUT);
    pin++;
  }
  pin=AnalogStartPin;
}

void TimerStart()
{
  //
  //  Timer starten
  //
  TCCR1B = 0b00011001; // Vorteiler = 1
  delayMicroseconds(58);  // 180° Phasenverschiebung
  TCCR3B = 0b00011001; // Vorteiler = 1
}

void TimerStop()
{
  //
  //  Timer stoppen
  //
  TCCR1B = 0b00011000;
  TCCR3B = 0b00011000;
  TCNT1=0;
  TCNT3=0;
  delay(200);
  digitalWrite(PD5,LOW);
  digitalWrite(PB6,LOW);
}

