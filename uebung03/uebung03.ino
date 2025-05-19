#define OUTPUT_PIN PB4 // Arduino Pin 12 = PB4
#define NOTE_C6 1046
// Aufgabe 3
#define SOUND_PIN PB2 // Pin 10 = OC1B (for sound output)

/*Aufgabe 5*/
// Neue globale Variable für den Timer-Zähler
volatile uint32_t tCount = 0;

void setup() {
  // Setze Pin 13 als Ausgang (Data Direction Register B, Bit 5)
  DDRB |= (1 << PB5);
  
  /*Aufgabe 5*/
  // Initialisiere die serielle Schnittstelle mit 38400 Baud
  Serial.begin(38400);
  Serial.println("System gestartet");
  
  // Aktiviere Timer2 für 1ms Interrupt
  setTimer2(true);
  
  // Starte mit einer Frequenz
  setTimer1Freq(1046); // C6
}


/* Aufgabe 5*/
// Letzte Zeit für die Sekunden-Ausgabe
uint32_t lastPrintTime = 0;
// Letzte Zeit für Frequenzwechsel
uint32_t lastFreqChangeTime = 0;
// Aktueller Frequenzindex
uint8_t currentFreqIndex = 0;
// Array mit verschiedenen Frequenzen für die Tonfolge
uint16_t frequencies[] = {1046, 880, 659, 523, 440, 659, 880, 1046};
uint8_t numFrequencies = 8;

void loop() {
  // Blinke LED unabhängig vom Ton
  setPin13((tCount % 1000) < 500);
  
  // Gib einmal pro Sekunde den aktuellen Timer-Zähler aus
  if (tCount - lastPrintTime >= 1000) {
    Serial.print("Timer Counter: ");
    Serial.println(tCount);
    lastPrintTime = tCount;
  }
  
  // Ändere die Frequenz alle 500ms
  if (tCount - lastFreqChangeTime >= 500) {
    currentFreqIndex = (currentFreqIndex + 1) % numFrequencies;
    setTimer1Freq(frequencies[currentFreqIndex]);
    lastFreqChangeTime = tCount;
  }
}

void setPin13(bool high) {
  if (high) {
    PORTB |= (1 << PB5); // Setze Bit 5 (Pin 13 high)
  } else {
    PORTB &= ~(1 << PB5); // Lösche Bit 5 (Pin 13 low)
  }
}

/*Aufgabe 4 und so */
void setTimer1Freq(uint16_t freq) {
  // Frequenzgrenzen prüfen
  if (freq < 100 || freq > 3000) {
    // Frequenz ungültig → Timer stoppen und Pin auf LOW setzen
    TCCR1B = 0;
    PORTB &= ~(1 << SOUND_PIN); // Setze Pin 10 auf LOW
    return;
  }
  
  cli(); // Disable global interrupts during setup
  
  // Set Pin 10 (PB2 = OC1B) as output
  DDRB |= (1 << SOUND_PIN);
  
  // Reset Timer1 control registers (Aufgabe 3)
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  
  // Set CTC mode (WGM12 = 1, others = 0)
  TCCR1B |= (1 << WGM12);
  
  // Enable hardware toggle on OC1B (Pin 10)
  TCCR1A |= (1 << COM1B0); // Toggle OC1B on compare match
  
  // Set compare match value for 1046 Hz
  // Prescaler fest auf 8
  uint16_t ocr = (F_CPU / (2 * 8 * freq)) - 1;
  
  // Prüfen ob Wert in 16-Bit passt
  if (ocr > 65535) {
    // Wert zu groß → Timer deaktivieren
    TCCR1B = 0;
    PORTB &= ~(1 << SOUND_PIN);
    return;
  }
  
  OCR1A = ocr;
  
  // Set prescaler to 8 and start the timer
  TCCR1B |= (1 << CS11); // CS11 = prescaler 8
  
  sei(); // Enable global interrupts
}

/*Aufgabe 5*/
// Neue Funktion zum Konfigurieren von Timer2 für 1ms Interrupts
void setTimer2(bool enable) {
  cli(); // Disable global interrupts during setup
  
  // Reset Timer2 registers
  TCCR2A = 0;
  TCCR2B = 0;
  TCNT2 = 0;
  
  if (enable) {
    // Set CTC mode (WGM21 = 1, WGM20, WGM22 = 0)
    TCCR2A |= (1 << WGM21);
    
    // Bei 16MHz Takt und Prescaler 64:
    // 16MHz / 64 = 250kHz => 250 Ticks für 1ms
    OCR2A = 249;
    
    // Enable Timer2 Compare Match A interrupt
    TIMSK2 |= (1 << OCIE2A);
    
    // Set prescaler to 64 and start timer
    TCCR2B |= (1 << CS22); // CS22 = 1, CS21 = 0, CS20 = 0 für Prescaler 64
  } else {
    // Disable Timer2 Compare Match A interrupt
    TIMSK2 &= ~(1 << OCIE2A);
  }
  
  sei(); // Enable global interrupts
}

/*Aufgabe 5*/
// ISR für Timer2 Compare Match A - wird alle 1ms aufgerufen
ISR(TIMER2_COMPA_vect) {
  tCount++; // Inkrementiere den globalen Zähler
}

// Arduino-kompatibles Main für reine C-Umgebung
int main(void) {
  setup();
  while (1) {
    loop();
  }
}
