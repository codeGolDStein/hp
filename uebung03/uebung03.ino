// Aufgabe 2
#define OUTPUT_PIN PB4  // Arduino Pin 12 = PB4
#define NOTE_C6 1046
// Aufgabe 3
#define SOUND_PIN PB2   // Pin 10 = OC1B (for sound output)

void setup() {
    // Setze Pin 13 als Ausgang (Data Direction Register B, Bit 5)
    DDRB |= (1 << PB5);
    // Start tone at 1046 Hz
    setTimer1Freq(); 
}

void loop() {
    // Toggle alle 500ms => ergibt 1 Hz (1x an/aus pro Sekunde)
    setPin13(true);
    _delay_ms(500);
    setPin13(false);
    _delay_ms(500);
}

void setPin13(bool high) {
    if (high) {
        PORTB |= (1 << PB5);    // Setze Bit 5 (Pin 13 high)
    } else {
        PORTB &= ~(1 << PB5);   // Lösche Bit 5 (Pin 13 low)
    }
}

void setTimer1Freq() {
    cli(); // Disable global interrupts during setup

    /*
    // Set Pin 12 (PB4) as output
    DDRB |= (1 << OUTPUT_PIN);
    */

    // Set Pin 10 (PB2 = OC1B) as output
    DDRB |= (1 << SOUND_PIN);

    /* Wegen Aufgabe 2
    // Reset Timer1 registers
    TCCR1A = 0;
    TCCR1B = 0;
    TIMSK1 = 0;
    */

    // Reset Timer1 control registers (Aufgabe 3)
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1  = 0;

    // Set CTC mode (WGM12 = 1, others = 0)
    TCCR1B |= (1 << WGM12);

     // Enable hardware toggle on OC1B (Pin 10)
    TCCR1A |= (1 << COM1B0);  // Toggle OC1B on compare match

    // Set compare match value for 1046 Hz
    OCR1A = (F_CPU / (2 * 8 * NOTE_C6)) - 1;

    /*
    // Enable Timer1 Compare A Match interrupt
    TIMSK1 |= (1 << OCIE1A);
    */
    
    // Set prescaler to 8 and start the timer
    TCCR1B |= (1 << CS11);  // CS11 = prescaler 8

    sei(); // Enable global interrupts
}

/* Aufgabe 2
// ISR toggles PB4 (Pin 12)
ISR(TIMER1_COMPA_vect) {
    PINB |= (1 << OUTPUT_PIN); // Toggle pin
}
*/

// Arduino-kompatibles Main für reine C-Umgebung
int main(void) {
    setup();
    while (1) {
        loop();
    }
}
