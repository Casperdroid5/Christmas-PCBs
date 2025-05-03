#include <FastLED.h>

// Hardware Configuration
#define RGB_PIN 3      // Data pin for LED strip
#define BUZZER 10      // Buzzer pin
#define BUTTON1 5      // Button 1 pin (play/change songs)
#define BUTTON2 4      // Button 2 pin (change LED patterns)
#define LDR_PIN 2      // Analog pin for LDR
#define NUM_LEDS 6     // Total number of LEDs
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

// Display Mode Enum - Christmas themed patterns
enum DisplayMode {
  STATIC_COLOR,
  RAINBOW_MODE,
  SNAKE_MODE,
  RANDOM_BLINK,
  CHASE_MODE,
  BREATHE_MODE,
  WAVE_MODE,
  CHRISTMAS_TWINKLE,
  OFF_MODE,
  LDR_REACTIVE_MODE
};

// Current mode
DisplayMode currentMode = STATIC_COLOR;

// Color Options - Modified for Christmas colors
int currentColorIndex = 0;
CRGB colorOptions[] = {
  CRGB::Red,
  CRGB::Green,
  CRGB::White,
  CRGB::Gold,    // Christmas gold
  CRGB::Blue,
  CRGB::Purple,
  0xFF0000,      // Red
  0x00FF00,      // Green
  0x000000       // Black (off)
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
const unsigned long TWINKLE_SPEED = 100;  // Christmas twinkle speed

// Snake Pattern Variables
uint8_t snakeHeadPos = 0;
const uint8_t snakeLength = 3;
uint8_t snakeHue = 0;

// Random Blink Variables
uint8_t randomLEDs[3] = {0};  // Tracks which LEDs are currently active
uint8_t randomHues[3] = {0};  // Hues for random LEDs
unsigned long randomBlinkInterval = 500;  // Slower random blink
unsigned long lastRandomUpdate = 0;

// Chase Pattern Variables
uint8_t chasePos = 0;
uint8_t chaseHue = 0;

// Breathe Pattern Variables
uint8_t breatheBrightness = 0;
bool breatheIncreasing = true;

// Wave Pattern Variables
uint8_t waveOffset = 0;

// Christmas Twinkle Variables
uint8_t twinkleLEDs[NUM_LEDS] = {0};
unsigned long lastTwinkleUpdate = 0;

// LDR Reactive Mode Variables
uint8_t ldrHue = 0;
unsigned long lastLdrHueUpdate = 0;

// Bluetooth LE Flag
bool bleEnabled = false;

// Christmas Music Notes with correct frequencies
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define REST     0

// Jingle Bells Melody (corrected with accurate notes and tempo)
const int jingleBells[] = {
  NOTE_E5, NOTE_E5, NOTE_E5,
  NOTE_E5, NOTE_E5, NOTE_E5,
  NOTE_E5, NOTE_G5, NOTE_C5, NOTE_D5,
  NOTE_E5,
  NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5,
  NOTE_F5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5,
  NOTE_E5, NOTE_D5, NOTE_D5, NOTE_E5,
  NOTE_D5, NOTE_G5
};

const int jingleBellsTempo[] = {
  8, 8, 4,
  8, 8, 4,
  8, 8, 8, 8,
  2,
  8, 8, 8, 8,
  8, 8, 8, 16, 16,
  8, 8, 8, 8,
  4, 4
};
const int jingleBellsLength = sizeof(jingleBells) / sizeof(jingleBells[0]);

// We Wish You a Merry Christmas (corrected with accurate notes and tempo)
const int wishMelody[] = {
  NOTE_B3, 
  NOTE_F4, NOTE_F4, NOTE_G4, NOTE_F4, NOTE_E4,
  NOTE_D4, NOTE_D4, NOTE_D4,
  NOTE_G4, NOTE_G4, NOTE_A4, NOTE_G4, NOTE_F4,
  NOTE_E4, NOTE_E4, NOTE_E4,
  NOTE_A4, NOTE_A4, NOTE_B4, NOTE_A4, NOTE_G4,
  NOTE_F4, NOTE_D4, NOTE_B3, NOTE_B3,
  NOTE_D4, NOTE_G4, NOTE_E4,
  NOTE_F4
};

const int wishTempo[] = {
  4,
  4, 8, 8, 8, 8,
  4, 4, 4,
  4, 8, 8, 8, 8,
  4, 4, 4,
  4, 8, 8, 8, 8,
  4, 4, 8, 8,
  4, 4, 4,
  2
};
const int wishLength = sizeof(wishMelody) / sizeof(wishMelody[0]);

// Santa Claus Is Coming To Town (corrected with accurate notes and tempo)
const int santaMelody[] = {
  NOTE_G4,
  NOTE_E4, NOTE_F4, NOTE_G4, NOTE_G4, NOTE_G4,
  NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, NOTE_C5,
  NOTE_E4, NOTE_F4, NOTE_G4, NOTE_G4, NOTE_G4,
  NOTE_A4, NOTE_G4, NOTE_F4, NOTE_F4,
  NOTE_E4, NOTE_G4, NOTE_C4, NOTE_E4,
  NOTE_D4, NOTE_F4, NOTE_B3,
  NOTE_C4
};

const int santaTempo[] = {
  8,
  8, 8, 4, 4, 4,
  8, 8, 4, 4, 4,
  8, 8, 4, 4, 4,
  8, 8, 4, 2,
  4, 4, 4, 4,
  4, 2, 4,
  1
};
const int santaLength = sizeof(santaMelody) / sizeof(santaMelody[0]);

// Song State Machine
enum SongState {
  IDLE,
  PLAYING_JINGLE_BELLS,
  PLAYING_WISH,
  PLAYING_SANTA,
  COLOR_FADING,
  CONFETTI_MODE,
  ENDING
};

// Current song selection
int currentSong = 0;
const int NUM_SONGS = 3;

SongState songState = IDLE;

// Timing Variables
unsigned long previousNoteTime = 0;
unsigned long noteEndTime = 0;
unsigned long noteDuration = 0;
unsigned long pauseDuration = 0;
bool noteIsPlaying = false;
int currentNote = 0;

// Animation Variables
int ledsLit = 0;

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
void updateChristmasTwinkle();
void turnOffAllLEDs();
void updateDisplay();
void startChristmasSong(int songIndex);
void stopSong();
void enableBluetooth();
void disableBluetooth();
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

  // Initialize average for LDR readings
  for (int i = 0; i < LDR_SAMPLES; i++) {
    ldrReadings[i] = 0;
  }

  // Initial state
  updateDisplay();

  // Start serial
  Serial.begin(115200);
  Serial.println("Christmas Card Ready!");
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
      Serial.println("DARKNESS DETECTED! No action taken.");
      // Note: removed song triggering - just log the event
    }
    
    // Log status change if it changed
    if (previousStatus != currentLightStatus) {
      Serial.print("Light status changed to: ");
      switch (currentLightStatus) {
        case LIGHT_DARK:
          Serial.println("DARK - Brightness set to maximum");
          break;
        case LIGHT_MEDIUM:
          Serial.println("MEDIUM - Brightness adjusted");
          break;
        case LIGHT_BRIGHT:
          Serial.println("BRIGHT - Brightness reduced");
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
        // Dramatic pulsing with red/green Christmas colors
        uint8_t pulseValue = sin8(millis()/20);
        
        // Alternating red and green for Christmas
        for (int i = 0; i < NUM_LEDS; i++) {
          if (i % 2 == 0) {
            leds[i] = CHSV(0, 255, pulseValue); // Red
          } else {
            leds[i] = CHSV(96, 255, pulseValue); // Green
          }
        }
      } else {
        // Normal dark room effect - slower pulsing red/green
        uint8_t timeValue = millis()/10;
        for (int i = 0; i < NUM_LEDS; i++) {
          if (sin8(timeValue + i*20) > 200) {
            leds[i] = CRGB::Red;
          } else if (sin8(timeValue + i*20) < 50) {
            leds[i] = CRGB::Green;
          } else {
            leds[i] = CRGB::Gold;
          }
        }
      }
      break;
      
    case LIGHT_MEDIUM:
      // Medium brightness effect - Christmas colors gradient
      for(int i = 0; i < NUM_LEDS; i++) {
        // Create a nice red-green-gold gradient for medium lighting
        uint8_t pos = (ldrHue + i * 5) % 255;
        if (pos < 85) {
          leds[i] = CRGB::Red;
        } else if (pos < 170) {
          leds[i] = CRGB::Green;
        } else {
          leds[i] = CRGB::Gold;
        }
      }
      break;
      
    case LIGHT_BRIGHT:
      // Bright room effect - more subtle Christmas colors
      for(int i = 0; i < NUM_LEDS; i++) {
        // Softer colors for bright conditions
        uint8_t pos = (ldrHue + i * 10) % 255;
        if (pos < 85) {
          leds[i] = CRGB(150, 0, 0); // Darker red
        } else if (pos < 170) {
          leds[i] = CRGB(0, 150, 0); // Darker green
        } else {
          leds[i] = CRGB(100, 80, 0); // Darker gold
        }
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
    // Button 1 - Control Songs
    if (reading1 != button1State) {
      button1State = reading1;
      if (button1State == LOW) {
        if (songState == IDLE) {
          // If not playing, start the current song
          startChristmasSong(currentSong);
        } else {
          // If already playing, stop the current song and start the next one
          stopSong();
          currentSong = (currentSong + 1) % NUM_SONGS;
          startChristmasSong(currentSong);
        }
      }
    }

    // Button 2 - Change LED patterns
    if (reading2 != button2State) {
      button2State = reading2;
      if (button2State == LOW) {
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
      // Special mode - toggle Bluetooth LE
      if (!bleEnabled) {
        enableBluetooth();
      } else {
        disableBluetooth();
      }
      // Reset timer to avoid triggering multiple times
      bothButtonsStartTime = millis();
    }
  } else {
    bothButtonsPressed = false;
  }
}

void enableBluetooth() {
  Serial.println("BLUETOOTH MODE ACTIVATED!");
  Serial.println("BLE Device Name: christmas PCB");
  
  // Here you would add code to actually enable BLE
  // This depends on which BLE library you're using
  // For example:
  // BLEDevice::init("christmas PCB");
  
  bleEnabled = true;
  
  // Visual indication that BLE is enabled
  for (int i = 0; i < 3; i++) {
    fill_solid(leds, NUM_LEDS, CRGB::Blue);
    FastLED.show();
    delay(300);
    fill_solid(leds, NUM_LEDS, CRGB::Black);
    FastLED.show();
    delay(300);
  }
  
  // Restore display
  updateDisplay();
}

void disableBluetooth() {
  Serial.println("BLUETOOTH MODE DEACTIVATED!");
  
  // Here you would add code to disable BLE
  // This depends on which BLE library you're using
  // For example:
  // BLEDevice::deinit();
  
  bleEnabled = false;
  
  // Visual indication that BLE is disabled
  for (int i = 0; i < 3; i++) {
    fill_solid(leds, NUM_LEDS, CRGB::Red);
    FastLED.show();
    delay(300);
    fill_solid(leds, NUM_LEDS, CRGB::Black);
    FastLED.show();
    delay(300);
  }
  
  // Restore display
  updateDisplay();
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
    case CHRISTMAS_TWINKLE:
      patternUpdateInterval = TWINKLE_SPEED;
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
        
      case CHRISTMAS_TWINKLE:
        updateChristmasTwinkle();
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
  snakeHue += 1;  // Slower color change for snake
  
  // Draw the snake body - Christmas colors
  for (int i = 0; i < snakeLength; i++) {
    int pos = (snakeHeadPos - i + NUM_LEDS) % NUM_LEDS;  // Ensure positive wrap-around
    // Fade the brightness based on distance from head
    int brightness = 255 - (i * 255 / snakeLength);
    
    // Christmas red and green alternating
    if ((snakeHue / 20) % 2 == 0) {
      leds[pos] = CRGB(brightness, 0, 0); // Red
    } else {
      leds[pos] = CRGB(0, brightness, 0); // Green
    }
  }
}

void updateRandomBlinkPattern(unsigned long currentTime) {
  // Random LEDs turn on and off
  if (currentTime - lastRandomUpdate >= randomBlinkInterval) {
    lastRandomUpdate = currentTime;
    
    // Turn off old random LEDs
    for (int i = 0; i < 3; i++) {
      if (randomLEDs[i] < NUM_LEDS) {
        leds[randomLEDs[i]] = CRGB::Black;
      }
    }
    
    // Generate new random LEDs with Christmas colors
    for (int i = 0; i < 3; i++) {
      randomLEDs[i] = random8(NUM_LEDS);
      randomHues[i] = random8(3); // 0, 1, or 2
      
      // Christmas color scheme
      if (randomHues[i] == 0) {
        leds[randomLEDs[i]] = CRGB::Red;
      } else if (randomHues[i] == 1) {
        leds[randomLEDs[i]] = CRGB::Green;
      } else {
        leds[randomLEDs[i]] = CRGB::Gold;
      }
    }
  }
}

void updateChasePattern() {
  // A chase pattern that runs around the LEDs
  turnOffAllLEDs();
  
  // Christmas colors chase
  int position = chasePos % NUM_LEDS;
  
  // Christmas Red, Green, or Gold
  uint8_t colorChoice = (chasePos / 3) % 3;
  
  if (colorChoice == 0) {
    leds[position] = CRGB::Red;
  } else if (colorChoice == 1) {
    leds[position] = CRGB::Green;
  } else {
    leds[position] = CRGB::Gold;
  }
  
  // Update position
  chasePos = (chasePos + 1) % NUM_LEDS;
}

void updateBreathePattern() {
  // Breathing effect - all LEDs fade in and out together
  if (breatheIncreasing) {
    breatheBrightness += 3;  // Slower increase
    if (breatheBrightness >= 250) {
      breatheIncreasing = false;
    }
  } else {
    breatheBrightness -= 3;  // Slower decrease
    if (breatheBrightness <= 5) {
      breatheIncreasing = true;
      colorFadeHue += 5;  // Change color slightly with each breath
    }
  }
  
  // Use fixed colors for Christmas - alternate between red and green
  if ((colorFadeHue / 20) % 2 == 0) {
    fill_solid(leds, NUM_LEDS, CRGB(breatheBrightness, 0, 0)); // Red
  } else {
    fill_solid(leds, NUM_LEDS, CRGB(0, breatheBrightness, 0)); // Green
  }
  
  FastLED.setBrightness(255); // Override global brightness to allow full breathe effect
}

void updateWavePattern() {
  // Wave pattern - a sine wave of color moving through the LEDs
  waveOffset += 5;
  
  for (int i = 0; i < NUM_LEDS; i++) {
    // Creating a wave effect through the LEDs with Christmas colors
    uint8_t sinBrightness = sin8(waveOffset + (i * 255 / NUM_LEDS));
    
    // Use different colors for different parts of the strip
    if (i < NUM_LEDS/3) {
      leds[i] = CRGB(sinBrightness, 0, 0); // Red wave
    } 
    else if (i < 2*NUM_LEDS/3) {
      leds[i] = CRGB(0, sinBrightness, 0); // Green wave
    }
    else {
      leds[i] = CRGB(sinBrightness, sinBrightness/2, 0); // Gold-ish wave
    }
  }
}

void updateChristmasTwinkle() {
  // Christmas twinkle effect - random LEDs twinkle like Christmas lights
  // Fade all LEDs slightly
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].fadeToBlackBy(10);
  }
  
  // Randomly light up LEDs with Christmas colors
  if (random8(3) == 0) {
    int pos = random8(NUM_LEDS);
    int colorChoice = random8(3);
    
    if (colorChoice == 0) {
      leds[pos] = CRGB::Red;
    } else if (colorChoice == 1) {
      leds[pos] = CRGB::Green;
    } else {
      leds[pos] = CRGB::Gold;
    }
  }
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
      fill_solid(leds, NUM_LEDS, colorOptions[currentColorIndex]);
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
      for (int i = 0; i < 3; i++) {
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
      
    case CHRISTMAS_TWINKLE:
      // Initialize Christmas twinkle
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

void startChristmasSong(int songIndex) {
  currentNote = 0;
  ledsLit = 0;
  
  // Set which Christmas song to play based on the index
  switch (songIndex) {
    case 0:
      songState = PLAYING_JINGLE_BELLS;
      Serial.println("Starting Jingle Bells");
      break;
    case 1:
      songState = PLAYING_WISH;
      Serial.println("Starting We Wish You a Merry Christmas");
      break;
    case 2:
      songState = PLAYING_SANTA;
      Serial.println("Starting Santa Claus is Coming to Town");
      break;
  }
  
  previousNoteTime = millis();
  noteIsPlaying = false;
  
  // If not in static color mode, select a random color for the song
  if (currentMode != STATIC_COLOR) {
    currentColorIndex = random8(NUM_COLORS - 1); // Avoid the last color (black)
    Serial.print("Selected random color for song: ");
    Serial.println(currentColorIndex);
  }
}

void stopSong() {
  songState = IDLE;
  noTone(BUZZER);
  updateDisplay();
  Serial.println("Song stopped");
}

void updateColorFade() {
  unsigned long currentTime = millis();

  if (currentTime - lastColorFadeUpdate >= colorFadeUpdateInterval) {
    lastColorFadeUpdate = currentTime;
    colorFadeHue++;

    for (int i = 0; i < NUM_LEDS; i++) {
      // Use only red and green for Christmas color fading
      uint8_t hue = (colorFadeHue + (i * 128 / NUM_LEDS)) % 255;
      if (hue < 128) {
        // Red to yellow range
        uint8_t greenVal = map(hue, 0, 127, 0, 255);
        leds[i] = CRGB(255, greenVal, 0);
      } else {
        // Yellow to green range
        uint8_t redVal = map(hue, 128, 255, 255, 0);
        leds[i] = CRGB(redVal, 255, 0);
      }
    }
  }
}

void updateConfettiMode() {
  // Add new confetti in Christmas colors
  if (random8() < CONFETTI_SPAWN_RATE) {
    int pos = random16(NUM_LEDS);
    int colorChoice = random8(3);
    
    if (colorChoice == 0) {
      leds[pos] = CRGB::Red;
    } else if (colorChoice == 1) {
      leds[pos] = CRGB::Green;
    } else {
      leds[pos] = CRGB::Gold;
    }
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

    case PLAYING_JINGLE_BELLS:
      if (!noteIsPlaying && currentTime >= previousNoteTime) {
        if (currentNote < jingleBellsLength) {
          // Play note
          noteDuration = (1000 / jingleBellsTempo[currentNote]);
          pauseDuration = noteDuration * 0.3;
          tone(BUZZER, jingleBells[currentNote], noteDuration);

          // Light LEDs progressively based on song progress
          float ledsPerNote = (float)(NUM_LEDS) / jingleBellsLength;
          int targetLEDs = round((currentNote + 1) * ledsPerNote);

          while (ledsLit < targetLEDs && ledsLit < NUM_LEDS) {
            // Christmas color pattern
            if (ledsLit % 3 == 0) {
              leds[ledsLit] = CRGB::Red;
            } else if (ledsLit % 3 == 1) {
              leds[ledsLit] = CRGB::Green;
            } else {
              leds[ledsLit] = CRGB::Gold;
            }
            ledsLit++;
          }

          noteEndTime = currentTime + noteDuration;
          noteIsPlaying = true;
        } else {
          // Song complete, transition to color fading
          songState = COLOR_FADING;
          colorFadeHue = 0;
          lastColorFadeUpdate = currentTime;
          Serial.println("Jingle Bells complete, starting color fade");
        }
      } else if (noteIsPlaying && currentTime >= noteEndTime) {
        noteIsPlaying = false;
        currentNote++;
        previousNoteTime = currentTime + pauseDuration;
      }
      break;

    case PLAYING_WISH:
      if (!noteIsPlaying && currentTime >= previousNoteTime) {
        if (currentNote < wishLength) {
          // Play note
          noteDuration = (1000 / wishTempo[currentNote]);
          pauseDuration = noteDuration * 0.3;
          tone(BUZZER, wishMelody[currentNote], noteDuration);

          // Light LEDs progressively based on song progress
          float ledsPerNote = (float)(NUM_LEDS) / wishLength;
          int targetLEDs = round((currentNote + 1) * ledsPerNote);

          while (ledsLit < targetLEDs && ledsLit < NUM_LEDS) {
            // Christmas color pattern
            if (ledsLit % 3 == 0) {
              leds[ledsLit] = CRGB::Red;
            } else if (ledsLit % 3 == 1) {
              leds[ledsLit] = CRGB::Green;
            } else {
              leds[ledsLit] = CRGB::Gold;
            }
            ledsLit++;
          }

          noteEndTime = currentTime + noteDuration;
          noteIsPlaying = true;
        } else {
          // Song complete, transition to color fading
          songState = COLOR_FADING;
          colorFadeHue = 0;
          lastColorFadeUpdate = currentTime;
          Serial.println("We Wish You a Merry Christmas complete, starting color fade");
        }
      } else if (noteIsPlaying && currentTime >= noteEndTime) {
        noteIsPlaying = false;
        currentNote++;
        previousNoteTime = currentTime + pauseDuration;
      }
      break;

    case PLAYING_SANTA:
      if (!noteIsPlaying && currentTime >= previousNoteTime) {
        if (currentNote < santaLength) {
          // Play note
          noteDuration = (900 / santaTempo[currentNote]); // Note using 900 here for slightly faster tempo
          pauseDuration = noteDuration * 0.3;
          tone(BUZZER, santaMelody[currentNote], noteDuration);

          // Light LEDs progressively based on song progress
          float ledsPerNote = (float)(NUM_LEDS) / santaLength;
          int targetLEDs = round((currentNote + 1) * ledsPerNote);

          while (ledsLit < targetLEDs && ledsLit < NUM_LEDS) {
            // Christmas color pattern
            if (ledsLit % 3 == 0) {
              leds[ledsLit] = CRGB::Red;
            } else if (ledsLit % 3 == 1) {
              leds[ledsLit] = CRGB::Green;
            } else {
              leds[ledsLit] = CRGB::White;
            }
            ledsLit++;
          }

          noteEndTime = currentTime + noteDuration;
          noteIsPlaying = true;
        } else {
          // Song complete, transition to color fading
          songState = COLOR_FADING;
          colorFadeHue = 0;
          lastColorFadeUpdate = currentTime;
          Serial.println("Santa Claus is Coming to Town complete, starting color fade");
        }
      } else if (noteIsPlaying && currentTime >= noteEndTime) {
        noteIsPlaying = false;
        currentNote++;
        previousNoteTime = currentTime + pauseDuration;
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