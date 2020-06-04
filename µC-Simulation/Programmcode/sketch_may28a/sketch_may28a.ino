#include <LowPower.h> // Bibleothek für Low Power Mode
#include <Wire.h> // I2C Bibleothek
#include <LiquidCrystal_I2C.h>  //Bibleothek für I2C Displayansteuerung
#include <avr/wdt.h>

LiquidCrystal_I2C lcd(0x27,20,4);  // LCD Adresse ist 0x27, 16 Zeichen und 2 Zeilen

//
//Variablen Deklaration
//
int InterruptPin = PB2; //Externer Interrupt an Pin PB2 wird verwendet
int val[8], volt[8], pin=32;

void setup()
{
  wdt_disable();
  Serial.begin(9600); //Baudrate der seriellen Schnittstelle wird auf 9600 gesetzt
  
  //
  // Externer Interrupt wird konfiguriert: Interruptname (INT2) für PB2 wird gesucht, die ISR ist 
  // WakeUpHandler und es wird auf eine fallende Flanke reagiert
  //
  attachInterrupt(digitalPinToInterrupt(InterruptPin), WakeUpHandler, FALLING); 

  //
  // Initialisierung des Displays
  //
  lcd.init(); // LCD Display wird initialisiert 
  lcd.backlight();  // Hintergrundbeleuchtung wird angeschalten
  lcd.setCursor(1,0); // Coursor wird auf erstes Zeichen in der ersetn Zeile gesetzt
  lcd.print("Hello, world!"); // Auf dem Display wird "Hello, world!" ausgegeben

  //
  // Analogeingänge konfigurieren
  //
  for(int i=0;i<8;i++)
  {
    pinMode(pin,INPUT);
    pin++;
  }
  pin=32;

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
  
  TCCR1B = 0b00011001; // Vorteiler = 1
  delayMicroseconds(58);  // 180° Phasenverschiebung
  TCCR3B = 0b00011001; // Vorteiler = 1
}


void loop()
{
  Serial.println("Das ist meine Debugschnittstelle"); // Auf dem Terminal wird "Das ist meine Debugschnittstelle" ausgegeben
  delay(500); // 500ms Wartezeit
  
  //
  // Analogeingäng einlesen
  //
  for(int i=0;i<8;i++)
  {
    val[i]=analogRead(pin);
    volt[i]=map(val[i],0,1023,0,500);
    Serial.println(volt[i]/100.0);
    delay(300);
    pin++;
  }

  Serial.println("Gehe schlafen");
  delay(100); // Wartezeit, um den Text auszugeben, bevor das Display ausgeschaltet wird
  lcd.noBacklight();  // Hintergrundbeleuchtung wird ausgeschalten
  lcd.noDisplay();  // Textanzeige wird ausgeschalten
  LowPower.powerDown(SLEEP_FOREVER,ADC_OFF,BOD_OFF); // Power Down Mode wird aktiviert und erst bei externem Interrupt oder Reset wieder ausgeschalten
  Serial.println("Bin aufgewacht");
  lcd.backlight();
  lcd.display();
  lcd.clear();  // Text in Display löschen
  lcd.print("Bin aufgewacht");
  delay(1000);
}

//
// ISR
//
void WakeUpHandler() {
}

