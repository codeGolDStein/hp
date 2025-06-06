// Aufgabe 1 ii) Da millis() die Milisekunden nach einem Start oder Reset zählt lässt sich damit präzieser und variabler einstellen lässt.
// kann in der zwischen Zeit unser code weiter laufen. Bei delay() haltet der gesamte code bis der delay fertig ist.

//Aufgabe 1 i)
// Wait time in ms 1000ms = 1s
#define BLINK_TIME_MS 1000
// set output pin
#define LED_PIN 10
// Wait time in ms 1000ms = 1s
unsigned long BLINK_TIME_MS_INTERVAL = 1000;
// previous time 
unsigned long previousMILLIS = 0;
// set LED Status
bool LED_STATUS = false;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_PIN, OUTPUT);
}

void loop() {

  /*
  // Aufgabe 1 i)
  // put your main code here, to run repeatedly:
  digitalWrite(LED_PIN, HIGH);// Set LED on
  delay(BLINK_TIME_MS); // Wait
  digitalWrite(LED_PIN, LOW); //Set LED off
  delay(BLINK_TIME_MS); // Wait
  */

  unsigned long currentMILLIS = millis();

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