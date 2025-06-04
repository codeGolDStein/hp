#define OUTPUT_PIN PB4 // Arduino Pin 12 = PB4
#define NOTE_C6 1046
// Aufgabe 3
#define SOUND_PIN PB2 // Pin 10 = OC1B (for sound output)

/*Aufgabe 5*/
// Neue globale Variable für den Timer-Zähler
volatile uint32_t tCount = 0;

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

// Aufgabe 6 & 7 – Melodie-Daten
// Erhöhte Array-Größe für RTTTL-Klingeltöne
#define MAX_NOTES 100
uint16_t notes[MAX_NOTES] = { 262, 294, 330, 349, 392, 440, 494, 523, 587, 659 }; 
uint16_t durations[MAX_NOTES] = { 200, 250, 300, 200, 250, 300, 400, 250, 200, 500 };
volatile uint8_t melodyIdx = 0;
uint8_t melodyLen = 10; // Standardwert, wird beim Parsen aktualisiert

// Aufgabe 7 - RTTTL Klingelton
char buffer[] = "Bgirl:d=4,o=5,b=125:8g#,8e,8g#,8c#6,a,p,8f#,8d#,8f#,8b,g#,8f#,8e,p,8e,8c#,f#,c#,p,8f#,8e,g#,f#";


// Basis-Frequenzen für die 4. Oktave
uint16_t baseFrequencies[] = { 262, // C
  277, // C#
  294, // D
  311, // D#
  330, // E
  349, // F
  370, // F#
  392, // G
  415, // G#
  440, // A
  466, // A#
  494  // B/H
};




void setup() {
  // Setze Pin 13 als Ausgang (Data Direction Register B, Bit 5)
  DDRB |= (1 << PB5);
  
  /*Aufgabe 5*/
  // Initialisiere die serielle Schnittstelle mit 38400 Baud
  Serial.begin(115200);
  Serial.println("System gestartet");
  
  // RTTTL parsen
  Serial.println("Starte RTTTL-Parser");
  parseRTTTL(buffer);
  Serial.println("RTTTL-Parser beendet");
  
  // Melody abspielen
  //playMelody();
}

void loop() {
  
  if (Serial.available() > 0) {
    char cmd = Serial.read();
    if (cmd == 'S') {
      playMelody(); // Trigger sound on 'S' command
    }
  }
  
  // Gib einmal pro Sekunde den aktuellen Timer-Zähler aus
  if (tCount - lastPrintTime >= 1000) {
    Serial.print("Timer Counter: ");
    Serial.println(tCount);
    Serial.print("Note: ");
    Serial.print(melodyIdx);
    if (melodyIdx < melodyLen) {
      Serial.print(" (");
      Serial.print(notes[melodyIdx]);
      Serial.println(" Hz)");
    } else {
      Serial.println(" (Melodie beendet)");
    }
    lastPrintTime = tCount;
  }
}




// Hilfsfunktionen für RTTTL-Parsing
bool charIsDigit(char c) {
  return (c >= '0' && c <= '9');
}

uint16_t str2uint(const char* str, uint8_t* idx) {
  uint16_t val = 0;
  
  while (charIsDigit(str[*idx])) {
    val = val * 10 + (str[*idx] - '0');
    (*idx)++;
  }
  
  return val;
}

// RTTTL-Parser
void parseRTTTL(const char* rtttl) {
  uint8_t idx = 0;
  uint8_t noteIndex = 0;
  
  // Standardwerte festlegen
  uint8_t defaultDuration = 4;
  uint8_t defaultOctave = 6;
  uint16_t bpm = 63;
  
  // Name überspringen (bis zum ersten ':')
  while (rtttl[idx] != ':' && rtttl[idx] != '\0') {
    idx++;
  }
  
  // Überspringen des ':'
  if (rtttl[idx] == ':') {
    idx++;
  } else {
    // Ungültiges Format
    Serial.println("Fehler: RTTTL ohne ':'");
    return;
  }
  
  // Standardparameter parsen
  bool parsingParams = true;
  
  while (parsingParams && rtttl[idx] != '\0') {
    // Whitespace überspringen
    while (rtttl[idx] == ' ') idx++;
    
    if (rtttl[idx] == 'd' && rtttl[idx+1] == '=') {
      idx += 2;
      defaultDuration = str2uint(rtttl, &idx);
      Serial.print("Default Duration: ");
      Serial.println(defaultDuration);
    } else if (rtttl[idx] == 'o' && rtttl[idx+1] == '=') {
      idx += 2;
      defaultOctave = str2uint(rtttl, &idx);
      Serial.print("Default Octave: ");
      Serial.println(defaultOctave);
    } else if (rtttl[idx] == 'b' && rtttl[idx+1] == '=') {
      idx += 2;
      bpm = str2uint(rtttl, &idx);
      Serial.print("BPM: ");
      Serial.println(bpm);
    }
    
    // Zum nächsten Parameter oder Ende der Parameter gehen
    if (rtttl[idx] == ',') {
      idx++;
    } else if (rtttl[idx] == ':') {
      idx++;
      parsingParams = false;
    }
  }
  
  // Berechnen der Basisdauer einer Viertelnote in ms
  uint16_t quarterNoteDuration = 60 * 1000 / bpm;
  Serial.print("Quarter note duration (ms): ");
  Serial.println(quarterNoteDuration);
  
  // Parsen der einzelnen Noten
  while (rtttl[idx] != '\0' && noteIndex < MAX_NOTES) {
    // Whitespace überspringen
    while (rtttl[idx] == ' ') idx++;
    
    // Duration parsen (optional)
    uint8_t duration = defaultDuration;
    if (charIsDigit(rtttl[idx])) {
      duration = str2uint(rtttl, &idx);
    }
    
    // Note parsen
    if (rtttl[idx] == '\0') break;
    
    uint8_t note = 0;
    bool isPause = false;
    
    switch (rtttl[idx]) {
      case 'c': note = 0; break;
      case 'd': note = 2; break;
      case 'e': note = 4; break;
      case 'f': note = 5; break;
      case 'g': note = 7; break;
      case 'a': note = 9; break;
      case 'b': case 'h': note = 11; break;
      case 'p': isPause = true; break;
      default: 
        Serial.print("Ungültige Note: ");
        Serial.println(rtttl[idx]);
        break;
    }
    idx++;
    
    // Prüfen auf # (Erhöhung um einen Halbton)
    if (rtttl[idx] == '#') {
      note++;
      idx++;
    }
    
    // Oktave parsen (optional)
    uint8_t octave = defaultOctave;
    if (charIsDigit(rtttl[idx])) {
      octave = rtttl[idx] - '0';
      idx++;
    }
    
    // Prüfen auf punktierte Note
    bool dotted = false;
    if (rtttl[idx] == '.') {
      dotted = true;
      idx++;
    }
    
    // Frequenz berechnen
    uint16_t frequency = 0;
    if (!isPause) {
      // Basisfrequenz aus der 4. Oktave holen
      uint16_t baseFreq = baseFrequencies[note];
      
      // Anpassung an die gewünschte Oktave
      // f(Noct) = f(N0) * 2^oct => f(N4) * 2^(oct-4)
      frequency = baseFreq;
      for (uint8_t i = 4; i < octave; i++) {
        frequency *= 2;
      }
    }
    
    // Dauer der Note in ms berechnen
    uint16_t noteDuration = (4 * quarterNoteDuration) / duration;
    
    // Punktierte Note behandeln (Verlängerung um die Hälfte)
    if (dotted) {
      noteDuration = noteDuration + (noteDuration / 2);
    }
    
    // Werte in die Arrays speichern
    notes[noteIndex] = frequency;
    durations[noteIndex] = noteDuration;
    
    // Debug-Ausgabe
    Serial.print("Note ");
    Serial.print(noteIndex);
    Serial.print(": Freq=");
    Serial.print(frequency);
    Serial.print(" Hz, Dur=");
    Serial.print(noteDuration);
    Serial.println(" ms");
    
    noteIndex++;
    
    // Zum nächsten Komma oder Ende gehen
    if (rtttl[idx] == ',') {
      idx++;
    } else if (rtttl[idx] == '\0' || noteIndex >= MAX_NOTES) {
      break;
    }
  }
  
  // Gesamtlänge der Melodie speichern
  melodyLen = noteIndex;
  Serial.print("Melodie mit ");
  Serial.print(melodyLen);
  Serial.println(" Noten geparst");
}

/* Aufgabe 1 */
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
    // Frequenz ungültig -> Timer stoppen und Pin auf LOW setzen
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
  TCCR1A |= (1 << COM1B0);
  
  // Set compare match value for freq Hz
  // Prescaler fest auf 8
  uint16_t ocr = (F_CPU / (2 * 8 * freq)) - 1;
  
  // Prüfen ob Wert in 16-Bit passt
  if (ocr > 65535) {
    // Wert zu groß -> deaktivieren
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

void playMelody() {
  melodyIdx = 0;
  tCount = 0;
  setTimer1Freq(notes[melodyIdx]);
  setTimer2(true);  // 1ms-Timer aktivieren
  Serial.println("Melodie gestartet");
}

/*Aufgabe 5*/
// ISR für Timer2 Compare Match A - wird alle 1ms aufgerufen
ISR(TIMER2_COMPA_vect) {
  tCount++; //globaler Zähler

  if (tCount >= durations[melodyIdx]) {
    tCount = 0;
    melodyIdx++;

    if (melodyIdx < melodyLen) {
      setTimer1Freq(notes[melodyIdx]);
    } else {
      setTimer2(false);  // Alle Töne gespielt → Timer2 aus
      setTimer1Freq(0);  // Ton stoppen
      Serial.println("Melodie beendet");
    }
  }
}

int main(void) {
  setup();
  while (1) {
    loop();
  }
}
