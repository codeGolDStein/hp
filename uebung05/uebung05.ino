// Pinbelegung Motoren
const int8_t A1_PIN = D1;  // GPIO5
const int8_t A2_PIN = D2;  // GPIO4
const int8_t B1_PIN = D5;
const int8_t B2_PIN = D6;

// Pinbelegung Sensoren
const int8_t S1_PIN = D4; 
const int8_t S2_PIN = D7;
const int8_t S3_PIN = D3;


// Abbiege Rheinfolge erreicht
bool finished = false;
int state = 0;


// 5cm = 447 duration 
// 10cm = 633 duration
// 15 cm = 947 duration
// 20 = 1233 duration


void setup() {
  // Motor Pins als Ausgang definieren
  pinMode(A1_PIN, OUTPUT);
  pinMode(A2_PIN, OUTPUT);
  pinMode(B1_PIN, OUTPUT);
  pinMode(B2_PIN, OUTPUT);
  
  Serial.begin(9600);
}

void loop() {

  float dist = measureDistance(S2_PIN);
  Serial.print("Distance (m): ");
  Serial.println(dist);
  delay(1000);


}

void doTask(bool run){
  // if finished quit
  if (!run || finished) {
    return;
  }
  

  switch(step) {
    case 0:
      // Step 1: Drive straight until distance is 60cm or less
      drive(true, 100, 150); // Drive forward for 100ms at speed 150
      
      // Check distance (convert from duration to approximate cm)
      // Based on your calibration: 5cm = 447, so ~89.4 duration per cm
      float dist = measureDistance(S2_PIN);
      if (dist > 0 && dist <= 535) { // ~60cm threshold (60 * 89.4 ≈ 535)
        step = 1; // Move to next step
      }
      break;
      
    case 1:
      // Turn right
      turn(false, 500, 150); // Turn right for 500ms at speed 150
      delay(200); // Brief pause after turn
      step = 2;
      break;
      
    case 2:
      // Step 2: Drive straight until distance is 60cm or less
      drive(true, 100, 150);
      
      dist = measureDistance(S2_PIN);
      if (dist > 0 && dist <= 535) { // ~60cm threshold
        step = 3;
      }
      break;
      
    case 3:
      // Turn left
      turn(true, 500, 150); // Turn left for 500ms at speed 150
      delay(200);
      step = 4;
      break;
      
    case 4:
      // Step 3: Drive straight until distance is 60cm or less
      drive(true, 100, 150);
      
      dist = measureDistance(S2_PIN);
      if (dist > 0 && dist <= 535) { // ~60cm threshold
        step = 5;
      }
      break;
      
    case 5:
      // Final turn left
      turn(true, 500, 150);
      delay(200);
      finished = true; // Mark sequence as complete
      step = 0; // Reset for potential future runs
      break;
  }
}



// Aufgabe 5: Misst die Entfernung in Metern (oder -1, wenn kein Hindernis erkannt)
float measureDistance(int pin) {
  // Schritt 1: Trigger-Puls senden
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
  delayMicroseconds(2);
  digitalWrite(pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(pin, LOW);

  // Schritt 2: Pin als Eingang setzen und Echo-Zeit messen
  pinMode(pin, INPUT);
  long duration = pulseIn(pin, HIGH, 30000);  // Timeout: 30 ms

  // Schritt 3: Kein Signal erhalten?
  if (duration == 0) {
    return -1.0;  // Kein Hindernis erkannt
  }

  // Schritt 4: Entfernung berechnen (Meter)
  //float distance = (duration * 0.0343) / 2.0;
  return duration;
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
