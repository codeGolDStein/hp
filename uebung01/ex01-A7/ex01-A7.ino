#include <LiquidCrystal.h>

// LCD Connection
#define R_S 8
#define E   9
#define DB4 10
#define DB5 11
#define DB6 12
#define DB7 13

#define NUM_CHAR 20
#define NUM_LINES 4

#define INPUT_PIN A0
#define INPUT_PIN A1

// LED PINS
#define LED_PIN 5

// LCD initialisieren
LiquidCrystal lcd(R_S, E, DB4, DB5, DB6, DB7);

// Merken, was zuletzt angezeigt wurde
String lastButton = "";

unsigned long BLINK_TIME_MS_INTERVAL = 1000;
// set LED Status
bool LED_STATUS = false;
unsigned long previousMILLIS = 0;


//--------------------------------
// Tr1 Frequenz
//unsigned long tr1_value = 0


void setup() {
  lcd.begin(NUM_CHAR, NUM_LINES);
  lcd.setCursor(0, 1);  // Start auf zweiter Zeile
  lcd.print("button: -");

  
}

void loop() {
  unsigned long currentMILLIS = millis();


  // read value of Trimmer
  int a0_value = analogRead(INPUT_PIN);
  // read value of Button
  int a1_value = analogRead(INPUT_PIN);
  
  // chnage Blink time interval
  BLINK_TIME_MS_INTERVAL = (BLINK_TIME_MS_INTERVAL / a0_value);


  // Turn light on and off
  //--------------------------------
  if (currentMILLIS - previousMILLIS >= BLINK_TIME_MS_INTERVAL){
    previousMILLIS = currentMILLIS; // Reset

    LED_STATUS = !LED_STATUS;

    if (LED_STATUS){
      digitalWrite(LED_PIN, HIGH);// Set LED on
    } else {
        digitalWrite(LED_PIN, LOW); //Set LED off
    }

  }
}  
