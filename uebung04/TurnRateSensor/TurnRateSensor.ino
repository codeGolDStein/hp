// TurnRate Sensor: read data from analog gyroscope, output heading on lcd

/* Connections:
 R/S : Pin 11
 R/W : not connected
 E   : Pin 12
 DB4 : Pin 13
 DB5 : Pin A0
 DB6 : Pin A1
 DB7 : Pin A2
 
 Motor:
 A1 : Pin 5
 A2 : Pin 6
 B1 : Pin 9
 B2 : Pin 10
 Turn-Rate Sensor: A3
*/

// include LCD functions:
#include <LiquidCrystal.h> 


// define the LCD screen
LiquidCrystal lcd(11, 12, 13, A0, A1, A2);


//LiquidCrystal lcd(R_S, E, DB4, DB5, DB6, DB7);

// Pins of motor
#define PIN_MOTOR_A1 5
#define PIN_MOTOR_A2 6
#define PIN_MOTOR_B1 9
#define PIN_MOTOR_B2 10

const uint8_t motorPins[] = { PIN_MOTOR_A1, PIN_MOTOR_A2, PIN_MOTOR_B1, PIN_MOTOR_B2 };

/*Aufgabe 2*/
// Variable für den Ruhewert des Sensors
int16_t gyroOffset = 0;
bool offsetCalculated = false;

// Aufgabe 3
unsigned long lastMicros = 0;
long heading_int = 0;
const int DEADZONE = 5;  // Turnrate unterhalb dieses Werts wird ignoriert


// initialization
void setup()
{
   // Pins A0, A1, A2 als Ausgänge konfigurieren
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);

  lcd.clear();
  lcd.begin(20, 4); 
  lcd.setCursor(0, 0);
  lcd.print("Kalibrierung...");
  
  // Warten auf Stabilisierung nach dem Einschalten
  delay(2000);
  
  /*Aufgabe 2*/
  // Berechnung des Mittelwerts über mehrere Messungen
  long sum = 0;
  const int numSamples = 100;
  
  for(int i = 0; i < numSamples; i++) {
    sum += analogRead(A3);
    delay(10);  // Kurze Pause zwischen Messungen
  }
  
  gyroOffset = sum / numSamples;
  
  lastMicros = micros();  // Initiale Zeit merken

  // LCD für normale Anzeige vorbereiten
  lcd.clear();

  /*Aufgabe 1*/
  lcd.setCursor(0, 0);
  lcd.print("ADC: ");
  lcd.setCursor(0, 1);
  lcd.print("turn-rate: ");
  
}

void loop()
{ 
  /* --> Aufgabe 1
  // read the current analog value on a3
  int16_t analogValue = analogRead(A3);
  // implement your code here:

    
  lcd.setCursor(0, 0);  // Zeile 1, Position 0
  lcd.print("ADC: ");
  lcd.print(analogValue);
  lcd.print("   ");  // Überschreibt Restzeichen
  */

  // Aufgabe 2
  // #########################
    
    // Aktuellen ADC-Wert lesen
  int16_t analogValue = analogRead(A3);
    
  // Drehrate berechnen (aktueller Wert - Ruhewert)
  int16_t turnRate = analogValue - gyroOffset;
  
  // ADC-Wert in erster Zeile anzeigen
  lcd.setCursor(5, 0);
  lcd.print(analogValue);
  lcd.print("   ");  // Überschreibt Restzeichen
  
  // Drehrate in zweiter Zeile anzeigen
  lcd.setCursor(11, 1);
  lcd.print(turnRate);
  lcd.print("   ");  // Überschreibt Restzeichen
   
  // #########################


  // Aufgabe 3
  // #########################
  // Zeit berechnen
  unsigned long currentMicros = micros();
  float deltaTime = (currentMicros - lastMicros) / 1e6;  // Zeit in Sekunden
  lastMicros = currentMicros;

  // Deadzone einführen (Sensorrauschen ignorieren)
  if (abs(turnRate) > DEADZONE) {
    heading_int += turnRate * deltaTime;  // Integration
  }

  // Ausgabe in Zeile 3
  lcd.setCursor(0, 2);  // dritte Zeile
  lcd.print("heading_int: ");
  lcd.print(heading_int);
  lcd.print("   ");
  // #########################
}


/* Usefull LCD functions:
set the current write position of the lcd to specific line and row:
  lcd.setCursor(row, line);  

write onto lcd, starting at current position:
  lcd.print("abc");
*/

