  // Include WiFi libs
  #include <ESP8266WiFi.h>
  #include <ESP8266mDNS.h>
  #include "website.h"

  // Add your wifi credentials here
  const char* ssid     = "Omar";
  const char* password = "omarpassword";


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
  int step = 0;
  bool run = true;

  // distances of US
  float d1 = 0;
  float d2 = 0;
  float d3 = 0;

  // connecting to arduino


  // In setup() after Serial.begin()
  void setup() {
    // Init serial
    //Serial.begin(115200);
    Serial.begin(9600); // Match baud rate with Arduino

    // Init motor pins as output
    for (size_t i = 0; i < sizeof(motorPins)/sizeof(motorPins[0]); i++) {
      pinMode(motorPins[i], OUTPUT);
      digitalWrite(motorPins[i], LOW);
    }

    // Comment if you want that the ESP creates an AP
    // Connect to wifi
    //Serial.print("Connecting to ");
    //Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      //Serial.print(".");
    }
    // Die IP vom Webserver auf dem seriellen Monitor ausgeben
    /*
    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP Adress: ");
    Serial.println(WiFi.localIP());
    
    // You can remove the password parameter if you want the AP to be open.
    /*
    WiFi.softAP(ssid, password);
    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(myIP);
    */

    // mDNS name resolving
    if (MDNS.begin(DEVICE_NAME)) {
      //Serial.println("MDNS started");
    } else {
      //Serial.println("Error starting MDNS");
    }

    // Start webserver
    server.begin();
    
  }


  void loop() {
    // Handle clients
    handleClient();
    // Update MDNS
    MDNS.update();

    //d1 = readUS(US1_PIN);  // Left sensor
    d2 = readUS(US2_PIN);  // Center sensor
    //d3 = readUS(US3_PIN);  // Right sensor

    
    /*
    Serial.print("Center distance (d2): ");
    Serial.print(d2);
    Serial.println(" cm");
    */


    // Runs when Teslamode is activated thgough webite 
    if (teslaMode) {
      // drive parkout  
      doTask(d2);
      }
  }


  // Function for robot to follow given tasks, text distance and turn if needed. 
  void doTask(float d2) {
    // if finished quit
    if (!run) {
      return;
    }
    
    switch(step) {
      case 0:
        // Step 1: Drive straight until distance is 60cm --> then turn right
        if (d2 > 60) {
          drive(true, 150, 200);
        }
        else if (d2 > 0 && d2 <= 60) { // ~60cm threshold (60 * 89.4 ≈ 535)
          Serial.write('S');
          delay(1000);
          turn(true, 465, 150); // Turn right for 500ms at speed 150
          step = 1; // Move to next step
        }
        break;
      case 1:
        // Step 2: Drive straight until distance is 60cm --> then turn Left
        if (d2 > 60) {
          drive(true, 100, 200);
        }
        else if (d2 > 0 && d2 <= 60) { 
          Serial.write('S');
          delay(1000);
          turn(false, 465, 150); // Turn left for 500ms at speed 150
          step = 2;
        }
        break;
      case 2:
      // Step 3: Drive straight until distance is 60cm --> then turn Left
        if (d2 > 60) {
          drive(true, 100, 200);
        }
        else if (d2 > 0 && d2 <= 60) { 
          Serial.write('S');
          delay(1000);
          turn(false, 465, 150); // Turn left for 500ms at speed 150
          step = 3;
        }
        break;
      case 3:
        // drive until distance is 60cm and stop.
        if (d2 > 60) {
          drive(true, 100, 200);
        }
        else if (d2 > 0 && d2 <= 60) { // ~60cm threshold
          Serial.write('S');
          delay(1000);
          // Final turn left
          turn(false, 465, 0);
          run = false; // parkour complete
          step = 0;
          teslaMode = false;
        }
        break;
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
      float us1 = d1;
      float us2 = d2;
      float us3 = d3;

      // Send US data to website
      client.printf("{\"US1\":%.2f, \"US2\":%.2f, \"US3\":%.2f}", us1, us2, us3);
      
    } 
    
    if (request.indexOf("GET /tesla") >= 0) {
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

  float readUS(int pin) {
    static const unsigned long TRIGGER_DELAY = 2;
    static const unsigned long TIMEOUT_MICROSECONDS = 30000;
    static const float DISTANCE_MULTIPLIER = 0.017241; // ca 1/58
    
    //trigger pulse
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
    delayMicroseconds(TRIGGER_DELAY);
    digitalWrite(pin, HIGH);
    delayMicroseconds(TRIGGER_DELAY);
    digitalWrite(pin, LOW);
    
    pinMode(pin, INPUT);
    
    // Wait for echo signal start
    while (!digitalRead(pin));
    
    unsigned long echoStart = micros();
    unsigned long elapsed;
    
    // calc distance 
    while ((elapsed = micros() - echoStart) < TIMEOUT_MICROSECONDS) {
      if (!digitalRead(pin)) {
        return elapsed * DISTANCE_MULTIPLIER;
      }
    }
    
    return -1.0; //error
  }

  void drive(bool left, uint16_t time, uint16_t speed) {
    left = !left;

    setMotor(left, speed, true);   // Motor A
    setMotor(!left, speed, false);  // Motor B

    delay(time);

    // Stop both motors
    setMotor(true, 0, true);
    setMotor(true, 0, false);
  }


  void turn(bool forward, uint16_t time, uint16_t speed) {
    forward = !forward;

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
