// Include necessary libraries
#include <Adafruit_NeoPixel.h>
#include <avr/power.h>
#include "pitches.h"

// Pin definitions
#define PIN PB1       // NeoPixel pin
#define melodyPin PB0 // Buzzer pin
#define buttonPin PB2 // Button pin
#define F_CPU == 16000000 // Set the CPU frequency to 16 MHz

// LED configuration
#define NUMPIXELS 7 // Number of RGB leds
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// Timing and brightness settings
#define DELAYVAL 500 // Delay in milliseconds between RGB lighting patterns
int brightness = 10; // Set RGB-LED brightness level (0 to 255)
int songSpeed = 2;   // Adjust song speed (1 = normal, >1 = faster, <1 = slower)

// Christmas melodies and tempos
int melody[] = {
    NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_G5, NOTE_C5, NOTE_D5,
    NOTE_E5, NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5,
    NOTE_E5, NOTE_D5, NOTE_D5, NOTE_E5, NOTE_D5, NOTE_G5};
int tempo[] = {8, 8, 4, 8, 8, 4, 8, 8, 8, 8, 2, 8, 8, 8, 8, 8, 8, 16, 16, 8, 8, 8, 8, 4, 4};

int wish_melody[] = {
    NOTE_B3, NOTE_F4, NOTE_F4, NOTE_G4, NOTE_F4, NOTE_E4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_G4,
    NOTE_G4, NOTE_A4, NOTE_G4, NOTE_F4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_A4, NOTE_A4, NOTE_B4,
    NOTE_A4, NOTE_G4, NOTE_F4, NOTE_D4, NOTE_B3, NOTE_B3, NOTE_D4, NOTE_G4, NOTE_E4, NOTE_F4};
int wish_tempo[] = {
    4, 4, 8, 8, 8, 8, 4, 4, 4, 4, 8, 8, 8, 8, 4, 4, 4, 4, 8, 8, 8, 8, 4, 4, 8, 8, 4, 4, 4, 2};

int santa_melody[] = {
    NOTE_G4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_G4, NOTE_G4, NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5,
    NOTE_C5, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_G4, NOTE_G4, NOTE_A4, NOTE_G4, NOTE_F4, NOTE_F4,
    NOTE_E4, NOTE_G4, NOTE_C4, NOTE_E4, NOTE_D4, NOTE_F4, NOTE_B3, NOTE_C4};
int santa_tempo[] = {
    8, 8, 8, 4, 4, 4, 8, 8, 4, 4, 4, 8, 8, 4, 4, 4, 8, 8, 4, 2, 4, 4, 4, 4, 4, 2, 4, 1};

// Current song index
int currentSong = 0; // 0 = Jingle Bells, 1 = We Wish You a Merry Christmas, 2 = Santa Claus is Coming to Town

// Button state
bool buttonPressed = false;

// Timing variables for non-blocking pattern display
unsigned long previousMillis = 0;

void setup()
{
    pixels.begin();
    pixels.setBrightness(brightness);
    randomSeed(analogRead(0)); // Seed the random number generator for LED light patterns

    // Initialize pins for buzzer and button
    pinMode(melodyPin, OUTPUT);
    pinMode(buttonPin, INPUT_PULLUP);
}

void displayRandomPattern()
{
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= DELAYVAL)
    {
        previousMillis = currentMillis;

        pixels.clear();
        for (int i = 0; i < NUMPIXELS; i++)
        {
            if (random(2) == 0)
            {
                pixels.setPixelColor(i, pixels.Color(150, 0, 0)); // Random red
            }
            else
            {
                pixels.setPixelColor(i, pixels.Color(0, 150, 0)); // Random green
            }
        }
        pixels.show();
    }
}

void buzz(int targetPin, long frequency, long length)
{
    long delayValue = 1000000 / frequency / 2;  // calculate the delay value between transitions
    long numCycles = frequency * length / 1000; // calculate the number of cycles for proper timing

    for (long i = 0; i < numCycles; i++)
    {
        digitalWrite(targetPin, HIGH);
        delayMicroseconds(delayValue);
        digitalWrite(targetPin, LOW);
        delayMicroseconds(delayValue);
    }
}

void sing(int *melody, int *tempo, int size)
{
    // Play notes of the selected song
    for (int thisNote = 0; thisNote < size; thisNote++)
    {
        int noteDuration = 1000 / (tempo[thisNote] * songSpeed);
        buzz(melodyPin, melody[thisNote], noteDuration);
        int pauseBetweenNotes = noteDuration * 1.30;
        delay(pauseBetweenNotes);
        buzz(melodyPin, 0, noteDuration);
    }
}

void playNextSong()
{
    // Play the current song
    if (currentSong == 0)
    {
        sing(melody, tempo, sizeof(melody) / sizeof(int));
    }
    else if (currentSong == 1)
    {
        sing(wish_melody, wish_tempo, sizeof(wish_melody) / sizeof(int));
    }
    else if (currentSong == 2)
    {
        sing(santa_melody, santa_tempo, sizeof(santa_melody) / sizeof(int));
    }

    // Move to the next song
    currentSong = (currentSong + 1) % 3;
}

void loop()
{
    // Check if the button is pressed
    if (digitalRead(buttonPin) == LOW)
    {
        if (!buttonPressed)
        {
            buttonPressed = true;
            playNextSong();
        }
    }
    else
    {
        buttonPressed = false;
    }

    // Display random NeoPixel pattern
    displayRandomPattern();
}
