// Include WiFi libs
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include "website.h"

// Add your wifi credentials here
const char* ssid     = "iPhone Alex";
const char* password = "hpistgeil";


// Webserver on port 80 (standard http port)
WiFiServer server(80);


// Variable to store incoiming http request
String request;


// Name of the device (can be used as DNS query in browser)
#define DEVICE_NAME "HWPRobo"


// Pins of motor
#define MOTOR_A1_PIN D1
#define MOTOR_A2_PIN D2
#define MOTOR_B1_PIN D5
#define MOTOR_B2_PIN D6
const uint8_t motorPins[] = {MOTOR_A1_PIN, MOTOR_A2_PIN, MOTOR_B1_PIN, MOTOR_B2_PIN};


// Us pins
#define US1_PIN D8
#define US2_PIN D7
#define US3_PIN D3
const uint8_t usPins[] = {US1_PIN, US2_PIN, US3_PIN};


bool teslaMode = false;


void setup() {
  // Init serial
  Serial.begin(115200);

  // Init motor pins as output
  for (size_t i = 0; i < sizeof(motorPins)/sizeof(motorPins[0]); i++) {
    pinMode(motorPins[i], OUTPUT);
    digitalWrite(motorPins[i], LOW);
  }
  
  // Comment if you want that the ESP creates an AP
  // Connect to wifi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Die IP vom Webserver auf dem seriellen Monitor ausgeben
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP Adress: ");
  Serial.println(WiFi.localIP());

  // Uncomment if you want that the ESP creates an AP
  
  // You can remove the password parameter if you want the AP to be open.
  /*
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  */

  // mDNS name resolving
  if (MDNS.begin(DEVICE_NAME)) {
    Serial.println("MDNS started");
  } else {
    Serial.println("Error starting MDNS");
  }

  // Start webserver
  server.begin();
}


void loop() {
  // Handle clients
  handleClient();
  // Update MDNS
  MDNS.update();

if (teslaMode) {
    float d1 = measureDistance(US1_PIN);  // Left sensor
    float d2 = measureDistance(US2_PIN);  // Center sensor
    float d3 = measureDistance(US3_PIN);  // Right sensor

    // Check if any sensor sees an obstacle within 30 cm
    if ((d1 > 0 && d1 < 0.30) || 
        (d2 > 0 && d2 < 0.30) || 
        (d3 > 0 && d3 < 0.30)) {

      turn(false, 400, 80);     // Turn right
      delay(300);
      drive(true, 200, 80);     // Move forward
    } else {
      drive(true, 200, 80);     // Clear path, go forward
    }
  } else {
    drive(true, 200, 0);        // Stop mode
  }
}


void handleClient() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client)  {  
    return;  
  }
  // Read the first line of the request (we just need this line)
  request = client.readStringUntil('\r');

  // Print request to serial
  Serial.print("request: ");
  Serial.println(request); 

  // print header message
  client.println(header);
  // Check for corresponding get message  
  if (request.indexOf("GET /pollUS") >= 0) {
    // Serial.println("Polling");
    float us1 = measureDistance(US1_PIN);
    float us2 = measureDistance(US2_PIN);
    float us3 = measureDistance(US3_PIN);

    // Send US data to website
    client.printf("{\"US1\":%.2f, \"US2\":%.2f, \"US3\":%.2f}", us1, us2, us3);
    
  } 
  
  // Insert code to make the d-pad control working
  // Start by pressing the buttons of the d pad and watch the serial console to see how the get requests look.

  else if (request.indexOf("GET /up") >= 0) {

    drive(true, 300, 512);
  }
  else if (request.indexOf("GET /back") >= 0) {

    drive(false, 300, 512);
  }
  else if (request.indexOf("GET /left") >= 0) {

    turn(false, 300, 512);
  }
  else if (request.indexOf("GET /right") >= 0) {

    turn(true, 300, 512);
  }

  else if (request.indexOf("GET /tesla") >= 0) {
  teslaMode = !teslaMode;
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/plain");
  client.println();
  client.println(teslaMode ? "Tesla ON" : "Tesla OFF");

  // Serve initial Website
  } else {
    // Finish HTTP-Request with a newline (thats cruical)
    client.flush();
    client.println(page);
    client.println();
  }
}


float measureDistance(uint8_t pin) {
  long duration;
  float   distance;

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