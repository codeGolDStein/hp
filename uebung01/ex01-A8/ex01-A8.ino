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

#define BUTTON_PIN A1       // Taster S1/S2/S3... werden darüber gelesen
#define TRIMMER_PIN A0      // Trimmer Tr1 für Frequenz

const int LED_PINS[] = {2, 3, 4, 5, 6, 7};
const int NUM_LEDS = 6;

LiquidCrystal lcd(R_S, E, DB4, DB5, DB6, DB7);

const String COLORS[] = {"GREEN", "YELLOW", "RED"};
int colorIndex = 0;

// Entprellen
bool lastButtonStateS1 = false;
bool lastButtonStateS2 = false;
unsigned long lastDebounceTimeS1 = 0;
unsigned long lastDebounceTimeS2 = 0;
const unsigned long debounceDelay = 200;

// Blinken
unsigned long lastBlinkTime = 0;
bool ledState = false;
int blinkDelay = 500;  

// Lauflicht
bool lauflichtMode = false;
int lauflichtIndex = 0;
int lauflichtDirection = 1; 

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
  int frequency = mapToFrequency(trimmerValue);
  blinkDelay = 1000 / frequency / 2;  // für Ein/Aus-Phasen

  // --- Taster einlesen (S1/S2) ---
  int a1_value = analogRead(BUTTON_PIN);
  String currentButton = "";
  if (a1_value >= 0 && a1_value <= 100) {
    currentButton = "S1";
  } else if (a1_value >= 101 && a1_value <= 200) {
    currentButton = "S2";
  }

  // --- S1: Farbwechsel ---
  bool buttonPressedS1 = (currentButton == "S1");
  if (buttonPressedS1 && !lastButtonStateS1 && (millis() - lastDebounceTimeS1 > debounceDelay)) {
    colorIndex = (colorIndex + 1) % 3;
    lastDebounceTimeS1 = millis();
  }
  lastButtonStateS1 = buttonPressedS1;

  // --- S2: Moduswechsel (Blinken <-> Lauflicht) ---
  bool buttonPressedS2 = (currentButton == "S2");
  if (buttonPressedS2 && !lastButtonStateS2 && (millis() - lastDebounceTimeS2 > debounceDelay)) {
    lauflichtMode = !lauflichtMode; // Umschalten zwischen Blinken und Lauflicht
    lastDebounceTimeS2 = millis();
    // LEDs zurücksetzen
    for (int i = 0; i < NUM_LEDS; i++) digitalWrite(LED_PINS[i], LOW);
    lauflichtIndex = 0;
    lauflichtDirection = 1;
  }
  lastButtonStateS2 = buttonPressedS2;

  unsigned long currentMillis = millis();

  if (!lauflichtMode) {
    if (currentMillis - lastBlinkTime >= blinkDelay) {
      lastBlinkTime = currentMillis;
      ledState = !ledState;
      for (int i = 0; i < NUM_LEDS; i++) {
        if (ledState && isLedActiveForColor(i, colorIndex)) {
          digitalWrite(LED_PINS[i], HIGH);
        } else {
          digitalWrite(LED_PINS[i], LOW);
        }
      }
    }
  } else {
    // --- Lauflicht mit Bounce ---
    if (currentMillis - lastBlinkTime >= blinkDelay) {
      lastBlinkTime = currentMillis;
      // Alle LEDs aus
      for (int i = 0; i < NUM_LEDS; i++) digitalWrite(LED_PINS[i], LOW);
      // Nur aktuelle LED an
      digitalWrite(LED_PINS[lauflichtIndex], HIGH);

      // Bounce-Logik
      lauflichtIndex += lauflichtDirection;
      if (lauflichtIndex == NUM_LEDS - 1) lauflichtDirection = -1; // Am Ende: Richtung wechseln
      if (lauflichtIndex == 0) lauflichtDirection = 1;             // Am Anfang: Richtung wechseln
    }
  }

  // --- LCD Zeile 3 + 4 aktualisieren ---
  lcd.setCursor(0, 2);  
  if (!lauflichtMode) {
    lcd.print("Color: " + COLORS[colorIndex] + "       ");
  } else {
    lcd.print("Mode: Lauflicht         ");
  }
  lcd.setCursor(0, 3);  
  lcd.print("Freq: " + String(frequency) + " Hz     ");
}

// Gibt zurück, ob LED[i] für die Farbe aktiv ist
bool isLedActiveForColor(int ledIndex, int colorIdx) {

  if (colorIdx == 0) return (ledIndex == 0 || ledIndex == 1); // GREEN
  if (colorIdx == 1) return (ledIndex == 2 || ledIndex == 3); // YELLOW
  if (colorIdx == 2) return (ledIndex == 4 || ledIndex == 5); // RED
  return false;
}

int mapToFrequency(int trimmerValue) {
    if (trimmerValue < 0) trimmerValue = 0;
    if (trimmerValue > 660) trimmerValue = 660;
    double frequency = (49.0 / 660.0) * trimmerValue + 1.0;
    return static_cast<int>(frequency + 0.5); 
}
