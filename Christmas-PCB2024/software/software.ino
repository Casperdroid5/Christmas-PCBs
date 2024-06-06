#include <avr/io.h>
#include <util/delay.h>

// Define frequencies for the notes
#define C 2100
#define D 1870
#define E 1670
#define f 1580
#define G 1400
#define R 0 // Rest

// Define pin assignments
#define SPEAKER_PIN PB0
#define BUTTON_PIN PB1
#define LED_PIN PB2

// Simplified melody to reduce memory usage
const uint16_t melody[] = { E, E, E, R, E, E, E, R, E, G, C, D, E, R };
const uint8_t melody_length = sizeof(melody) / sizeof(melody[0]);

// WS2812 LED control function
void sendWS2812(uint8_t r, uint8_t g, uint8_t b) {
    for(uint8_t i = 0; i < 8; i++) {
        if(g & (1 << (7 - i))) PORTB |= (1 << LED_PIN);
        else PORTB &= ~(1 << LED_PIN);
        _delay_us(0.9);
        PORTB &= ~(1 << LED_PIN);
        _delay_us(0.35);
    }

    for(uint8_t i = 0; i < 8; i++) {
        if(r & (1 << (7 - i))) PORTB |= (1 << LED_PIN);
        else PORTB &= ~(1 << LED_PIN);
        _delay_us(0.9);
        PORTB &= ~(1 << LED_PIN);
        _delay_us(0.35);
    }

    for(uint8_t i = 0; i < 8; i++) {
        if(b & (1 << (7 - i))) PORTB |= (1 << LED_PIN);
        else PORTB &= ~(1 << LED_PIN);
        _delay_us(0.9);
        PORTB &= ~(1 << LED_PIN);
        _delay_us(0.35);
    }
}

// Function to play a tone
void playTone(uint16_t tone, uint16_t duration) {
    if (tone > 0) {
        uint16_t elapsed_time = 0;
        while (elapsed_time < duration) {
            PORTB |= (1 << SPEAKER_PIN);
            for (uint16_t i = 0; i < tone / 2; i++) _delay_us(1);
            PORTB &= ~(1 << SPEAKER_PIN);
            for (uint16_t i = 0; i < tone / 2; i++) _delay_us(1);
            elapsed_time += tone;
        }
    } else {
        _delay_ms(duration / 1000);
    }
}

// Function to play the melody
void playMelody() {
    for (uint8_t i = 0; i < melody_length; i++) {
        uint16_t duration = 500000 / 60; // Beat duration
        playTone(melody[i], duration);
        _delay_ms(100); // Pause between notes
    }
}

void setup() {
    DDRB |= (1 << SPEAKER_PIN) | (1 << LED_PIN); // Set SPEAKER_PIN and LED_PIN as output
    PORTB &= ~(1 << BUTTON_PIN); // Set BUTTON_PIN as input with pull-up
}

void loop() {
    static uint8_t color = 0;
    static uint8_t buttonPressed = 0;

    // Change LED colors
    switch (color) {
        case 0: sendWS2812(255, 0, 0); break; // Red
        case 1: sendWS2812(0, 255, 0); break; // Green
        case 2: sendWS2812(0, 0, 255); break; // Blue
    }
    color = (color + 1) % 3;
    _delay_ms(1000); // Delay between color changes

    // Check if button is pressed
    if (!(PINB & (1 << BUTTON_PIN))) {
        if (!buttonPressed) {
            playMelody();
            buttonPressed = 1;
        }
    } else {
        buttonPressed = 0;
    }
}

int main() {
    setup();
    while (1) {
        loop();
    }
    return 0;
}
