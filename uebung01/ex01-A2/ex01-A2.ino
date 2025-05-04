// Include LCD functions:
#include <LiquidCrystal.h>

// LCD Connection
#define R_S 8
#define E   9
#define DB4 10
#define DB5 11
#define DB6 12
#define DB7 13

// LCD Defines
#define NUM_CHAR 20
#define NUM_LINES 4

#define INPUT_PIN A0

// Global variables
// Define the LCD screen
LiquidCrystal lcd(R_S, E, DB4, DB5, DB6, DB7);

void setup() {
  // LCD has 4 lines with 20 chars
  lcd.begin(NUM_CHAR, NUM_LINES);
}


void loop() {
  // Set cursor to arbitrary position
  int a0_value = analogRead(INPUT_PIN);
 
  lcd.setCursor(0, 0);

  // Print statements
  lcd.print("Analog 0: " + String(a0_value));

  // Prevent display flickering for too fast updates
  delay(1000);
  // Clear the display.
  lcd.clear();
  // NOTE: If you update only parts of the screen, don't use lcd.clear.
  // Set the cursor to the line and column to be updated and override existing chars.
}
