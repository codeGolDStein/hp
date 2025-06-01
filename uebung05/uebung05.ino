// Pins of motor
#define PIN_MOTOR_A1 5
#define PIN_MOTOR_A2 6
#define PIN_MOTOR_B1 9
#define PIN_MOTOR_B2 10

const uint8_t motorPins[] = { PIN_MOTOR_A1, PIN_MOTOR_A2, PIN_MOTOR_B1, PIN_MOTOR_B2 };


// Us pins
#define US1_PIN 6
#define US2_PIN 7
#define US3_PIN 8

const uint8_t usPins[] = {US1_PIN, US2_PIN, US3_PIN};


#define BUTTON_PIN A1    // Taster S1/S2/S3... werden darüber gelesen


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

  // Measure and print distances
  float d1 = measureDistance(US1_PIN);
  float d2 = measureDistance(US2_PIN);
  float d3 = measureDistance(US3_PIN);

  Serial.print("US1: ");
  Serial.print(d1);
  Serial.print(" m, ");

  Serial.print("US2: ");
  Serial.print(d2);
  Serial.print(" m, ");

  Serial.print("US3: ");
  Serial.print(d3);
  Serial.println(" m");

  delay(500); // Update every 500 ms

  /*
  if(s1Pushed()){

    // Fahre zur ersten Wand
    while (measureDistance(US2_PIN) <= 0.6) {
      drive(true, 100, 100);
    }
    drive(true, 0, 0);
  }
  */

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
  distance = duration * 0.0001715; 

  // if(pin == 0){
  //   Serial.println(String(pin) + " Duration: " +  String(duration));
  // }

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
  int8_t Motor1 = PIN_MOTOR_A1;
  int8_t Motor2 = PIN_MOTOR_A2;
  uint16_t speedReverse = 0;

  if (!motorA){
    Motor1 = PIN_MOTOR_B1;
    Motor2 = PIN_MOTOR_B2;
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