// Pinbelegung
const int A1 = D1;  // GPIO5
const int A2 = D2;  // GPIO4

void setup() {
  // Pins als Ausgang definieren
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  
  // Optional: Motor am Anfang stoppen
  digitalWrite(A1, LOW);
  digitalWrite(A2, LOW);
}

// Funktion zum Steuern des Motors
void setMotor(bool forward) {
  if (forward) {
    digitalWrite(A1, LOW);   // A1 = 0
    digitalWrite(A2, HIGH);  // A2 = 1
  } else {
    digitalWrite(A1, HIGH);  // A1 = 1
    digitalWrite(A2, LOW);   // A2 = 0
  }
}

void loop() {
  // Beispiel: Vorwärts für 2 Sekunden
  setMotor(true);
  delay(2000);

  // Rückwärts für 2 Sekunden
  setMotor(false);
  delay(2000);
}
