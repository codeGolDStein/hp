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

/*Aufgabe 2*/
// Variable für den Ruhewert des Sensors
int16_t gyroOffset = 0;
bool offsetCalculated = false;

// Aufgabe 3
unsigned long lastMicros = 0;
float heading_int = 0;
const int DEADZONE = 5;  // Turnrate unterhalb dieses Werts wird ignoriert

// Aufgabe 6 - Zustandsautomat Variablen
int state = 0;
unsigned long lastTime = 0;
unsigned long timer = 0;
float targetHeading = 0;

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
  timer = millis();  // Timer initialisieren

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
  /* Aufgabe 2   */
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
   
  /* Aufgabe 3 */
  // Zeit berechnen
  unsigned long currentMicros = micros();
  float deltaTime = (currentMicros - lastMicros) / 1e6;  // Zeit in Sekunden
  lastMicros = currentMicros;

  // Deadzone einführen 
  if (abs(turnRate) > DEADZONE) {
    heading_int += turnRate * deltaTime;
  }

    // Aufgabe 4
    const float SCALE_FACTOR = 0.71;
    int heading = (int)(fmod((heading_int * SCALE_FACTOR), 360.0));
    if (heading < 0) heading += 360;

    lcd.setCursor(0, 2);  // dritte Zeile
    lcd.print("heading: ");
    lcd.print(heading);
    lcd.print((char)223);  // Gradzeichen
    lcd.print("   ");

  // Aufgabe 6 - Zustandsautomat
  unsigned long currentTime = millis();
  float dt = (currentTime - lastTime) / 1000.0;  // in Sekunden

  // Jetzt erst lastTime updaten!
  lastTime = currentTime;

  float currentHeading = heading;

  switch (state) {
    case 0:
      // drehen
      setMotor(true, 70, true);
      setMotor(false, 70, false);
      timer += dt;

      if (timer >= 4.0) {
        targetHeading = currentHeading + 120;
        if (targetHeading >= 360) targetHeading -= 360;

        state = 1;
        timer = 0;
      }
      break;

    case 1:
      // vorwärts fahren
      setMotor(true, 20, true);
      setMotor(true, 40, false);

      float delta = abs(currentHeading - targetHeading);
      if (delta < 2 || delta > 358) {
        state = 0;
        timer = 0;
      }
      break;
  }

  // targetHeading auf LCD anzeigen (dritte Zeile, kommentiert heading_int aus)
  lcd.setCursor(0, 3);  // 4te Zeile
  lcd.print("target: ");
  lcd.print(targetHeading);
  lcd.print((char)223);  // deg
  lcd.print(" S:");
  lcd.print(state);
  lcd.print("  ");
}

/*Aufgabe 5*/
// setMotor: Steuert einen Motor (A oder B) mit Richtung und Geschwindigkeit
// forward: true = vorwärts, false = rückwärts
// speed: (0-255)
// motorA: true = Motor A, false = Motor B
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
    digitalWrite(pin1, LOW);      // pin1 = 0
    analogWrite(pin2, speed);     // pin2 = PWM
  } else {
    analogWrite(pin1, speed);     // pin1 = PWM
    digitalWrite(pin2, LOW);      // pin2 = 0
  }
}

/* Usefull LCD functions:
set the current write position of the lcd to specific line and row:
  lcd.setCursor(row, line);  

write onto lcd, starting at current position:
  lcd.print("abc");
*/
