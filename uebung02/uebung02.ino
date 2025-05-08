// Pinbelegung
const int8_t A1_PIN = D1;  // GPIO5
const int8_t A2_PIN = D2;  // GPIO4
const int8_t B1_PIN = D5;
const int8_t B2_PIN = D6;


void setup() {
  // Pins als Ausgang definieren
  pinMode(A1_PIN, OUTPUT);
  pinMode(A2_PIN, OUTPUT);
  pinMode(B1_PIN, OUTPUT);
  pinMode(B2_PIN, OUTPUT);
  
}

void loop() {
  // Beispiel: Vorwärts für 2 Sekunden
  setMotor(true, 300, true);
  delay(2000);

  // Rückwärts für 2 Sekunden
  //setMotor(false, 300);
  //delay(2000);
}

// Neue Funktion mit Geschwindigkeitskontrolle (motor = true -> right Wheel)
void setMotor(bool forward, uint16_t speed, bool motor) {
  
  int8_t Motor1 = B1_PIN;
  int8_t Motor2 = B2_PIN;

  if (!motor){
    Motor1 = A1_PIN;
    Motor2 = A2_PIN;
  }

  if (forward) {
    analogWrite(Motor1, 0);
    analogWrite(Motor2, speed);
  } else {
    analogWrite(Motor1, speed);
    analogWrite(Motor2, 0);
  }
}
