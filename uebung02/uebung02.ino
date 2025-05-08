// Pinbelegung
const int8_t A1 = D1;  // GPIO5
const int8_t A2 = D2;  // GPIO4

void setup() {
  // Pins als Ausgang definieren
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  
  // Optional: Motor am Anfang stoppen
  digitalWrite(A1, LOW);
  digitalWrite(A2, LOW);
}

void loop() {
  // Beispiel: Vorwärts für 2 Sekunden
  setMotor(true, 300);
  delay(2000);

  // Rückwärts für 2 Sekunden
  //setMotor(false, 300);
  //delay(2000);
}

// Neue Funktion mit Geschwindigkeitskontrolle
void setMotor(bool forward, uint16_t speed) {
  // Begrenze den Wert auf gültigen Bereich
  speed = constrain(speed, 0, 1023);

  if (forward) {
    analogWrite(A1, 0);         // LOW
    analogWrite(A2, speed);     // PWM Signal
  } else {
    analogWrite(A1, speed);     // PWM Signal
    analogWrite(A2, 0);         // LOW
  }
}
