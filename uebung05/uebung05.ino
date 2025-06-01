// Pins of motor
#define MOTOR_A1_PIN D1
#define MOTOR_A2_PIN D2
#define MOTOR_B1_PIN D5
#define MOTOR_B2_PIN D6
const uint8_t motorPins[] = {MOTOR_A1_PIN, MOTOR_A2_PIN, MOTOR_B1_PIN, MOTOR_B2_PIN};


// Us pins
<<<<<<< Updated upstream
<<<<<<< Updated upstream
#define US1_PIN 6
#define US2_PIN 4
#define US3_PIN 8

=======
#define US1_PIN D8
#define US2_PIN D7
#define US3_PIN D3
>>>>>>> Stashed changes
=======
#define US1_PIN D8
#define US2_PIN D7
#define US3_PIN D3
>>>>>>> Stashed changes
const uint8_t usPins[] = {US1_PIN, US2_PIN, US3_PIN};


#define BUTTON_PIN D4    // Taster S1/S2/S3... werden darüber gelesen


void setup (){

  // Init motor pins as output
  for (size_t i = 0; i < sizeof(motorPins)/sizeof(motorPins[0]); i++) {
    pinMode(motorPins[i], OUTPUT);
    digitalWrite(motorPins[i], LOW);
  }

  // Init serial communication
  Serial.begin(9600);
  while (!Serial); // Optional: wait for serial if using USB-serial converter

  // Init button pin
  pinMode(BUTTON_PIN, INPUT);

}


void loop (){

  if(s1Pushed()){

    float dist1 = 0;
    while (dist1 <= 0.6) {
      dist1 = measureDistance(US2_PIN);
      Serial.println("Distance: " +  String(dist1));
      drive(true, 100, 100);
    }
    
  }
<<<<<<< Updated upstream
<<<<<<< Updated upstream
=======

>>>>>>> Stashed changes
=======

>>>>>>> Stashed changes

}


float measureDistance(uint8_t pin) {
  long duration;
  float distance;

  // Trigger-Puls senden
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
  delayMicroseconds(5);
  digitalWrite(pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(pin, LOW);

  // Pin auf Input stellen, um Echo zu empfangen
  pinMode(pin, INPUT);
  duration = pulseIn(pin, HIGH, 30000);  // Timeout nach 30 ms

  if (duration == 0) {
    return -1.0;  // Kein Hindernis erkannt
  }

  // Umrechnung von Mikrosekunden in Meter
  // Distance = (Speed of Sound × Time) / 2 x 100
  // speed of Sound = 343m/s so in mirco seconds will be 0.0343 then we get at the end (duration(which is time) x 0.0343 / 2 x 100), which equals the formula at the end
  distance = duration * 0.0001715; ;

  return distance;
}

void turn(bool left, uint16_t time, uint16_t speed) {
  setMotor(left, speed, true);   // Motor A
  setMotor(!left, speed, false);  // Motor B

  delay(time);

  // Stop both motors
  setMotor(true, 0, true);
  setMotor(true, 0, false);
}


void drive(bool forward, uint16_t time, uint16_t speed) {
  setMotor(forward, speed, true);   // Motor A
  setMotor(forward, speed, false);  // Motor B

  delay(time);

  // Stop both motors
  setMotor(true, 0, true);
  setMotor(true, 0, false);
}


void setMotor(bool forward, uint16_t speed, bool motorA) {
  int8_t Motor1 = MOTOR_A1_PIN;
  int8_t Motor2 = MOTOR_A2_PIN;
  uint16_t speedReverse = 0;

  if (!motorA){
    Motor1 = MOTOR_B1_PIN;
    Motor2 = MOTOR_B2_PIN;
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

bool s1Pushed() {
  int a1_value = analogRead(BUTTON_PIN);
  String currentButton = "";
  if (a1_value >= 0 && a1_value <= 100) {
    return true;
  } else {
    return false;
  }
}