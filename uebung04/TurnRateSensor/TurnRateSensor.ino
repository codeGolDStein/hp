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

// Pins of motor
#define PIN_MOTOR_A1 5
#define PIN_MOTOR_A2 6
#define PIN_MOTOR_B1 9
#define PIN_MOTOR_B2 10

const uint8_t motorPins[] = { PIN_MOTOR_A1, PIN_MOTOR_A2, PIN_MOTOR_B1, PIN_MOTOR_B2 };

// Aufgabe 2
// Variable für den Ruhewert des Sensors
int16_t gyroOffset = 0;
// Aufgabe 3
//unsigned long lastMicros = 0;
long heading_int = 0;
// const int DEADZONE = 5;
// Aufgabe 6 - Zustandsautomat Variablen
char state = '0';  // Zustand: '0' = drive forward, '1' = rotate right
unsigned long timer = 0;  // Timer in Millisekunden
int targetHeading = 0;  // Zielrichtung in Grad
const int8_t TOLERANCE = 2;  // Toleranzbereich für heading Vergleich
unsigned long timerState = 0;
int16_t turnRate = 0;

// initialization
void setup()
{
   // Pins A0, A1, A2 als Ausgänge konfigurieren
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  
  
  // Motor Pins als Ausgänge konfigurieren
  for(int i = 0; i < 4; i++) {
    pinMode(motorPins[i], OUTPUT);
  }

  lcd.clear();
  lcd.begin(20, 4); 
  lcd.setCursor(0, 0);
  lcd.print("Kalibrierung...");
  
  // Warten auf Stabilisierung nach dem Einschalten
  delay(2000);
  
  // Aufgabe 2
  // Berechnung des Mittelwerts über mehrere Messungen
  long sum = 0;
  const int numSamples = 100;
  
  for(int i = 0; i < numSamples; i++) {
    sum += analogRead(A3);
    delay(10);  // Kurze Pause zwischen Messungen
  }
  
  gyroOffset = sum / numSamples;
  
  // Timer initialisieren
  timer = millis();

  // LCD für normale Anzeige vorbereiten
  lcd.clear();

  /*Aufgabe 1*/
  lcd.setCursor(0, 0);
  lcd.print("ADC: ");
  lcd.setCursor(0, 1);
  lcd.print("turn-rate: ");
  
  timerState = timer + 4000;

}

void loop()
{ 
  /* Aufgabe 2   */
  // Aktuellen ADC-Wert lesen
  int16_t analogValue = analogRead(A3);

  // ADC-Wert in erster Zeile anzeigen
  lcd.setCursor(5, 0);
  lcd.print(analogValue);
  lcd.print("   ");  // Überschreibt Restzeichen
  
  // Drehrate in zweiter Zeile anzeigen
  lcd.setCursor(11, 1);
  lcd.print(turnRate);
  lcd.print("   ");  // Überschreibt Restzeichen
    
  int heading = getHeading();

  // Aufgabe 3
  /*
  lcd.setCursor(0, 2);  // dritte Zeile
  lcd.print("heading: ");
  lcd.print(heading_int);
  lcd.print((char)223);  // Gradzeichen
  lcd.print("   ");
  */

  // Aufgabe 4
  lcd.setCursor(0, 2);  // dritte Zeile
  lcd.print("heading: ");
  lcd.print(heading);
  lcd.print((char)223);  // Gradzeichen
  lcd.print("   ");

  // Aufgabe 6 - Zustandsautomat
  doState(heading);

  // targetHeading auf LCD anzeigen (dritte Zeile, kommentiert heading_int aus)
  lcd.setCursor(0, 3);  // 4te Zeile
  lcd.print("target: ");
  lcd.print(targetHeading);
  lcd.print((char)223);  
  lcd.print(" S:");
  lcd.print(state);
  lcd.print("  ");
}

// Aufgabe 3 und 4
int getHeading() {
  // Aufgabe 3
  int16_t analogValue = analogRead(A3);
  turnRate = gyroOffset - analogValue;
  unsigned long headingTimer = millis();
  if (!(turnRate <= 5 and turnRate >= -5)) {
    heading_int += turnRate * (headingTimer- timer);
  }
  // Aufgabe 4
  timer = headingTimer;
  // Sklarieungsfaktor  = 2280 (wir haben einfach 
  int heading = (heading_int / 2280) % 360;
  if (heading < 0) {
   heading += 360;
  }
  return heading;
}

// Aufgabe 5
void setMotor(bool forward, uint8_t speed, bool motorA) {
  int pin1, pin2;
  
  if (motorA) {
    pin1 = PIN_MOTOR_A1;
    pin2 = PIN_MOTOR_A2;
  } else {
    pin1 = PIN_MOTOR_B1;
    pin2 = PIN_MOTOR_B2;
  }

  if (forward) {
    digitalWrite(pin1, LOW);     
    analogWrite(pin2, speed);     
  } else {
    analogWrite(pin1, speed);     
    digitalWrite(pin2, LOW);      
  }
}

// Aufgabe 6
void doState(int heading) {

  // Zustandsautomat
  if (state == '0') {  // State 0: drive forward

    // Vorwärts fahren
    setMotor(true, 50, true);   // Motor A vorwärts
    setMotor(true, 50, false);  // Motor B vorwärts
    
    // Prüfen ob 4 Sekunden vergangen sind
    if (millis() >= timerState) {
      // --> State 1
      state = '1';
      // Zielrichtung berechnen (aktueller heading + 120)
      targetHeading = (heading + 120) % 360;
      // Timer reset
      //timer = currentTime;
    }
  }
  else if (state == '1') {  // rotate right
    // Rechts drehen (Motor A vorwärts, Motor B rückwärts)
    setMotor(true, 50, true);    // Motor A vorwärts
    setMotor(false, 50, false);  // Motor B rückwärts
    
    int TOLERANCE = 3;
    bool targetReached = false;

    // Prüfen ob Zielrichtung erreicht ist
    if ((heading - targetHeading) <= TOLERANCE && (heading - targetHeading) > -1 ) {targetReached = true;}

    if (targetReached) {
      // --> State 0
      state = '0';
      heading_int = 0;
      // set to current time plus 4000
      timerState = millis() + 4000;  // Set next target time
    }
  }
}

/* Usefull LCD functions:
set the current write position of the lcd to specific line and row:
  lcd.setCursor(row, line);  

write onto lcd, starting at current position:
  lcd.print("abc");
*/
