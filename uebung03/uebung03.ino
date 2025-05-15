#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>

#define PIN13_MASK (1 << 5)  // Bit 5 entspricht Pin 13 (PORTB5)

void setup() {
    // Setze Pin 13 als Ausgang (Data Direction Register B, Bit 5)
    DDRB |= PIN13_MASK;
}

void loop() {
    // Toggle alle 500ms => ergibt 1 Hz (1x an/aus pro Sekunde)
    setPin13(true);
    _delay_ms(500);
    setPin13(false);
    _delay_ms(500);
}

// Funktion zum Setzen oder Löschen von Pin 13
void setPin13(bool high) {
    if (high) {
        PORTB |= PIN13_MASK;    // Setze Bit 5 (LED an)
    } else {
        PORTB &= ~PIN13_MASK;   // Lösche Bit 5 (LED aus)
    }
}

int main(void) {
    setup();
    while (1) {
        loop();
    }
}
