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
  setMotor(true, 300, false);
  delay(2000);

    // Drive forward for 2 seconds at medium speed
  drive(true, 2000, 600);

  delay(1000);

  // Drive backward for 1.5 seconds at full speed
  drive(false, 1500, 1023);

  delay(1000);

  // Rückwärts für 2 Sekunden
  //setMotor(false, 300);
  //delay(2000);
}

// Neue Funktion mit Geschwindigkeitskontrolle (motor = true -> left Wheel)
void setMotor(bool forward, uint16_t speed, bool motor) {
  
  int8_t Motor1 = A1_PIN;
  int8_t Motor2 = A2_PIN;
  uint16_t speedReverse = 0;


  if (!motor){
    Motor1 = B1_PIN;
    Motor2 = B2_PIN;
    speedReverse = speed;
    speed = 0;
  }

  if (forward) {
    analogWrite(Motor1, speedReverse);
    analogWrite(Motor2, speed);
  } else {
    analogWrite(Motor1, speed);
    analogWrite(Motor2, speedReverse);
  }
}

// Drive both motors forward/backward for a duration (ms) with given speed
void drive(bool forward, uint32_t duration, uint16_t speed) {
  setMotor(forward, speed, true);   // Motor A
  //setMotor(forward, speed, false);  // Motor B

  delay(duration);

  // Stop both motors
  setMotor(true, 0, true);
  setMotor(true, 0, false);
}
