/*  The MIT License (MIT)

    Copyright (c) 2015 Andy Tran

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.

    Updated Christmas Songs Player
    Uses timer-based approach for non-blocking song playback
*/

#include <Arduino.h>
#include <FastLED.h>
#include "christmas_songs.h"

// Hardware Configuration - Updated for new PCB
#define RGB_PIN 3      // Data pin for LED strip (GPIO3)
#define BUZZER 10      // Buzzer pin (GPIO10)
#define BUTTON1 4      // Button 1 pin (GPIO4) - change color/pattern ONLY
#define BUTTON2 5      // Button 2 pin (GPIO5) - play/stop song
#define BATT_SENSE 3   // Battery voltage sensing pin (GPIO3)
#define LDR_PIN 6      // Light sensor pin (GPIO6)
#define NUM_LEDS 8     // Total number of LEDs (based on schematic)

// LED Array
CRGB leds[NUM_LEDS];

// The current variable keeps track of the song to be played.
// Currently, as there are 6 tracks in the repertoire, the range of the current variable is from 0 to 5.
// Track 0: Jingle Bells
// Track 1: We Wish You a Merry Christmas
// Track 2: Rudolph the Red-Nosed Reindeer
// Track 3: Let It Snow! Let It Snow! Let It Snow!
// Track 4: All I Want for Christmas Is You
// Track 5: Silent Night

int current = 0;

// Set this to be the pin that your buzzer resides in
int tonePin = BUZZER;

// Song state variables
boolean songPlaying = false;
int songStep = 0;
unsigned long stepStartTime = 0;
unsigned long noteEndTime = 0;
boolean noteCurrentlyPlaying = false;

// Button handling
bool button1State = HIGH;
bool lastButton1State = HIGH;
bool button2State = HIGH;
bool lastButton2State = HIGH;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

// Function declarations
void checkButtons();
void handleButton1Press();
void handleButton2Press();
void startSong();
void stopSong();
void incrementSong();
void updateSong();
void updateLights();
void displayChristmasLights();

void setup() {
  Serial.begin(115200);
  Serial.println("🎄 Christmas PCB Starting... 🎄");
  
  // Initialize hardware
  pinMode(tonePin, OUTPUT);
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);
  pinMode(BATT_SENSE, INPUT);
  pinMode(LDR_PIN, INPUT);
  
  // Initialize LEDs
  FastLED.addLeds<WS2812B, RGB_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(128);
  
  // Turn off all LEDs initially
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  FastLED.show();
  
  Serial.println("🎅 Christmas PCB Ready! 🎁");
  Serial.println("Controls:");
  Serial.println("  Button 1: Change LED colors/patterns");
  Serial.println("  Button 2: Play/Stop Christmas songs");
  Serial.print("Current song queued: ");
  Serial.println(songNames[current]);
}

void loop() {
  checkButtons();
  updateSong();
  updateLights();
  FastLED.show();
}

void checkButtons() {
  bool reading1 = digitalRead(BUTTON1);
  bool reading2 = digitalRead(BUTTON2);

  // Debounce logic
  if (reading1 != lastButton1State || reading2 != lastButton2State) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // Button 1 - Change LED patterns/colors
    if (reading1 != button1State) {
      button1State = reading1;
      if (button1State == LOW) {
        handleButton1Press();
      }
    }

    // Button 2 - Play/Stop songs
    if (reading2 != button2State) {
      button2State = reading2;
      if (button2State == LOW) {
        handleButton2Press();
      }
    }
  }

  lastButton1State = reading1;
  lastButton2State = reading2;
}

void handleButton1Press() {
  // Button 1 changes LED patterns/colors
  Serial.println("Button 1: LED pattern/color change");
  // Add your LED pattern logic here
}

void handleButton2Press() {
  // Button 2 controls songs
  if (songPlaying) {
    // Stop current song and move to next
    stopSong();
    incrementSong();
  } else {
    // Start current song
    startSong();
  }
}

void startSong() {
  songPlaying = true;
  songStep = 0;
  stepStartTime = millis();
  noteCurrentlyPlaying = false;
  
  Serial.print("🎵 Now playing: ");
  Serial.println(songNames[current]);
}

void stopSong() {
  songPlaying = false;
  noTone(tonePin);
  Serial.println("🎵 Song stopped");
}

void incrementSong() {
  current = (current + 1) % NUM_CHRISTMAS_SONGS;
  Serial.print("Next song queued: ");
  Serial.println(songNames[current]);
}

void updateSong() {
  if (!songPlaying) return;
  
  unsigned long currentTime = millis();
  Song currentSongData = getSongData((ChristmasSong)current);
  
  // Check if current note has finished
  if (noteCurrentlyPlaying && currentTime >= noteEndTime) {
    noTone(tonePin);
    noteCurrentlyPlaying = false;
    songStep++;
    stepStartTime = currentTime;
  }
  
  // Start next note if ready
  if (!noteCurrentlyPlaying && currentTime >= stepStartTime) {
    if (songStep < currentSongData.size) {
      int frequency = currentSongData.melody[songStep];
      int noteDuration = currentSongData.baseTempo / currentSongData.tempo[songStep];
      
      // Play the note
      if (frequency > 0) {
        tone(tonePin, frequency, noteDuration * 0.9);
      }
      
      noteEndTime = currentTime + (noteDuration * 1.3);
      noteCurrentlyPlaying = true;
      
    } else {
      // Song finished
      Serial.println("🎄 Song finished! 🎁");
      songPlaying = false;
      
      // Auto-advance to next song
      incrementSong();
    }
  }
}

void updateLights() {
  // Only show Christmas lights when song is playing
  if (!songPlaying) {
    fill_solid(leds, NUM_LEDS, CRGB::Black);
    return;
  }
  
  displayChristmasLights();
}

void displayChristmasLights() {
  // Progressive LED lighting with festive patterns
  Song currentSongData = getSongData((ChristmasSong)current);
  float progress = (float)songStep / currentSongData.size;
  int ledsToLight = (int)(progress * NUM_LEDS);
  if (ledsToLight > NUM_LEDS) ledsToLight = NUM_LEDS;
  
  // Turn off all LEDs first
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  
  // Different Christmas lighting patterns for different songs
  switch (current) {
    case JINGLE_BELLS:
      // Classic alternating red and green
      for (int i = 0; i < ledsToLight; i++) {
        leds[i] = (i % 2 == 0) ? CRGB::Red : CRGB::Green;
      }
      break;
      
    case SILENT_NIGHT:
      // Peaceful white and soft blue
      for (int i = 0; i < ledsToLight; i++) {
        leds[i] = (i % 2 == 0) ? CRGB::White : CRGB::Blue;
      }
      break;
      
    case RUDOLPH:
      // A special pattern for Rudolph
      for (int i = 0; i < ledsToLight; i++) {
        if (i == 0) { // The first LED is Rudolph's nose
          leds[i] = CRGB::Red;
        } else {
          leds[i] = CRGB::White;
        }
      }
      break;
      
    default:
      // Traditional Christmas colors with sparkle for all other songs
      for (int i = 0; i < ledsToLight; i++) {
        if (i % 3 == 0) {
          leds[i] = CRGB::Red;
        } else if (i % 3 == 1) {
          leds[i] = CRGB::Green;
        } else {
          leds[i] = CRGB::Gold;
        }
      }
      break;
  }
  
  // Add a sparkle effect every few beats
  if (songStep % 8 == 0 && ledsToLight > 0) {
    int sparklePos = random(0, ledsToLight);
    leds[sparklePos] = CRGB::White;
  }
}