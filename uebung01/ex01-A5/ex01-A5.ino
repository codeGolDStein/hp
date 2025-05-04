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

#define INPUT_PIN A1

// LCD initialisieren
LiquidCrystal lcd(R_S, E, DB4, DB5, DB6, DB7);

// Merken, was zuletzt angezeigt wurde
String lastButton = "";

void setup() {
  lcd.begin(NUM_CHAR, NUM_LINES);
  lcd.setCursor(0, 1);  // Start auf zweiter Zeile
  lcd.print("button: -");
}

void loop() {
  int a1_value = analogRead(INPUT_PIN);
  String currentButton;

  if (a1_value >= 0 && a1_value <= 100) {
    currentButton = "S1";
  } else if (a1_value >= 101 && a1_value <= 200) {
    currentButton = "S2";
  } else if (a1_value >= 201 && a1_value <= 300) {
    currentButton = "S3";
  } else if (a1_value >= 301 && a1_value <= 400) {
    currentButton = "S4";
  } else if (a1_value >= 401 && a1_value <= 500) {
    currentButton = "S5";
  } else {
    currentButton = "-";
  }

  // Nur neu anzeigen, wenn sich der Status ändert
  if (currentButton != lastButton) {
    lcd.setCursor(0, 1);  // Zweite Zeile
    lcd.print("button: " + currentButton + "       ");
    lastButton = currentButton;
  }

  delay(200);
}