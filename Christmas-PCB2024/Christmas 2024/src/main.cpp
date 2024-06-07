// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// Released under the GPLv3 license to match the rest of the
// Adafruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#include <Arduino.h>

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        1

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 7 // Popular NeoPixel ring size

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 500 // Time (in milliseconds) to pause between patterns

int brightness = 10; // Set brightness level (0 to 255)

void setup() {
  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.

  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  pixels.setBrightness(brightness); // Set initial brightness level
  randomSeed(analogRead(0)); // Initialize random number generator
}

void loop() {
  pixels.clear(); // Set all pixel colors to 'off'
  
  for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
    if (random(2) == 0) {
      pixels.setPixelColor(i, pixels.Color(150, 0, 0)); // Random red
    } else {
      pixels.setPixelColor(i, pixels.Color(0, 150, 0)); // Random green
    }
  }
  
  pixels.show(); // Send the updated pixel colors to the hardware.
  delay(DELAYVAL); // Pause before next pattern
}
