#include <FastLED.h>

// Hardware Configuration
#define RGB_PIN 3      // Data pin for LED strip
#define BUZZER 10      // Buzzer pin
#define BUTTON1 5      // Button 1 pin (change color)
#define BUTTON2 4      // Button 2 pin (play song)
#define LDR_PIN 2      // Analog pin for LDR
#define NUM_LEDS 18    // Total number of LEDs
#define BASE_BRIGHTNESS 50  // Base brightness level

#define LDR_SAMPLES 10
#define DARK_THRESHOLD 1650   // Room is dark below this value
#define MEDIUM_THRESHOLD 2200 // Room is medium lit below this value
#define BRIGHT_THRESHOLD 3000 // Room is brightly lit at or above this value

// Light status enum
enum LightStatus {
  LIGHT_DARK,
  LIGHT_MEDIUM,
  LIGHT_BRIGHT
};

int ldrReadings[LDR_SAMPLES];  // Array to store readings for moving average
int ldrReadIndex = 0;          // Index for the current reading
int ldrTotal = 0;              // Running total of readings
bool isDark = false;           // Boolean for dark detection
bool wasDark = false;          // Previous state
unsigned long darkStartTime = 0; // Time when darkness was first detected
unsigned long darkDuration = 5000; // Duration needed to trigger dark event (5 seconds)
LightStatus currentLightStatus = LIGHT_MEDIUM; // Default light status


// LED Mapping
const uint8_t digit1Mapping[6] = { 0, 1, 2, 3, 4, 5 };                             // Digit '1' mapping
const uint8_t digit2Mapping[12] = { 8, 9, 11, 10, 6, 7, 12, 13, 14, 15, 16, 17 };  // Digit '8' mapping

// LED Array
CRGB leds[NUM_LEDS];

// Button Handling
bool button1State = HIGH;
bool lastButton1State = HIGH;
bool button2State = HIGH;
bool lastButton2State = HIGH;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

// LDR Variables
int ldrValue = 0;
int brightness = BASE_BRIGHTNESS;
unsigned long lastLdrCheck = 0;
const unsigned long ldrCheckInterval = 500; // Check LDR every 500ms

// Display Mode Enum - Adding new patterns
enum DisplayMode {
  STATIC_COLOR,
  RAINBOW_MODE,
  SNAKE_MODE,
  RANDOM_BLINK,
  CHASE_MODE,
  BREATHE_MODE,
  WAVE_MODE,
  OFF_MODE,
  LDR_REACTIVE_MODE  // New mode that reacts to light levels
};

// Current mode
DisplayMode currentMode = STATIC_COLOR;

// Color Options (Used for STATIC_COLOR mode)
int currentColorIndex = 0;
CRGB colorOptions[] = {
  CRGB::Red,
  CRGB::Green,
  CRGB::Blue,
  CRGB::Purple,
  CRGB::Yellow,
  CRGB::Cyan,
  CRGB::White,
  CRGB::Black
};
#define NUM_COLORS (sizeof(colorOptions) / sizeof(colorOptions[0]))

// Pattern Variables
unsigned long lastPatternUpdate = 0;
unsigned long patternUpdateInterval = 50;  // Default update interval for patterns

// Pattern-specific speed controls
const unsigned long RAINBOW_SPEED = 100;  // Slower rainbow cycle
const unsigned long SNAKE_SPEED = 150;    // Slower snake movement
const unsigned long CHASE_SPEED = 120;    // Slightly slower chase pattern
const unsigned long WAVE_SPEED = 80;      // Medium speed for wave
const unsigned long BREATHE_SPEED = 30;   // Keep breathe relatively smooth

// Snake Pattern Variables
uint8_t snakeHeadPos = 0;
const uint8_t snakeLength = 6;
uint8_t snakeHue = 0;

// Random Blink Variables
uint8_t randomLEDs[5] = {0};  // Tracks which LEDs are currently active
uint8_t randomHues[5] = {0};  // Hues for random LEDs
unsigned long randomBlinkInterval = 500;  // Slower random blink (was 200)
unsigned long lastRandomUpdate = 0;

// Chase Pattern Variables
uint8_t chasePos = 0;
uint8_t chaseHue = 0;

// Breathe Pattern Variables
uint8_t breatheBrightness = 0;
bool breatheIncreasing = true;

// Wave Pattern Variables
uint8_t waveOffset = 0;

// LDR Reactive Mode Variables
uint8_t ldrHue = 0;
unsigned long lastLdrHueUpdate = 0;

// Music Notes
const int NOTE_C4 = 262;
const int NOTE_D4 = 294;
const int NOTE_E4 = 330;
const int NOTE_F4 = 349;
const int NOTE_G4 = 392;
const int NOTE_A4 = 440;
const int NOTE_B4 = 494;
const int NOTE_C5 = 523;
const int NOTE_D5 = 587;
const int NOTE_E5 = 659;
const int NOTE_F5 = 698;
const int NOTE_G5 = 784;

// Happy Birthday Melody
const int melody[] = {
  NOTE_G4, NOTE_G4, NOTE_A4, NOTE_G4, NOTE_C5, NOTE_B4,
  NOTE_G4, NOTE_G4, NOTE_A4, NOTE_G4, NOTE_D5, NOTE_C5,
  NOTE_G4, NOTE_G4, NOTE_G5, NOTE_E5, NOTE_C5, NOTE_B4, NOTE_A4,
  NOTE_F5, NOTE_F5, NOTE_E5, NOTE_C5, NOTE_D5, NOTE_C5
};

const int noteDurationFractions[] = {
  8, 8, 4, 4, 4, 2,
  8, 8, 4, 4, 4, 2,
  8, 8, 4, 4, 4, 4, 4,
  8, 8, 4, 4, 4, 2
};

const int tempo = 250;
const int melodyLength = sizeof(melody) / sizeof(melody[0]);
int currentNote = 0;

// Song State Machine
enum SongState {
  IDLE,
  PLAYING_BIRTHDAY,
  PLAYING_HIEPER,
  COLOR_FADING,
  CONFETTI_MODE,
  ENDING
};
SongState songState = IDLE;

// Timing Variables
unsigned long previousNoteTime = 0;
unsigned long noteEndTime = 0;
unsigned long noteDuration = 0;
unsigned long pauseDuration = 0;
bool noteIsPlaying = false;

// Animation Variables
int ledsLit = 0;
const int hieperSequence[] = { 1, 1, 1, 1, 3, 1, 2 };  // 0=very short, 1=short, 2=long, 3=pause
const int hieperLength = 7;
int currentHieperNote = 0;

// Color Fading
uint8_t colorFadeHue = 0;
unsigned long lastColorFadeUpdate = 0;
const unsigned long colorFadeUpdateInterval = 20;

// Confetti Mode
#define CONFETTI_DURATION 86400000 // 24 hours  
#define CONFETTI_SPAWN_RATE 13
#define CONFETTI_FADE_RATE 1
unsigned long confettiStartTime = 0;
uint8_t confettiHue = 1;

// Button Combination Detection
bool bothButtonsPressed = false;
unsigned long bothButtonsStartTime = 0;
#define BOTH_BUTTONS_HOLD_TIME 2000  // 2 second

// Function declarations
void checkButtons();
void checkBothButtons();
void checkLDR();
void updatePatterns();
void updateLdrReactiveMode();
void updateRainbowPattern();
void updateSnakePattern();
void updateRandomBlinkPattern(unsigned long currentTime);
void updateChasePattern();
void updateBreathePattern();
void updateWavePattern();
void turnOffAllLEDs();
void updateDisplay();
void turnOnDigit1(CRGB color);
void turnOnDigit2(CRGB color);
void startBirthdaySong();
void stopSong();
void startConfettiMode();
void updateColorFade();
void updateConfettiMode();
void updateSong();

void setup() {
  // Initialize hardware
  pinMode(BUZZER, OUTPUT);
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);
  pinMode(LDR_PIN, INPUT);  // Set LDR pin as input

  // Initialize LEDs
  FastLED.addLeds<WS2812B, RGB_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(BASE_BRIGHTNESS);

  // Initial state
  updateDisplay();

  // Start serial
  Serial.begin(115200);
  Serial.println("18th BirthdayCard Ready!");
}

void loop() {
  checkButtons();
  checkBothButtons();
  updateSong();
  checkLDR();
  
  // Update pattern animations
  updatePatterns();

  if (songState == COLOR_FADING) {
    updateColorFade();
  } else if (songState == CONFETTI_MODE) {
    updateConfettiMode();
  }


  
  FastLED.show();
}

void checkLDR() {
  unsigned long currentTime = millis();
  
  // Only check LDR periodically to avoid too much noise
  if (currentTime - lastLdrCheck >= ldrCheckInterval) {
    lastLdrCheck = currentTime;
    
    // Read raw LDR value (0-4095 for ESP32)
    int rawReading = analogRead(LDR_PIN);
    
    // Update moving average
    ldrTotal -= ldrReadings[ldrReadIndex];  // Subtract the oldest reading
    ldrReadings[ldrReadIndex] = rawReading; // Add the new reading
    ldrTotal += ldrReadings[ldrReadIndex];  // Add to the total
    ldrReadIndex = (ldrReadIndex + 1) % LDR_SAMPLES; // Advance index
    
    // Calculate the average
    ldrValue = ldrTotal / LDR_SAMPLES;
    
    // Determine light status
    LightStatus previousStatus = currentLightStatus;
    
    if (ldrValue < DARK_THRESHOLD) {
      currentLightStatus = LIGHT_DARK;
      brightness = 255;  // Maximum brightness in dark conditions
    } else if (ldrValue < MEDIUM_THRESHOLD) {
      currentLightStatus = LIGHT_MEDIUM;
      // Linear transition from 255 to 150 between dark and medium thresholds
      brightness = 255 - ((ldrValue - DARK_THRESHOLD) * 105 / (MEDIUM_THRESHOLD - DARK_THRESHOLD));
    } else {
      currentLightStatus = LIGHT_BRIGHT;
      // Linear transition from 150 to 30 between medium and bright thresholds
      brightness = 150 - ((ldrValue - MEDIUM_THRESHOLD) * 120 / (BRIGHT_THRESHOLD - MEDIUM_THRESHOLD));
      if (brightness < 30) brightness = 30; // Minimum brightness
    }
    
    // Apply brightness if we're not in breathe mode
    if (currentMode != BREATHE_MODE) {
      FastLED.setBrightness(brightness);
    }
    
    // Check if it's dark and track persistence
    bool currentlyDark = (currentLightStatus == LIGHT_DARK);
    
    // State change: not dark -> dark
    if (currentlyDark && !wasDark) {
      darkStartTime = currentTime;
      wasDark = true;
    }
    // State change: dark -> not dark
    else if (!currentlyDark && wasDark) {
      wasDark = false;
      isDark = false;
    }
    
    // Check if it's been dark long enough to trigger dark event
    if (wasDark && !isDark && (currentTime - darkStartTime > darkDuration)) {
      isDark = true;
      Serial.println("DARKNESS DETECTED! Triggering dark event");
      // You can add any actions to take when darkness is detected here
    }
    
    // Log status change if it changed
    if (previousStatus != currentLightStatus) {
      Serial.print("Light status changed to: ");
      switch (currentLightStatus) {
        case LIGHT_DARK:
          Serial.println("DARK");
          break;
        case LIGHT_MEDIUM:
          Serial.println("MEDIUM");
          break;
        case LIGHT_BRIGHT:
          Serial.println("BRIGHT");
          break;
      }
    }
    
    // If in LDR_REACTIVE_MODE, update the display based on light level
    if (currentMode == LDR_REACTIVE_MODE) {
      updateLdrReactiveMode();
    }
    
    Serial.print("LDR Raw: ");
    Serial.print(rawReading);
    Serial.print(", Average: ");
    Serial.print(ldrValue);
    Serial.print(", Brightness: ");
    Serial.print(brightness);
    Serial.print(", Status: ");
    Serial.print(currentLightStatus == LIGHT_DARK ? "DARK" : 
                (currentLightStatus == LIGHT_MEDIUM ? "MEDIUM" : "BRIGHT"));
    Serial.print(", Persistent Dark: ");
    Serial.println(isDark ? "YES" : "NO");
  }
}

void updateLdrReactiveMode() {
  // Using the three status levels: DARK, MEDIUM, BRIGHT
  
  switch (currentLightStatus) {
    case LIGHT_DARK:
      if (isDark) {
        // Special effect when persistent darkness is detected
        // Dramatic pulsing with blue/purple hues
        uint8_t pulseValue = sin8(millis()/20);
        fill_solid(leds, NUM_LEDS, CHSV(200, 255, pulseValue));
      } else {
        // Normal dark room effect - slower pulsing blue
        fill_solid(leds, NUM_LEDS, CHSV(180 + (sin8(millis()/10) / 8), 255, brightness));
      }
      break;
      
    case LIGHT_MEDIUM:
      // Medium brightness effect - teal-blue gradient
      for(int i = 0; i < NUM_LEDS; i++) {
        // Create a nice teal-blue gradient for medium lighting
        leds[i] = CHSV(150 + (i * 30 / NUM_LEDS), 255, brightness);
      }
      break;
      
    case LIGHT_BRIGHT:
      // Bright room effect - vibrant colors
      for(int i = 0; i < NUM_LEDS; i++) {
        // Create a yellow-red-magenta gradient for bright conditions
        leds[i] = CHSV((i * 60 / NUM_LEDS), 255, brightness);
      }
      break;
  }
  
  // Update the hue slowly over time for some variation
  if (millis() - lastLdrHueUpdate > 100) {
    lastLdrHueUpdate = millis();
    ldrHue++;
  }
}
  

void checkButtons() {
  bool reading1 = digitalRead(BUTTON1);
  bool reading2 = digitalRead(BUTTON2);

  // Debounce logic
  if (reading1 != lastButton1State || reading2 != lastButton2State) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // Button 1 - Change mode
    if (reading1 != button1State) {
      button1State = reading1;
      if (button1State == LOW) {
        // If we're in a static color mode, increment the color
        if (currentMode == STATIC_COLOR) {
          currentColorIndex = (currentColorIndex + 1) % NUM_COLORS;
          if (currentColorIndex == NUM_COLORS - 1) {  // After going through all colors
            // Switch to first pattern mode
            currentMode = RAINBOW_MODE;
            Serial.println("Mode changed to RAINBOW");
          } else {
            Serial.print("Color changed to index: ");
            Serial.println(currentColorIndex);
          }
        } else {
          // If we're in a pattern mode, switch to the next pattern
          currentMode = (DisplayMode)((int)currentMode + 1);
          if (currentMode > LDR_REACTIVE_MODE) {  // After the last pattern
            currentMode = STATIC_COLOR;   // Reset to static color mode
            currentColorIndex = 0;        // Reset to first color
            Serial.println("Mode reset to STATIC_COLOR");
          } else {
            Serial.print("Mode changed to: ");
            Serial.println(currentMode);
          }
        }
        updateDisplay();  // Update display based on new mode
      }
    }

    // Button 2 - Play song
    if (reading2 != button2State) {
      button2State = reading2;
      if (button2State == LOW) {
        if (songState == IDLE) {
          startBirthdaySong();
        } else {
          stopSong();
        }
      }
    }
  }

  lastButton1State = reading1;
  lastButton2State = reading2;
}

void checkBothButtons() {
  bool btn1 = digitalRead(BUTTON1) == LOW;
  bool btn2 = digitalRead(BUTTON2) == LOW;

  if (btn1 && btn2) {
    if (!bothButtonsPressed) {
      bothButtonsPressed = true;
      bothButtonsStartTime = millis();
    } else if (millis() - bothButtonsStartTime >= BOTH_BUTTONS_HOLD_TIME) {
      if (songState != CONFETTI_MODE) {
        startConfettiMode();
      }
    }
  } else {
    bothButtonsPressed = false;
  }
}

void updatePatterns() {
  unsigned long currentTime = millis();
  
  // Only update patterns if we're in IDLE state (not playing music)
  if (songState != IDLE) return;
  
  // Set the appropriate update interval based on current mode
  switch (currentMode) {
    case RAINBOW_MODE:
      patternUpdateInterval = RAINBOW_SPEED;
      break;
    case SNAKE_MODE:
      patternUpdateInterval = SNAKE_SPEED;
      break;
    case CHASE_MODE:
      patternUpdateInterval = CHASE_SPEED;
      break;
    case WAVE_MODE:
      patternUpdateInterval = WAVE_SPEED;
      break;
    case BREATHE_MODE:
      patternUpdateInterval = BREATHE_SPEED;
      break;
    case RANDOM_BLINK:
      // Random blink uses its own timing mechanism
      break;
    case LDR_REACTIVE_MODE:
      // LDR reactive mode is handled in checkLDR()
      return;
    default:
      patternUpdateInterval = 50; // Default for other modes
      break;
  }
  
  // Update based on current pattern mode
  if (currentTime - lastPatternUpdate >= patternUpdateInterval) {
    lastPatternUpdate = currentTime;
    
    switch (currentMode) {
      case STATIC_COLOR:
        // Static color is set by updateDisplay(), no animation updates needed
        break;
        
      case RAINBOW_MODE:
        updateRainbowPattern();
        break;
        
      case SNAKE_MODE:
        updateSnakePattern();
        break;
        
      case RANDOM_BLINK:
        updateRandomBlinkPattern(currentTime);
        break;
        
      case CHASE_MODE:
        updateChasePattern();
        break;
        
      case BREATHE_MODE:
        updateBreathePattern();
        break;
        
      case WAVE_MODE:
        updateWavePattern();
        break;
        
      case OFF_MODE:
        turnOffAllLEDs();
        break;
    }
  }
}

void updateRainbowPattern() {
  // Simple rainbow pattern that cycles through the color wheel
  // Slowed down color transition
  colorFadeHue++;  // Increment by 1 for a slower color change
  
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(colorFadeHue + (i * 255 / NUM_LEDS), 255, 255);
  }
}

void updateSnakePattern() {
  // Snake pattern - a moving segment of LEDs
  turnOffAllLEDs();  // Clear all LEDs
  
  // Update snake head position
  snakeHeadPos = (snakeHeadPos + 1) % NUM_LEDS;
  snakeHue += 1;  // Slower color change for snake (was 2)
  
  // Draw the snake body
  for (int i = 0; i < snakeLength; i++) {
    int pos = (snakeHeadPos - i + NUM_LEDS) % NUM_LEDS;  // Ensure positive wrap-around
    // Fade the brightness based on distance from head
    int brightness = 255 - (i * 255 / snakeLength);
    leds[pos] = CHSV(snakeHue, 255, brightness);
  }
}

void updateRandomBlinkPattern(unsigned long currentTime) {
  // Random LEDs turn on and off
  if (currentTime - lastRandomUpdate >= randomBlinkInterval) {
    lastRandomUpdate = currentTime;
    
    // Turn off old random LEDs
    for (int i = 0; i < 5; i++) {
      if (randomLEDs[i] < NUM_LEDS) {
        leds[randomLEDs[i]] = CRGB::Black;
      }
    }
    
    // Generate new random LEDs
    for (int i = 0; i < 5; i++) {
      randomLEDs[i] = random8(NUM_LEDS);
      randomHues[i] = random8();
      leds[randomLEDs[i]] = CHSV(randomHues[i], 255, 255);
    }
  }
}

void updateChasePattern() {
  // A chase pattern that runs around the digits
  turnOffAllLEDs();
  
  // First handle digit 1 (the "1")
  if (chasePos < 6) {
    leds[digit1Mapping[chasePos]] = CHSV(chaseHue, 255, 255);
  } 
  // Then handle digit 2 (the "8")
  else if (chasePos < 18) {
    leds[digit2Mapping[chasePos - 6]] = CHSV(chaseHue, 255, 255);
  }
  
  // Update position and hue
  chasePos = (chasePos + 1) % 18;
  chaseHue += 5;
}

void updateBreathePattern() {
  // Breathing effect - all LEDs fade in and out together
  if (breatheIncreasing) {
    breatheBrightness += 3;  // Slower increase (was 5)
    if (breatheBrightness >= 250) {
      breatheIncreasing = false;
    }
  } else {
    breatheBrightness -= 3;  // Slower decrease (was 5)
    if (breatheBrightness <= 5) {
      breatheIncreasing = true;
      colorFadeHue += 5;  // Change color slightly with each breath
    }
  }
  
  fill_solid(leds, NUM_LEDS, CHSV(colorFadeHue, 255, breatheBrightness));
  FastLED.setBrightness(255); // Override global brightness to allow full breathe effect
}

void updateWavePattern() {
  // Wave pattern - a sine wave of color moving through the digits
  waveOffset += 5;
  
  for (int i = 0; i < 6; i++) {
    // Creating a wave effect through the first digit
    uint8_t sinBrightness = sin8(waveOffset + (i * 255 / 6));
    leds[digit1Mapping[i]] = CHSV(colorFadeHue, 255, sinBrightness);
  }
  
  for (int i = 0; i < 12; i++) {
    // Creating a wave effect through the second digit
    uint8_t sinBrightness = sin8(waveOffset + ((i + 6) * 255 / 12));
    leds[digit2Mapping[i]] = CHSV(colorFadeHue + 30, 255, sinBrightness); // Offset hue for contrast
  }
  
  colorFadeHue++;  // Slowly change the base color
}

void turnOffAllLEDs() {
  fill_solid(leds, NUM_LEDS, CRGB::Black);
}

void updateDisplay() {
  // Reset any ongoing animations when changing modes
  if (songState == COLOR_FADING || songState == CONFETTI_MODE) {
    songState = IDLE;
  }

  // Clear all LEDs first
  turnOffAllLEDs();
  
  // Then set the display according to current mode
  switch (currentMode) {
    case STATIC_COLOR:
      turnOnDigit1(colorOptions[currentColorIndex]);
      turnOnDigit2(colorOptions[currentColorIndex]);
      break;
      
    case RAINBOW_MODE:
      // Will be handled by updatePatterns
      break;
      
    case SNAKE_MODE:
      // Initialize snake pattern
      snakeHeadPos = 0;
      snakeHue = random8();
      break;
      
    case RANDOM_BLINK:
      // Initialize random blink
      for (int i = 0; i < 5; i++) {
        randomLEDs[i] = random8(NUM_LEDS);
        randomHues[i] = random8();
      }
      break;
      
    case CHASE_MODE:
      // Initialize chase pattern
      chasePos = 0;
      chaseHue = random8();
      break;
      
    case BREATHE_MODE:
      // Initialize breathe pattern
      breatheBrightness = 0;
      breatheIncreasing = true;
      colorFadeHue = random8();
      break;
      
    case WAVE_MODE:
      // Initialize wave pattern
      waveOffset = 0;
      colorFadeHue = random8();
      break;
      
    case OFF_MODE:
      // All LEDs off, already handled by turnOffAllLEDs()
      break;
      
    case LDR_REACTIVE_MODE:
      // Initialize LDR reactive mode
      ldrHue = random8();
      break;
  }
  
  // Make sure to reset the brightness to the current LDR value
  if (currentMode != BREATHE_MODE) {
    FastLED.setBrightness(brightness);
  }
}

void turnOnDigit1(CRGB color) {
  for (int i = 0; i < 6; i++) {
    leds[digit1Mapping[i]] = color;
  }
}

void turnOnDigit2(CRGB color) {
  for (int i = 0; i < 12; i++) {
    leds[digit2Mapping[i]] = color;
  }
}

void startBirthdaySong() {
  currentNote = 0;
  ledsLit = 0;
  songState = PLAYING_BIRTHDAY;
  previousNoteTime = millis();
  noteIsPlaying = false;
  turnOffAllLEDs();
  
  // If not in static color mode, select a random color for the song
  if (currentMode != STATIC_COLOR) {
    currentColorIndex = random8(NUM_COLORS - 1); // Avoid the last color (black)
    Serial.print("Selected random color for song: ");
    Serial.println(currentColorIndex);
  }
  
  Serial.println("Starting birthday song");
}

void stopSong() {
  songState = IDLE;
  noTone(BUZZER);
  updateDisplay();
  Serial.println("Song stopped");
}

void startConfettiMode() {
  Serial.println("CONFETTI MODE ACTIVATED!");
  songState = CONFETTI_MODE;
  confettiStartTime = millis();
  confettiHue = 0;

  // Celebration sound
  tone(BUZZER, NOTE_E5, 200);
  delay(200);
  tone(BUZZER, NOTE_C5, 100);
  delay(100);
  tone(BUZZER, NOTE_G5, 500);

  // Print secret message
  Serial.println("Secret message activated!");
  turnOffAllLEDs();
}

void updateColorFade() {
  unsigned long currentTime = millis();

  if (currentTime - lastColorFadeUpdate >= colorFadeUpdateInterval) {
    lastColorFadeUpdate = currentTime;
    colorFadeHue++;

    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CHSV(colorFadeHue + (i * 255 / NUM_LEDS), 255, 255);
    }
  }
}

void updateConfettiMode() {
  // Add new confetti
  if (random8() < CONFETTI_SPAWN_RATE) {
    int pos = random16(NUM_LEDS);
    leds[pos] = CHSV(confettiHue + random8(32), 200 + random8(55), 200 + random8(55));
    confettiHue += random8(8, 16);
  }

  // Fade out
  fadeToBlackBy(leds, NUM_LEDS, CONFETTI_FADE_RATE);

  // Check duration
  if (millis() - confettiStartTime > CONFETTI_DURATION) {
    songState = IDLE;
    updateDisplay();
    Serial.println("Confetti mode ended");
  }
}

void updateSong() {
  unsigned long currentTime = millis();

  switch (songState) {
    case IDLE:
      break;

    case PLAYING_BIRTHDAY:
      if (!noteIsPlaying && currentTime >= previousNoteTime) {
        if (currentNote < melodyLength) {
          // Play note
          noteDuration = (tempo * 4 / noteDurationFractions[currentNote]);
          pauseDuration = noteDuration * 0.3;
          tone(BUZZER, melody[currentNote], noteDuration);

          // Light LEDs progressively
          float ledsPerNote = (float)(NUM_LEDS) / melodyLength;
          int targetLEDs = round((currentNote + 1) * ledsPerNote);

          while (ledsLit < targetLEDs && ledsLit < NUM_LEDS) {
            if (ledsLit < 6) {
              leds[digit1Mapping[ledsLit]] = colorOptions[currentColorIndex];
            } else {
              leds[digit2Mapping[ledsLit - 6]] = colorOptions[currentColorIndex];
            }
            ledsLit++;
          }

          noteEndTime = currentTime + noteDuration;
          noteIsPlaying = true;
        } else {
          // Song complete, transition to hieper
          fill_solid(leds, NUM_LEDS, colorOptions[currentColorIndex]);
          songState = PLAYING_HIEPER;
          currentHieperNote = 0;
          noteIsPlaying = false;
          previousNoteTime = currentTime + 500;
        }
      } else if (noteIsPlaying && currentTime >= noteEndTime) {
        noteIsPlaying = false;
        currentNote++;
        previousNoteTime = currentTime + pauseDuration;
      }
      break;

    case PLAYING_HIEPER:
      if (!noteIsPlaying && currentTime >= previousNoteTime) {
        if (currentHieperNote < hieperLength) {
          int hieperType = hieperSequence[currentHieperNote];
          if (hieperType == 0) {  // Very short beep
            tone(BUZZER, NOTE_C5, 80);
            noteDuration = 80;
            fill_solid(leds, NUM_LEDS, CRGB::White);
          } else if (hieperType == 1) {  // Short beep
            tone(BUZZER, NOTE_C5, 150);
            noteDuration = 150;
            fill_solid(leds, NUM_LEDS, CRGB::White);
          } else if (hieperType == 3) {  // Pause
            noTone(BUZZER);
            noteDuration = 300;  // 0.3-second pause
            turnOffAllLEDs();
          } else {                       // Long beep (type 2)
            tone(BUZZER, NOTE_C5, 800);  // Extended length for the final "piieeeeep"
            noteDuration = 800;
            for (int j = 0; j < NUM_LEDS; j++) {
              leds[j] = CHSV(j * 255 / NUM_LEDS, 255, 255);
            }
          }
          noteEndTime = currentTime + noteDuration;
          noteIsPlaying = true;
        } else {
          songState = COLOR_FADING;
          colorFadeHue = 0;
          lastColorFadeUpdate = currentTime;
          Serial.println("Starting color fade");
        }
      } else if (noteIsPlaying && currentTime >= noteEndTime) {
        noteIsPlaying = false;
        currentHieperNote++;
        // Adjust timing based on the previous note type
        if (currentHieperNote < hieperLength) {
          if (hieperSequence[currentHieperNote - 1] == 0) {         // After very short beep
            previousNoteTime = currentTime + 50;                    // Shorter pause
          } else if (hieperSequence[currentHieperNote - 1] == 1) {  // After short beep
            previousNoteTime = currentTime + 100;                   // Short pause
          } else {                                                  // After long beep
            previousNoteTime = currentTime + 200;                   // Longer pause
          }
        }
      }
      break;

    case COLOR_FADING:
      // Handled in main loop
      break;

    case CONFETTI_MODE:
      // Handled in main loop
      break;

    case ENDING:
      songState = IDLE;
      updateDisplay();
      break;
  }
}