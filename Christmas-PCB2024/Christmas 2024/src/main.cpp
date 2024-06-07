/*
  Arduino Christmas Songs
  
  Based on a project and code by Dipto Pratyaksa, updated on 31/3/13
  Modified for Christmas by Joshi, on Dec 17th, 2017.
*/

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
unsigned long noteStartTime = 0;
unsigned long noteDuration = 0;
int currentNote = 0;
bool playing = false;
bool ledsSet = false;

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

void playNote(int *melody, int *tempo, int noteIndex)
{
    int frequency = melody[noteIndex];
    noteDuration = 1000 / (tempo[noteIndex] * songSpeed);
    buzz(melodyPin, frequency, noteDuration);
    noteStartTime = millis();
    playing = true;
}

void playNextSong()
{
    currentNote = 0;
    noteStartTime = 0;
    noteDuration = 0;
    playing = false;
}

void loop()
{
    unsigned long currentMillis = millis();

    // Check if the button is pressed
    if (digitalRead(buttonPin) == LOW)
    {
        if (!buttonPressed)
        {
            buttonPressed = true;
            displayRandomPattern();
            ledsSet = true;
            playNextSong();
        }
    }
    else
    {
        buttonPressed = false;
    }

    // Play the current song
    if (playing)
    {
        if (currentMillis - noteStartTime >= noteDuration)
        {
            currentNote++;
            if (currentSong == 0 && currentNote < sizeof(melody) / sizeof(int))
            {
                playNote(melody, tempo, currentNote);
            }
            else if (currentSong == 1 && currentNote < sizeof(wish_melody) / sizeof(int))
            {
                playNote(wish_melody, wish_tempo, currentNote);
            }
            else if (currentSong == 2 && currentNote < sizeof(santa_melody) / sizeof(int))
            {
                playNote(santa_melody, santa_tempo, currentNote);
            }
            else
            {
                playing = false;
                currentSong = (currentSong + 1) % 3;
            }
        }
    }
    else
    {
        if (buttonPressed)
        {
            if (currentSong == 0)
            {
                playNote(melody, tempo, currentNote);
            }
            else if (currentSong == 1)
            {
                playNote(wish_melody, wish_tempo, currentNote);
            }
            else if (currentSong == 2)
            {
                playNote(santa_melody, santa_tempo, currentNote);
            }
        }
    }

    // Prevent further updates to the LEDs once set
    if (!ledsSet)
    {
        displayRandomPattern();
    }
}
