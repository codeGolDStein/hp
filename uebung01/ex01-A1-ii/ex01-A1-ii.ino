// Wait time in ms 1000ms = 1s
unsigned long BLINK_TIME_MS_INTERVAL = 1000;
// previous time 
unsigned long previousMILLIS = 0;
// set output pin
#define LED_PIN 10


void setup() {
  // put your setup code here, to run once:
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  unsigned long currentMILLIS = millis();

  if (currentMILLIS - previousMILLIS >= BLINK_TIME_MS_INTERVAL){
    previousMILLIS = currentMILLIS; // Reset
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    //Serial.print(digitalRead(LED_PIN))
  }
}

// Aufgabe 1 ii) Da millis() die Milisekunden nach einem Start oder Reset zählt lässt sich damit präzieser und variabler einstellen lässt.
// kann in der zwischen Zeit unser code weiter laufen. Bei delay() haltet der gesamte code bis der delay fertig ist.
