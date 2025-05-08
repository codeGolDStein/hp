// Pinbelegung Motoren
const int8_t A1_PIN = D1;  // GPIO5
const int8_t A2_PIN = D2;  // GPIO4
const int8_t B1_PIN = D5;
const int8_t B2_PIN = D6;

// Pinbelegung Sensoren
const int8_t S1_PIN = D4; 
const int8_t S2_PIN = D7;
const int8_t S3_PIN = D3;

enum Sensoren {
  Sen1,
  Sen2,
  Sen3,
};


void setup() {
  // Motor Pins als Ausgang definieren
  pinMode(A1_PIN, OUTPUT);
  pinMode(A2_PIN, OUTPUT);
  pinMode(B1_PIN, OUTPUT);
  pinMode(B2_PIN, OUTPUT);

  // Sensor Pins als Ausgang definieren
  pinMode(S1_PIN, OUTPUT);
  pinMode(S2_PIN, OUTPUT);
  pinMode(S3_PIN, OUTPUT);
  
}

void loop() {
  // Drive backward for 1.5 seconds at full speed
  drive(false, 1500, 4000);

  delay(1000);

  turn(true, 2000, 400);

  delay(2000);

  turn(false, 2000, 400);

  // Rückwärts für 2 Sekunden
  //setMotor(false, 300);
  //delay(2000);
}

int8_t measureDistance(Sensoren sensoren){
  
  int distance;
  int8_t selectedPIN;

  switch(sensoren) {
    case S1:
      selectedPIN = S1_PIN;
    case S2:
      selectedPIN = S2_PIN;
    case S3:
      selectedPIN = S3_PIN;
  }

  pinMode(selectedPIN, OUTPUT);
  digitalWrite(selectedPIN, LOW);
  delay(2):
  digitalWrite(selectedPIN, HIGH);
  delay(10);
  digitalWrite(selectedPIN, LOW);
  distance = pulseIn(selectedPIN, HIGH);


  return distance;
}

// Neue Funktion mit Geschwindigkeitskontrolle (motor = true -> left Wheel)
void setMotor(bool forward, uint16_t speed, bool motor) {
  
  int8_t Motor1 = A1_PIN;
  int8_t Motor2 = A2_PIN;
  uint16_t speedReverse = 0;

  // Aufgabe 2
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

// Aufgabe 3
// Drive both motors forward/backward for a duration (ms) with given speed
void drive(bool forward, uint32_t duration, uint16_t speed) {
  setMotor(forward, speed, true);   // Motor A
  setMotor(forward, speed, false);  // Motor B

  delay(duration);

  // Stop both motors
  setMotor(true, 0, true);
  setMotor(true, 0, false);
}


// turn, if left = true -> 
void turn(bool left, uint32_t duration, uint16_t speed) {
  setMotor(left, speed, true);   // Motor A
  setMotor(!left, speed, false);  // Motor B

  delay(duration);

  // Stop both motors
  setMotor(true, 0, true);
  setMotor(true, 0, false);
}
