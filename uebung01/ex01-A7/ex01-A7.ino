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

#define BUTTON_PIN A1       // Taster S1 wird darüber gelesen
#define TRIMMER_PIN A0      // Trimmer Tr1 für Frequenz

// LED Pins (LED1 bis LED6 an Pins 2–7)
const int LED_PINS[] = {2, 3, 4, 5, 6, 7};
const int NUM_LEDS = 6;

// LCD initialisieren
LiquidCrystal lcd(R_S, E, DB4, DB5, DB6, DB7);

// Farbnamen
const String COLORS[] = {"GREEN", "YELLOW", "RED"};
int colorIndex = 0;

// Entprellen
bool lastButtonState = false;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 200;

// Blinken
unsigned long lastBlinkTime = 0;
bool ledState = false;
int blinkDelay = 500;

void setup() {
  lcd.begin(NUM_CHAR, NUM_LINES);

  // LEDs als Ausgang
  for (int i = 0; i < NUM_LEDS; i++) {
    pinMode(LED_PINS[i], OUTPUT);
    digitalWrite(LED_PINS[i], LOW);
  }
}

void loop() {
  // --- Frequenz einlesen (Trimmer) ---
  int trimmerValue = analogRead(TRIMMER_PIN);
  int frequency = map(trimmerValue, 0, 1023, 1, 50);  // 1–50 Hz
  blinkDelay = 1000 / frequency / 2;  // für Ein/Aus-Phasen

  // --- Taster einlesen ---
  int a1_value = analogRead(BUTTON_PIN);
  bool buttonPressed = (a1_value >= 0 && a1_value <= 100);

  if (buttonPressed && !lastButtonState && (millis() - lastDebounceTime > debounceDelay)) {
    // Farbwechsel
    colorIndex = (colorIndex + 1) % 3;
    lastDebounceTime = millis();
  }
  lastButtonState = buttonPressed;

  // --- LEDs je nach Farbe steuern ---
  unsigned long currentMillis = millis();
  if (currentMillis - lastBlinkTime >= blinkDelay) {
    lastBlinkTime = currentMillis;
    ledState = !ledState;

    // LED-Muster je nach Farbe
    for (int i = 0; i < NUM_LEDS; i++) {
      if (ledState && isLedActiveForColor(i, colorIndex)) {
        digitalWrite(LED_PINS[i], HIGH);
      } else {
        digitalWrite(LED_PINS[i], LOW);
      }
    }
  }

  // --- LCD Zeile 3 + 4 aktualisieren ---
  lcd.setCursor(0, 2);  // Zeile 3
  lcd.print("Color: " + COLORS[colorIndex] + "       ");

  lcd.setCursor(0, 3);  // Zeile 4
  lcd.print("Freq: " + String(frequency) + " Hz     ");
}

// Gibt zurück, ob LED[i] für die Farbe aktiv ist
bool isLedActiveForColor(int ledIndex, int colorIdx) {
  // Beispiel: 2 LEDs pro Farbe
  if (colorIdx == 0) return (ledIndex == 0 || ledIndex == 1); // RED
  if (colorIdx == 1) return (ledIndex == 2 || ledIndex == 3); // GREEN
  if (colorIdx == 2) return (ledIndex == 4 || ledIndex == 5); // BLUE
  return false;
}
