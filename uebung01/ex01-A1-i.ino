// Wait time in ms 1000ms = 1s
#define BLINK_TIME_MS 1000
// set output pin
#define LED_PIN 10

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED_PIN, HIGH);// Set LED on
  delay(BLINK_TIME_MS); // Wait
  digitalWrite(LED_PIN, LOW); //Set LED off
  delay(BLINK_TIME_MS); // Wait
}
