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
#define REFERENCE_VOLTAGE 3.3

LiquidCrystal lcd(R_S, E, DB4, DB5, DB6, DB7);

void setup() {
  lcd.begin(NUM_CHAR, NUM_LINES);
}

void loop() {
  int a0_value = analogRead(INPUT_PIN);

  float voltage = (float)(a0_value * REFERENCE_VOLTAGE / 1023.0);

  int volts = (int)voltage;
  int dezimal = (int)((voltage - volts) * 100);  

  lcd.setCursor(0, 0);
  lcd.print("Analog 0: ");
  lcd.print(volts);
  lcd.print(",");
  if (dezimal < 10) lcd.print("0"); 
  lcd.print(dezimal);
  lcd.print("V");

  delay(1000);
  lcd.clear();
}