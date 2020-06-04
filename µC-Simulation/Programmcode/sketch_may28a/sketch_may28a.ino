#include "Initialisierung.h"

void setup()
{
  
}

void loop()
{
  switch(Zustand)
  {
    case 0: // Initialisierung
    {
      //
      // Externer Interrupt wird konfiguriert: Interruptname (INT2) für PB2 wird gesucht, die ISR ist 
      // WakeUpHandler und es wird auf eine fallende Flanke reagiert
      //
      attachInterrupt(digitalPinToInterrupt(InterruptPin), WakeUpHandler, FALLING);
      InitializeDisplay();
      InitializePorts();
      InitializeTimers();
      if(Starttaster)
      {
        Zustand=2;
      }
      else
      {
        Zustand=1;
      }
    }
    case 1: // Standby
    {
      i=0;
      lcd.clear();
      lcd.print("Standby");
      delay(200); // Wartezeit, um den Text auszugeben, bevor das Display ausgeschaltet wird
      lcd.noBacklight();  // Hintergrundbeleuchtung wird ausgeschalten
      lcd.noDisplay();  // Textanzeige wird ausgeschalten
      LowPower.powerDown(SLEEP_FOREVER,ADC_OFF,BOD_OFF); // Power Down Mode wird aktiviert und erst bei externem Interrupt oder Reset wieder ausgeschalten
      Starttaster=false;
      lcd.backlight();
      lcd.display();
      lcd.clear();  // Text in Display löschen
      lcd.print("Aktiv");
      delay(500);
      Zustand=2;
      TimerStart(); // PWM wird gestartet
      break;
    }
    case 2: // Aktiv
    {
      //
      // Analogeingäng einlesen
      //
      for(i;i<8;i++)
      {
        val[i]=analogRead(pin);
        volt[i]=map(val[i],0,1023,0,500);
        Serial.println(volt[i]/100.0);
        if((volt[i]/100.0)<1)
        {
          Fehler=true;
          break;
        }
        else
        {
          delay(300);
          pin++;
        }
      }
      if(Fehler)
      {
        Zustand=-1;
      }
      else
      {
        pin=32;
        delay(500);
        Zustand=1;
        TimerStart();
      }
      break;
    }
    case -1:  //Fehler
    {
      //
      // Externer Interrupt wird konfiguriert: Interruptname (INT2) für PB2 wird gesucht, die ISR wird von 
      // WakeUpHandler auf FehlerBehoben geändert und es wird auf eine fallende Flanke reagiert
      //
      detachInterrupt(digitalPinToInterrupt(InterruptPin));
      attachInterrupt(digitalPinToInterrupt(InterruptPin), FehlerBehoben, FALLING);
      lcd.clear();
      lcd.print("Fehler");
      while(Starttaster==false)
      {
        digitalWrite(PB0,HIGH); // LED für die Visualisierung des Fehlers
      }
      digitalWrite(PB0,LOW);
      lcd.clear();
      lcd.print("Aktiv");
      //
      // Externer Interrupt wird konfiguriert: Interruptname (INT2) für PB2 wird gesucht, die ISR wird von 
      // FehlerBehoben auf WakeUpHandler geändert und es wird auf eine fallende Flanke reagiert
      //
      detachInterrupt(digitalPinToInterrupt(InterruptPin));
      attachInterrupt(digitalPinToInterrupt(InterruptPin), WakeUpHandler, FALLING);
      Fehler=false;
      Starttaster=false;
      Zustand=2;
      break;
    }
  }
}

//
// ISR
//
void WakeUpHandler() {
  Starttaster=true;
}

void FehlerBehoben()
{
  Starttaster=true;
}



