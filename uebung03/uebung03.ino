void setup() {
    // Setze Pin 13 als Ausgang (Data Direction Register B, Bit 5)
    DDRB |= (1 << PB5);
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

// Arduino-kompatibles Main für reine C-Umgebung
int main(void) {
    setup();
    while (1) {
        loop();
    }
}