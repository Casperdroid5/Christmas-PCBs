#include <Arduino.h>  // Required for PlatformIO
#include <FastLED.h>
#include <Preferences.h>
#include "christmas_songs.h"  // Include our Christmas songs header

// Preferences object for storing settings
Preferences preferences;

// Hardware Configuration - Updated for new PCB
#define RGB_PIN 3      // Data pin for LED strip (GPIO3)
#define BUZZER 10      // Buzzer pin (GPIO10)
#define BUTTON1 4      // Button 1 pin (GPIO4) - change color/pattern ONLY
#define BUTTON2 5      // Button 2 pin (GPIO5) - play/stop song
#define BATT_SENSE 0   // Battery voltage sensing pin (GPIO0)
#define LDR_PIN 2      // Light sensor pin (GPIO2)
#define NUM_LEDS 8     // Total number of LEDs 

// Battery voltage thresholds (after voltage divider: actual voltage = reading * 2)
// CR2032 battery not used in this design
#define BATT_AAA_MIN 1.8        // 3.6V / 2 = 1.8V (AAA batteries)
#define BATT_AAA_MAX 2.75       // 5.5V / 2 = 2.75V (AAA batteries)
#define BATT_NO_DETECT 0.5      // Below this = no battery detected

// Brightness levels based on power source - Updated to max 60%
#define BRIGHTNESS_USB 128      // 50% brightness for USB power (128/255 = 50%)
#define BRIGHTNESS_BATTERY 60   // 25% brightness for battery power (60/255 = 25%)

// LDR Configuration
#define LDR_THRESHOLD 2000      // Light threshold (0-4095) - adjust based on testing
#define LDR_HYSTERESIS 200      // Prevents flickering by adding hysteresis
#define LDR_CHECK_INTERVAL 1000 // Check LDR every 1 second

// LED Array
CRGB leds[NUM_LEDS];

// Button Handling
bool button1State = HIGH;
bool lastButton1State = HIGH;
bool button2State = HIGH;
bool lastButton2State = HIGH;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

// Long press detection for Button 1
unsigned long button1PressStartTime = 0;
bool button1LongPressDetected = false;
const unsigned long longPressTime = 2000; // 2 seconds for long press

// Power Management
enum PowerSource {
  POWER_USB,
  POWER_AAA
};
PowerSource currentPowerSource = POWER_USB;
int currentBrightness = BRIGHTNESS_USB;
bool wifiEnabled = true;

// Settings and Uptime Management
bool settingsChanged = false;
unsigned long lastSaveTime = 0;
const unsigned long saveInterval = 30000; // Save every 30 seconds if needed

// Uptime tracking (64-bit for extended duration)
uint32_t totalUptimeLow = 0;   // Lower 32 bits of uptime in seconds
uint32_t totalUptimeHigh = 0;  // Upper 32 bits of uptime in seconds
unsigned long lastMillisCheck = 0; // For millis() overflow detection

// Light Sensor Management
bool ledsEnabledByLight = true;  // LEDs enabled by light sensor
bool ledsForceEnabled = false;   // Override for button presses/songs - NOW SAVED TO FLASH
unsigned long lastLDRCheck = 0;
int lastLDRReading = 0;

// Auto re-enable in darkness
unsigned long darknessStartTime = 0;
bool inDarkness = false;
const unsigned long darknessReEnableDelay = 5000; // 5 seconds

// Display Mode Enum
enum DisplayMode {
  STATIC_COLOR,
  RANDOM_SCATTER,     // Random red/green pattern
  RAINBOW_MODE,
  SNAKE_MODE,
  RANDOM_BLINK,
  CHASE_MODE,
  WAVE_MODE,
  FADE_RANDOM,       // Fading patterns
  SPARKLE_MODE,      // Twinkling effect
  FIREWORK_MODE,     // Exploding pattern
  METEOR_MODE,       // Shooting star effect
  CANDY_CANE_MODE,   // Rotating stripes
  OFF_MODE
};

DisplayMode currentMode = STATIC_COLOR;

// Color Options (Limited to Christmas colors: Red, Green, White)
int currentColorIndex = 0;
CRGB colorOptions[] = {
  CRGB::Red,
  CRGB::Green,
  CRGB::Green // Solid green mode
};
#define NUM_COLORS (sizeof(colorOptions) / sizeof(colorOptions[0]))

// Pattern Variables
unsigned long lastPatternUpdate = 0;
unsigned long patternUpdateInterval = 50;

// Pattern-specific speed controls, higher value results in slower animation speed
const unsigned long RAINBOW_SPEED = 600;    
const unsigned long SNAKE_SPEED = 800;       
const unsigned long CHASE_SPEED = 720;      
const unsigned long WAVE_SPEED = 480;       
const unsigned long FADE_SPEED = 200;       
const unsigned long SCATTER_SPEED = 1000;   
const unsigned long SPARKLE_SPEED = 200;    
const unsigned long FIREWORK_SPEED = 400;   
const unsigned long METEOR_SPEED = 200;     
const unsigned long CANDY_SPEED = 600;      

// Fade pattern variables
uint8_t fadeProgress = 0;
CRGB currentFadeColors[NUM_LEDS];
CRGB targetFadeColors[NUM_LEDS];
bool needNewFadeTarget = true;

// Snake Pattern Variables
uint8_t snakeHeadPos = 0;
const uint8_t snakeLength = 3;  // Shorter snake for 8 LEDs
uint8_t snakeHue = 0;

// Random Blink Variables
uint8_t randomLEDs[3] = {0};    // Fewer active LEDs for power saving
uint8_t randomHues[3] = {0};
unsigned long randomBlinkInterval = 600;
unsigned long lastRandomUpdate = 0;

// Chase Pattern Variables
uint8_t chasePos = 0;
uint8_t chaseHue = 0;

// Breathe Pattern Variables
uint8_t breatheBrightness = 0;
bool breatheIncreasing = true;
uint8_t breatheHue = 0;

// Wave Pattern Variables
uint8_t waveOffset = 0;
uint8_t waveHue = 0;

// Sparkle Pattern Variables
uint8_t sparklePositions[NUM_LEDS];
uint8_t sparkleBrightness[NUM_LEDS];
uint8_t sparkleColors[NUM_LEDS];

// Firework Pattern Variables
struct Firework {
    int8_t position;
    uint8_t phase;  // 0=launch, 1=explode, 2=fade
    uint8_t brightness;
    bool isRed;
};
Firework currentFirework = {-1, 0, 0, true};

// Meteor Pattern Variables
int8_t meteorPos = -1;
uint8_t meteorTail[3];
bool meteorIsRed = true;

// Candy Cane Pattern Variables
uint8_t candyOffset = 0;
uint8_t stripeWidth = 2;

// Tree Pattern Variables
uint8_t treePhase = 0;
uint8_t treeStarBrightness = 0;
bool treeStarIncreasing = true;

// Song State Machine
enum SongState {
  IDLE,
  PLAYING_SONG
};
SongState songState = IDLE;

// Christmas Songs - Updated to use our new song system
ChristmasSong currentSong = SANTA_CLAUS_IS_COMIN;
Song currentSongData;

// Song timing variables - Updated for new playMusic function
unsigned long songStartTime = 0;
unsigned long songStepStartTime = 0;
unsigned long noteEndTime = 0;
int songStep = 0;
bool noteCurrentlyPlaying = false;

// Sensor monitoring - Updated for 1 second intervals
unsigned long lastBatteryCheck = 0;
unsigned long lastSensorOutput = 0;
const unsigned long batteryCheckInterval = 10000; // Check every 10 seconds
const unsigned long sensorOutputInterval = 500;   // Output sensor data every 0.5 seconds

// Function prototypes - Updated
void checkPowerSource();
void checkLightSensor();
bool shouldShowLEDs();
void printPowerStatus();
void outputSensorData();  // New function prototype
void handleButton1LongPress();
void flashLEDsToIndicateMode();
void checkButtons();
void handleButton1Press();
void handleButton2Press();
void updatePatterns();
void updateRainbowPattern();
void updateSnakePattern();
void updateRandomBlinkPattern(unsigned long currentTime);
void updateChasePattern();
void updateBreathePattern();
void updateWavePattern();
void turnOffAllLEDs();
void updateDisplay();
void startSong();
void stopSong();
void updateSong();
void displayChristmasLights();
void playMusic(const int16_t melody[], uint16_t numNotes, uint16_t songTempo);
void saveUptime();

// Function to mark settings as changed
void markSettingsChanged() {
    settingsChanged = true;
}

// Combined function to save settings and uptime if needed
void saveToMemory() {
    unsigned long currentMillis = millis();
    
    // Check for millis() overflow
    if (currentMillis < lastSaveTime) {
        // millis() has overflowed, force a save
        lastSaveTime = currentMillis;
    }
    
    // Only save if enough time has passed since last save and changes exist
    if (currentMillis - lastSaveTime >= saveInterval && settingsChanged) {
        preferences.begin("xmas-pcb", false);  // Open preferences in RW mode
        
        // Save settings
        preferences.putUChar("displayMode", (uint8_t)currentMode);
        preferences.putUChar("colorIndex", currentColorIndex);
        preferences.putUChar("songIndex", (uint8_t)currentSong);
        preferences.putBool("forceEnabled", ledsForceEnabled);  // Save force enable state
        
        // Save 64-bit uptime as two 32-bit values
        preferences.putULong("uptimeLow", totalUptimeLow);
        preferences.putULong("uptimeHigh", totalUptimeHigh);
        
        preferences.end();
        
        lastSaveTime = currentMillis;
        settingsChanged = false;
        Serial.println("Settings and uptime saved to memory");
    }
}

// Function to load settings from non-volatile memory
void loadSettings() {
  preferences.begin("xmas-pcb", true);  // Open preferences in read-only mode
  
  // Load display mode with STATIC_COLOR as default
  currentMode = (DisplayMode)preferences.getUChar("displayMode", STATIC_COLOR);
  
  // Load color index with 0 (Red) as default
  currentColorIndex = preferences.getUChar("colorIndex", 0);
  
  // Load song index with SANTA_CLAUS_IS_COMIN as default
  currentSong = (ChristmasSong)preferences.getUChar("songIndex", SANTA_CLAUS_IS_COMIN);
  
  // Load force enable state with false as default
  ledsForceEnabled = preferences.getBool("forceEnabled", false);
  
  // Load 64-bit uptime from two 32-bit values
  totalUptimeLow = preferences.getULong("uptimeLow", 0);
  totalUptimeHigh = preferences.getULong("uptimeHigh", 0);
  
  preferences.end();
  
  // Initialize millis() overflow detection
  lastMillisCheck = millis();
  
  Serial.println("Settings loaded from memory:");
  Serial.print("Display Mode: ");
  Serial.println((int)currentMode);
  Serial.print("Color Index: ");
  Serial.println(currentColorIndex);
  Serial.print("Song: ");
  Serial.println(songNames[currentSong]);
  Serial.print("Force Enabled: ");
  Serial.println(ledsForceEnabled ? "Yes" : "No");
  
  // Calculate total seconds for display
  uint64_t totalSeconds = ((uint64_t)totalUptimeHigh << 32) | totalUptimeLow;
  Serial.print("Total Device Uptime: ");
  Serial.print(totalSeconds / 86400); // Days
  Serial.print(" days, ");
  Serial.print((totalSeconds % 86400) / 3600); // Hours
  Serial.print(" hours, ");
  Serial.print((totalSeconds % 3600) / 60); // Minutes
  Serial.print(" minutes, ");
  Serial.print(totalSeconds % 60); // Seconds
  Serial.println(" seconds");
}

void setup() {
  // Initialize serial with more aggressive output
  Serial.begin(115200);
  delay(1000);  // Give serial more time to initialize
  
  // Send multiple newlines to help serial monitor sync
  for(int i = 0; i < 10; i++) {
    Serial.write('\n');
    delay(10);
  }
  
  Serial.println("=====================================");
  Serial.println("=== Christmas PCB Starting... ======");
  Serial.println("=== Firmware Version: 1.1 ==========");
  Serial.println("=== No Force Enable Timeout ========");
  Serial.println("=====================================");
  Serial.flush();  // Make sure all serial data is sent
  delay(100);
  
  Serial.println("Initializing hardware...");
  Serial.flush();
  
  // Initialize hardware
  pinMode(BUZZER, OUTPUT);
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);
  pinMode(BATT_SENSE, INPUT);
  pinMode(LDR_PIN, INPUT);
  
  // Get random seed from LDR readings
  uint32_t seed = 0;
  for(int i = 0; i < 16; i++) {  // More readings since we're only using LDR
    seed = (seed << 2) | (analogRead(LDR_PIN) & 0x03);  // Use lower 2 bits (most random)
    delay(1);  // Small delay between readings
  }
  randomSeed(seed);  // Seed the random number generator
  
  // Check initial power source and light conditions
  checkPowerSource();
  checkLightSensor();
  
  // Initialize LEDs with appropriate brightness
  FastLED.addLeds<WS2812B, RGB_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(currentBrightness);
  
  // Load saved settings from memory
  loadSettings();
  
  // Update display with loaded settings
  updateDisplay();
  FastLED.show();
  
  Serial.println("Christmas PCB Ready!");
  Serial.println("Controls:");
  Serial.println("  Button 1: Change LED colors/patterns");
  Serial.println("  Button 2: Play/Stop Christmas songs");
  Serial.print("Current song queued: ");
  Serial.println(songNames[currentSong]);
  printPowerStatus();
}

void loop() {
  static unsigned long lastHeartbeat = 0;
  static unsigned long sessionUptime = 0;
  static bool lastSerialState = false;
  bool currentSerialState = Serial;
  
  // Heartbeat message and uptime tracking every second
  unsigned long currentMillis = millis();
  if (currentMillis - lastHeartbeat >= 1000) {
    sessionUptime++;
    
    // Update 64-bit uptime counter with overflow handling
    totalUptimeLow++;
    if (totalUptimeLow == 0) {  // Counter wrapped around
        totalUptimeHigh++;  // Increment high word
    }
    
    // Check for millis() overflow
    if (currentMillis < lastMillisCheck) {
        // millis() has overflowed, force a save to preserve uptime
        markSettingsChanged();
    }
    lastMillisCheck = currentMillis;
    
    lastHeartbeat = currentMillis;
    uint64_t totalSeconds = ((uint64_t)totalUptimeHigh << 32) | totalUptimeLow;
    
    Serial.print("Heartbeat - Session Uptime: ");
    Serial.print(sessionUptime);
    Serial.print("s, Total Uptime: ");
    Serial.print(totalSeconds / 86400); // Days
    Serial.print("d ");
    Serial.print((totalSeconds % 86400) / 3600); // Hours
    Serial.print("h ");
    Serial.print((totalSeconds % 3600) / 60); // Minutes
    Serial.print("m ");
    Serial.print(totalSeconds % 60); // Seconds
    Serial.println("s");
    Serial.flush();  // Force send the data
    
    // Mark settings as changed every second to ensure uptime is saved periodically
    markSettingsChanged();
  }
  
  // Check if serial state changed
  if (currentSerialState != lastSerialState) {
    if (currentSerialState) {
      Serial.println("\n=== Serial Connection Restored ===");
      printPowerStatus();  // Print current status when connection is restored
      Serial.flush();
    }
    lastSerialState = currentSerialState;
  }
  
  checkButtons();
  updateSong();
  
  // Check if LEDs should be shown based on light sensor and force enable status
  if (shouldShowLEDs()) {
    updatePatterns();
  } else {
    // Turn off LEDs when it's too bright (unless song is playing)
    if (songState == IDLE) {
      turnOffAllLEDs();
    } else {
      updatePatterns(); // Always show LEDs during song
    }
  }
  
  // Periodic monitoring
  if (millis() - lastBatteryCheck > batteryCheckInterval) {
    checkPowerSource();
    lastBatteryCheck = millis();
  }
  
  if (millis() - lastLDRCheck > LDR_CHECK_INTERVAL) {
    checkLightSensor();
    lastLDRCheck = millis();
  }
  
  // Output sensor data every 2 seconds
  if (millis() - lastSensorOutput > 2000) {
    outputSensorData();
    lastSensorOutput = millis();
  }
  
  // REMOVED: Force enable timeout check - ledsForceEnabled now persists until manually changed
  
  // Check if we need to save settings and uptime
  saveToMemory();
  
  FastLED.show();
}

void checkPowerSource() {
  // Read battery voltage (voltage divider gives us half the actual voltage)
  int reading = analogRead(BATT_SENSE);
  float voltage = (reading / 4095.0) * 3.3; // Convert to voltage
  
  PowerSource oldPowerSource = currentPowerSource;
  
  if (voltage < BATT_NO_DETECT) {
    // No battery detected - running on USB
    currentPowerSource = POWER_USB;
    currentBrightness = BRIGHTNESS_USB;
    wifiEnabled = true;
  } else if (voltage >= BATT_AAA_MIN && voltage <= BATT_AAA_MAX) {
    // AAA batteries detected
    currentPowerSource = POWER_AAA;
    currentBrightness = BRIGHTNESS_BATTERY;
    wifiEnabled = false;
  } else {
    // Unknown voltage - assume battery power for safety
    currentPowerSource = POWER_AAA;
    currentBrightness = BRIGHTNESS_BATTERY;
    wifiEnabled = false;
  }
  
  // Update brightness if power source changed
  if (oldPowerSource != currentPowerSource) {
    FastLED.setBrightness(currentBrightness);
    printPowerStatus();
    
    // Disable WiFi/Bluetooth for battery operation
    if (!wifiEnabled) {
      // Add WiFi disable code here if needed
      Serial.println("WiFi/Bluetooth disabled for battery operation");
    }
  }
}

void checkLightSensor() {
  int ldrReading = analogRead(LDR_PIN);
  
  // Check if we're in darkness (below threshold)
  bool isDark = ldrReading < (LDR_THRESHOLD - LDR_HYSTERESIS);
  
  // Track darkness duration for auto re-enable
  if (isDark && !inDarkness) {
    // Just entered darkness
    inDarkness = true;
    darknessStartTime = millis();
    Serial.print("Entered darkness at light level: ");
    Serial.println(ldrReading);
  } else if (!isDark && inDarkness) {
    // Exited darkness
    inDarkness = false;
    Serial.print("Exited darkness at light level: ");
    Serial.println(ldrReading);
  }
  
  // Auto re-enable LEDs if dark for more than 5 seconds
  if (inDarkness && !ledsForceEnabled && 
      (millis() - darknessStartTime) >= darknessReEnableDelay) {
    ledsForceEnabled = true;
    Serial.print("Auto re-enabled LEDs after ");
    Serial.print(darknessReEnableDelay / 1000);
    Serial.println(" seconds of darkness");
    markSettingsChanged(); // Save the new state
  }
  
  // Apply hysteresis to prevent flickering for light sensor control
  if (!ledsEnabledByLight && ldrReading < (LDR_THRESHOLD - LDR_HYSTERESIS)) {
    // It's getting dark enough - enable LEDs by light sensor
    ledsEnabledByLight = true;
    Serial.print("Light level decreased to ");
    Serial.print(ldrReading);
    Serial.println(" - LEDs enabled by light sensor");
  } else if (ledsEnabledByLight && ldrReading > (LDR_THRESHOLD + LDR_HYSTERESIS)) {
    // It's getting too bright - disable LEDs by light sensor (unless force enabled)
    ledsEnabledByLight = false;
    Serial.print("Light level increased to ");
    Serial.print(ldrReading);
    Serial.println(" - LEDs disabled by light sensor");
  }
  
  lastLDRReading = ldrReading;
}

bool shouldShowLEDs() {
  // Always show LEDs during song playback
  if (songState == PLAYING_SONG) {
    return true;
  }
  
  // Show LEDs if force enabled (persists until manually changed)
  if (ledsForceEnabled) {
    return true;
  }
  
  // Otherwise follow light sensor
  return ledsEnabledByLight;
}

void printPowerStatus() {
  Serial.print("Power Source: ");
  switch (currentPowerSource) {
    case POWER_USB:
      Serial.print("USB");
      break;
    case POWER_AAA:
      Serial.print("AAA");
      break;
  }
  Serial.print(", Brightness: ");
  Serial.print((currentBrightness * 100) / 255);
  Serial.print("%, Light Level: ");
  Serial.print(lastLDRReading);
  Serial.print(", LEDs: ");
  Serial.print(shouldShowLEDs() ? "Enabled" : "Disabled");
  Serial.print(", Force Enable: ");
  Serial.println(ledsForceEnabled ? "Yes" : "No");
}

void checkButtons() {
  bool reading1 = digitalRead(BUTTON1);
  bool reading2 = digitalRead(BUTTON2);

  // Debounce logic
  if (reading1 != lastButton1State || reading2 != lastButton2State) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // Button 1 - Change mode/color OR long press for force enable toggle
    if (reading1 != button1State) {
      button1State = reading1;
      
      if (button1State == LOW) {
        // Button pressed - start timing for long press
        button1PressStartTime = millis();
        button1LongPressDetected = false;
      } else {
        // Button released
        unsigned long pressDuration = millis() - button1PressStartTime;
        
        if (button1LongPressDetected) {
          // Long press was already handled, do nothing
        } else if (pressDuration < longPressTime) {
          // Short press - normal button function
          handleButton1Press();
        }
        
        button1LongPressDetected = false;
      }
    }
    
    // Check for long press while button is still held
    if (button1State == LOW && !button1LongPressDetected && 
        (millis() - button1PressStartTime) >= longPressTime) {
      button1LongPressDetected = true;
      handleButton1LongPress();
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
  // Button 1 SHORT PRESS - changes LED patterns/colors
  // Enable LEDs when button is pressed (even in bright light) - NO TIMEOUT
  if (!shouldShowLEDs()) {
    ledsForceEnabled = true;
    Serial.println("Button 1: Force enabling LEDs (no timeout)");
    markSettingsChanged();  // Save the force enable state
  }
  
  if (currentMode == STATIC_COLOR) {
    currentColorIndex = (currentColorIndex + 1) % NUM_COLORS;
    if (currentColorIndex == NUM_COLORS - 1) {
      // After going through all colors, switch to first pattern mode
      currentMode = RANDOM_SCATTER;
      Serial.println("Button 1: Mode changed to RANDOM SCATTER");
    } else {
      Serial.print("Button 1: Color changed to ");
      switch(currentColorIndex) {
        case 0: Serial.println("Red"); break;
        case 1: Serial.println("Green"); break;
      }
    }
  } else {
    // Switch to next pattern mode
    currentMode = (DisplayMode)((int)currentMode + 1);
    if (currentMode > OFF_MODE) {  // Changed from >= to > since we want to include OFF_MODE
      currentMode = STATIC_COLOR;
      currentColorIndex = 0;
      Serial.println("Button 1: Mode reset to STATIC_COLOR (Red)");
    } else {
      Serial.print("Button 1: Mode changed to ");
      switch(currentMode) {
        case RANDOM_SCATTER: Serial.println("RANDOM SCATTER"); break;
        case RAINBOW_MODE: Serial.println("RAINBOW"); break;
        case SNAKE_MODE: Serial.println("SNAKE"); break;
        case RANDOM_BLINK: Serial.println("RANDOM BLINK"); break;
        case CHASE_MODE: Serial.println("CHASE"); break;
        case WAVE_MODE: Serial.println("WAVE"); break;
        case FADE_RANDOM: Serial.println("FADE RANDOM"); break;
        case SPARKLE_MODE: Serial.println("SPARKLE"); break;
        case FIREWORK_MODE: Serial.println("FIREWORK"); break;
        case METEOR_MODE: Serial.println("METEOR"); break;
        case CANDY_CANE_MODE: Serial.println("CANDY CANE"); break;
        default: break;
      }
    }
  }
  updateDisplay();
  // Mark settings as changed when display mode or color changes
  markSettingsChanged();
}

void handleButton1LongPress() {
  // Button 1 LONG PRESS - toggle force enable mode
  ledsForceEnabled = !ledsForceEnabled;
  
  // Reset darkness tracking when manually toggling
  if (!ledsForceEnabled) {
    inDarkness = false;  // Reset darkness state when disabling force mode
  }
  
  // Flash LEDs to indicate mode change
  flashLEDsToIndicateMode();
  
  Serial.print("Button 1 LONG PRESS: Force enable mode ");
  Serial.println(ledsForceEnabled ? "ENABLED (LEDs ignore light sensor)" : "DISABLED (LEDs follow light sensor)");
  
  markSettingsChanged();  // Save the new state
  updateDisplay();
}

void flashLEDsToIndicateMode() {
  // Save current LED state
  CRGB savedLeds[NUM_LEDS];
  for(int i = 0; i < NUM_LEDS; i++) {
    savedLeds[i] = leds[i];
  }
  
  // Flash pattern to indicate mode
  if (ledsForceEnabled) {
    // Force enabled - flash GREEN (always on)
    fill_solid(leds, NUM_LEDS, CRGB::Green);
    FastLED.show();
    delay(200);
    turnOffAllLEDs();
    FastLED.show();
    delay(100);
    fill_solid(leds, NUM_LEDS, CRGB::Green);
    FastLED.show();
    delay(200);
    turnOffAllLEDs();
    FastLED.show();
    delay(100);
  } else {
    // Force disabled - flash RED (light sensor control)
    fill_solid(leds, NUM_LEDS, CRGB::Red);
    FastLED.show();
    delay(200);
    turnOffAllLEDs();
    FastLED.show();
    delay(100);
    fill_solid(leds, NUM_LEDS, CRGB::Red);
    FastLED.show();
    delay(200);
    turnOffAllLEDs();
    FastLED.show();
    delay(100);
  }
  
  // Restore previous LED state
  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = savedLeds[i];
  }
}

void handleButton2Press() {
  // Button 2 controls songs - IMMEDIATE stop/start
  if (songState == PLAYING_SONG) {
    // IMMEDIATELY stop current song
    Serial.print("Button 2: Stopping song - ");
    Serial.println(songNames[currentSong]);
    stopSong();
    
    // Cycle to next song for next time
    currentSong = (ChristmasSong)((int)currentSong + 1);
    if (currentSong >= NUM_CHRISTMAS_SONGS) {
      currentSong = SANTA_CLAUS_IS_COMIN;
    }
    
    Serial.print("Button 2: Next song queued - ");
    Serial.println(songNames[currentSong]);
    // Mark settings as changed when song changes
    markSettingsChanged();
  } else {
    // Start playing current song
    Serial.print("Button 2: Starting song - ");
    Serial.println(songNames[currentSong]);
    startSong();
  }
}

void updateRandomScatter() {
    // Use LDR to influence the probability of changes
    int ldrReading = analogRead(LDR_PIN);
    uint8_t changeProb = map(ldrReading & 0x0F, 0, 15, 1, 4); // 1/1 to 1/4 chance based on light
    
    for(int i = 0; i < NUM_LEDS; i++) {
        if(random8(changeProb) == 0) { // Random chance to change each LED
            leds[i] = random8(2) == 0 ? CRGB::Red : CRGB::Green;
        }
    }
}

void updateFadeRandom() {
    if (needNewFadeTarget) {
        // Save current colors as start point
        for(int i = 0; i < NUM_LEDS; i++) {
            currentFadeColors[i] = leds[i];
        }
        
        // Generate new random target colors using LDR
        int ldrReading = analogRead(LDR_PIN);
        uint8_t pattern = ldrReading & 0x03; // Use bottom 2 bits for pattern selection
        
        for(int i = 0; i < NUM_LEDS; i++) {
            switch(pattern) {
                case 0: // Random red/green
                    targetFadeColors[i] = random8(2) == 0 ? CRGB::Red : CRGB::Green;
                    break;
                case 1: // Alternating
                    targetFadeColors[i] = ((i + fadeProgress/32) % 2) == 0 ? CRGB::Red : CRGB::Green;
                    break;
                case 2: // Groups of 2
                    targetFadeColors[i] = ((i + fadeProgress/32) % 4) < 2 ? CRGB::Red : CRGB::Green;
                    break;
                case 3: // Chase pattern
                    targetFadeColors[i] = ((i + fadeProgress/32) % NUM_LEDS) < NUM_LEDS/2 ? CRGB::Red : CRGB::Green;
                    break;
            }
        }
        needNewFadeTarget = false;
        fadeProgress = 0;
    }
    
    // Update fade progress
    fadeProgress += 4;
    
    // Apply fade between current and target colors
    for(int i = 0; i < NUM_LEDS; i++) {
        leds[i] = blend(currentFadeColors[i], targetFadeColors[i], fadeProgress);
    }
    
    // Check if fade is complete
    if(fadeProgress >= 255) {
        needNewFadeTarget = true;
    }
}

void updateSparklePattern() {
    // Start new sparkles
    if(random8(3) == 0) {  // 1/3 chance to start new sparkle
        uint8_t pos = random8(NUM_LEDS);
        if(sparkleBrightness[pos] == 0) {
            sparklePositions[pos] = pos;
            sparkleBrightness[pos] = 255;
            sparkleColors[pos] = random8(2);  // 0 = red, 1 = green
        }
    }
    
    // Update existing sparkles
    for(int i = 0; i < NUM_LEDS; i++) {
        if(sparkleBrightness[i] > 0) {
            leds[i] = sparkleColors[i] == 0 ? CRGB::Red : CRGB::Green;
            leds[i].nscale8(sparkleBrightness[i]);
            sparkleBrightness[i] = (sparkleBrightness[i] * 3) >> 2; // Fade out
        } else {
            leds[i] = CRGB::Black;
        }
    }
}

void updateFireworkPattern() {
    // Clear all LEDs first
    turnOffAllLEDs();
    
    // Start new firework if none active
    if(currentFirework.position == -1) {
        currentFirework.position = 0;
        currentFirework.phase = 0;
        currentFirework.brightness = 255;
        currentFirework.isRed = random8(2) == 0;
    }
    
    // Update firework
    switch(currentFirework.phase) {
        case 0: // Launch
            leds[currentFirework.position] = currentFirework.isRed ? CRGB::Red : CRGB::Green;
            leds[currentFirework.position].nscale8(currentFirework.brightness);
            currentFirework.position++;
            if(currentFirework.position >= NUM_LEDS/2) {
                currentFirework.phase = 1;
            }
            break;
            
        case 1: // Explode
            for(int i = 0; i < NUM_LEDS; i++) {
                if(random8(2) == 0) {
                    leds[i] = currentFirework.isRed ? CRGB::Red : CRGB::Green;
                    leds[i].nscale8(currentFirework.brightness);
                }
            }
            currentFirework.brightness = (currentFirework.brightness * 7) >> 3;
            if(currentFirework.brightness < 40) {
                currentFirework.phase = 2;
            }
            break;
            
        case 2: // Reset
            currentFirework.position = -1;
            break;
    }
}

void updateMeteorPattern() {
    // Fade all LEDs
    for(int i = 0; i < NUM_LEDS; i++) {
        leds[i].nscale8(192);
    }
    
    // Start new meteor if needed
    if(meteorPos == -1) {
        meteorPos = NUM_LEDS;
        meteorIsRed = !meteorIsRed;  // Alternate colors
        for(int i = 0; i < 3; i++) {
            meteorTail[i] = 255 - (i * 64);  // Decreasing brightness tail
        }
    }
    
    // Update meteor position
    meteorPos--;
    
    // Draw meteor and tail
    for(int i = 0; i < 3; i++) {
        int pos = meteorPos + i;
        if(pos >= 0 && pos < NUM_LEDS) {
            leds[pos] = meteorIsRed ? CRGB::Red : CRGB::Green;
            leds[pos].nscale8(meteorTail[i]);
        }
    }
    
    // Reset when off screen
    if(meteorPos < -3) {
        meteorPos = -1;
    }
}

void updateCandyCanePattern() {
    candyOffset = (candyOffset + 1) % (NUM_LEDS * 2);
    
    for(int i = 0; i < NUM_LEDS; i++) {
        bool isRed = ((i + candyOffset/2) / stripeWidth) % 2 == 0;
        leds[i] = isRed ? CRGB::Red : CRGB::Green;
    }
}

void updateTreePattern() {
    // Update star brightness
    if(treeStarIncreasing) {
        treeStarBrightness = qadd8(treeStarBrightness, 2);
        if(treeStarBrightness >= 255) treeStarIncreasing = false;
    } else {
        treeStarBrightness = qsub8(treeStarBrightness, 2);
        if(treeStarBrightness <= 100) treeStarIncreasing = true;
    }
    
    // Tree pattern phases
    treePhase = (treePhase + 1) % 4;
    
    for(int i = 0; i < NUM_LEDS; i++) {
        if(i == NUM_LEDS-1) {
            // Star on top
            leds[i] = CRGB::Red;
            leds[i].nscale8(treeStarBrightness);
        } else {
            // Tree branches
            bool isRed = ((i + treePhase) % 3) == 0;
            leds[i] = isRed ? CRGB::Red : CRGB::Green;
        }
    }
}

void updatePatterns() {
    unsigned long currentTime = millis();
    
    // Set update interval based on current mode
    switch (currentMode) {
        case RANDOM_SCATTER:
            patternUpdateInterval = SCATTER_SPEED;
            break;
        case SPARKLE_MODE:
            patternUpdateInterval = SPARKLE_SPEED;
            break;
        case FIREWORK_MODE:
            patternUpdateInterval = FIREWORK_SPEED;
            break;
        case METEOR_MODE:
            patternUpdateInterval = METEOR_SPEED;
            break;
        case CANDY_CANE_MODE:
            patternUpdateInterval = CANDY_SPEED;
            break;
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
        case FADE_RANDOM:
            patternUpdateInterval = FADE_SPEED;
            break;
        case RANDOM_BLINK:
            // Uses its own timing
            break;
        default:
            patternUpdateInterval = 50;
            break;
    }
    
    if (currentTime - lastPatternUpdate >= patternUpdateInterval) {
        lastPatternUpdate = currentTime;
        
        switch (currentMode) {
            case STATIC_COLOR:
                // No animation needed
                break;
            case RANDOM_SCATTER:
                updateRandomScatter();
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
            case WAVE_MODE:
                updateWavePattern();
                break;
            case FADE_RANDOM:
                updateFadeRandom();
                break;
            case SPARKLE_MODE:
                updateSparklePattern();
                break;
            case FIREWORK_MODE:
                updateFireworkPattern();
                break;
            case METEOR_MODE:
                updateMeteorPattern();
                break;
            case CANDY_CANE_MODE:
                updateCandyCanePattern();
                break;
            case OFF_MODE:
                turnOffAllLEDs();
                break;
        }
    }
}

void updateRainbowPattern() {
  // Christmas rainbow - cycle between red and green only
  static uint8_t colorStep = 0;
  colorStep++;
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ( ((colorStep + i) % 2) == 0 ) ? CRGB::Red : CRGB::Green;
  }
}

void updateSnakePattern() {
  turnOffAllLEDs();
  
  snakeHeadPos = (snakeHeadPos + 1) % NUM_LEDS;
  
  // Cycle through red and green for snake
  static uint8_t snakeColorIndex = 0;
  if (snakeHeadPos == 0) snakeColorIndex = (snakeColorIndex + 1) % 2;
  CRGB snakeColor = (snakeColorIndex == 0) ? CRGB::Red : CRGB::Green;
  
  for (int i = 0; i < snakeLength; i++) {
    int pos = (snakeHeadPos - i + NUM_LEDS) % NUM_LEDS;
    int brightness = 255 - (i * 255 / snakeLength);
    leds[pos] = snakeColor;
    leds[pos].nscale8(brightness);
  }
}

void updateRandomBlinkPattern(unsigned long currentTime) {
  if (currentTime - lastRandomUpdate >= randomBlinkInterval) {
    lastRandomUpdate = currentTime;
    
    // Turn off old random LEDs
    for (int i = 0; i < 3; i++) {
      if (randomLEDs[i] < NUM_LEDS) {
        leds[randomLEDs[i]] = CRGB::Black;
      }
    }
    
    // Generate new random LEDs with only red and green
    CRGB christmasColors[] = {CRGB::Red, CRGB::Green};
    for (int i = 0; i < 3; i++) {
      randomLEDs[i] = random8(0, NUM_LEDS);
      leds[randomLEDs[i]] = christmasColors[random8(0, 2)];
    }
  }
}

void updateChasePattern() {
  turnOffAllLEDs();
  
  // Cycle through red and green
  static uint8_t chaseColorIndex = 0;
  CRGB chaseColor = (chaseColorIndex == 0) ? CRGB::Red : CRGB::Green;
  leds[chasePos] = chaseColor;
  chasePos = (chasePos + 1) % NUM_LEDS;
  if (chasePos == 0) chaseColorIndex = (chaseColorIndex + 1) % 2;
}

void updateBreathePattern() {
  if (breatheIncreasing) {
    breatheBrightness += 3;
    if (breatheBrightness >= 250) {
      breatheIncreasing = false;
    }
  } else {
    breatheBrightness -= 3;
    if (breatheBrightness <= 5) {
      breatheIncreasing = true;
      breatheHue = (breatheHue + 1) % 2; // Cycle through 2 colors
    }
  }
  CRGB breatheColor = (breatheHue == 0) ? CRGB::Red : CRGB::Green;
  fill_solid(leds, NUM_LEDS, breatheColor);
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].nscale8(breatheBrightness);
  }
}

void updateWavePattern() {
  waveOffset += 10;
  
  for (int i = 0; i < NUM_LEDS; i++) {
    uint8_t sinBrightness = sin8(waveOffset + (i * 255 / NUM_LEDS));
    CRGB waveColor = (i % 2 == 0) ? CRGB::Red : CRGB::Green;
    leds[i] = waveColor;
    leds[i].nscale8(sinBrightness);
  }
}

void turnOffAllLEDs() {
  fill_solid(leds, NUM_LEDS, CRGB::Black);
}

void updateDisplay() {
  if (!shouldShowLEDs()) {
    turnOffAllLEDs();
    return;
  }
  
  turnOffAllLEDs();
  
  // Get new random seed from LDR for pattern initialization
  uint32_t seed = analogRead(LDR_PIN);
  randomSeed(seed);
  
  switch (currentMode) {
    case STATIC_COLOR:
      fill_solid(leds, NUM_LEDS, colorOptions[currentColorIndex]);
      break;
    case RANDOM_SCATTER:
      for(int i = 0; i < NUM_LEDS; i++) {
        leds[i] = random(2) == 0 ? CRGB::Red : CRGB::Green;
      }
      break;
    case SNAKE_MODE:
      snakeHeadPos = 0;
      snakeHue = random8(0, 255);
      break;
    case RANDOM_BLINK:
      for (int i = 0; i < 3; i++) {
        randomLEDs[i] = random8(0, NUM_LEDS);
        randomHues[i] = random8(0, 255);
      }
      break;
    case CHASE_MODE:
      chasePos = 0;
      chaseHue = random8(0, 255);
      break;
    case WAVE_MODE:
      waveOffset = 0;
      waveHue = random8(0, 1); // Only red or green
      break;
    case SPARKLE_MODE:
      memset(sparkleBrightness, 0, sizeof(sparkleBrightness));
      break;
    case FIREWORK_MODE:
      currentFirework.position = -1;
      currentFirework.phase = 0;
      currentFirework.brightness = 255;
      currentFirework.isRed = random8(2) == 0;
      break;
    case METEOR_MODE:
      meteorPos = -1;
      meteorIsRed = true;
      break;
    case CANDY_CANE_MODE:
      candyOffset = 0;
      break;
    case FADE_RANDOM:
      needNewFadeTarget = true;
      fadeProgress = 0;
      break;
    default:
      break;
  }
}

void startSong() {
  songState = PLAYING_SONG;
  songStartTime = millis();
  
  // Get current song data using our new system
  currentSongData = getSongData(currentSong);
  
  Serial.print("Now playing: ");
  Serial.println(songNames[currentSong]);
  
  // Play the song using our playMusic function (non-blocking version needed)
  // We'll implement a non-blocking version in updateSong()
}

void stopSong() {
  songState = IDLE;
  noTone(BUZZER);
  updateDisplay();
  Serial.println("Song stopped");
}

void updateSong() {
  static unsigned long lastNoteTime = 0;
  static uint16_t currentNote = 0;
  static uint16_t currentNoteDuration = 0;
  
  if (songState == PLAYING_SONG) {
    unsigned long currentTime = millis();
    
    // Start new note or finish song
    if (currentTime - lastNoteTime >= currentNoteDuration) {
      if (currentNote >= currentSongData.size * 2) {
        // Song finished
        songState = IDLE;
        noTone(BUZZER);
        currentNote = 0;
        Serial.println("Song finished!");
        
        // Auto-advance to next song
        currentSong = (ChristmasSong)((int)currentSong + 1);
        if (currentSong >= NUM_CHRISTMAS_SONGS) {
          currentSong = SANTA_CLAUS_IS_COMIN;
        }
        Serial.print("Next song queued: ");
        Serial.println(songNames[currentSong]);
        return;
      }
      
      // Calculate duration for new note
      uint16_t wholeNote = (60000 * 4) / currentSongData.baseTempo;
      int8_t noteType = pgm_read_word(&currentSongData.melody[currentNote + 1]);
      if (noteType > 0) {
        currentNoteDuration = wholeNote / noteType;
      } else {
        currentNoteDuration = wholeNote / abs(noteType) * 1.5;
      }
      
      // Play the new note
      tone(BUZZER, pgm_read_word(&currentSongData.melody[currentNote]), currentNoteDuration * 0.9);
      lastNoteTime = currentTime;
      currentNote += 2;
      
      // Debug output for note changes
      Serial.print("Playing note ");
      Serial.print(currentNote/2);
      Serial.print(" of ");
      Serial.println(currentSongData.size);
    }
  }
}

// Function to output sensor data every second
void outputSensorData() {
  // Read current sensor values
  int ldrReading = analogRead(LDR_PIN);
  int battReading = analogRead(BATT_SENSE);
  float battVoltage = (battReading / 4095.0) * 3.3;
  
  Serial.println("\n=== Status Update ===");
  Serial.print("Light Sensor: ");
  Serial.println(ldrReading);

  Serial.print("Power Source: ");
  switch (currentPowerSource) {
    case POWER_USB: 
      Serial.print("USB (Brightness: ");
      break;
    case POWER_AAA: 
      Serial.print("AAA Batteries (Brightness: ");
      break;
  }
  Serial.print(currentBrightness * 100 / 255);
  Serial.println("%)");
  
  Serial.print("Battery Voltage: ");
  Serial.print(battVoltage * 2, 2);  // Display actual voltage (after voltage divider)
  Serial.println("V");
  
  Serial.print("Force Enable: ");
  Serial.println(ledsForceEnabled ? "Yes" : "No");
  
  Serial.print("Animation: ");
  // Declare variables outside switch
  int activeSparkles = 0;
  
  switch(currentMode) {
    case STATIC_COLOR: 
      Serial.print("Static Color (");
      Serial.print(currentColorIndex == 0 ? "Red" : "Green");
      Serial.println(")");
      break;
    case RANDOM_SCATTER: Serial.println("Random Scatter"); break;
    case RAINBOW_MODE: Serial.println("Rainbow"); break;
    case SNAKE_MODE: Serial.println("Snake"); break;
    case RANDOM_BLINK: Serial.println("Random Blink"); break;
    case CHASE_MODE: Serial.println("Chase"); break;
    case WAVE_MODE: Serial.println("Wave"); break;
    case FADE_RANDOM: 
      Serial.print("Fade Random (Progress: ");
      Serial.print(fadeProgress * 100 / 255);
      Serial.println("%)");
      break;
    case SPARKLE_MODE: 
      Serial.print("Sparkle (Active: ");
      // Count active sparkles
      activeSparkles = 0;  // Reset counter
      for(int i = 0; i < NUM_LEDS; i++) {
        if(sparkleBrightness[i] > 0) activeSparkles++;
      }
      Serial.print(activeSparkles);
      Serial.println(")");
      break;
    case FIREWORK_MODE: {
      Serial.print("Firework (Phase: ");
      Serial.print(currentFirework.phase);
      Serial.println(")");
      break;
    }
    case METEOR_MODE: {
      Serial.print("Meteor (Position: ");
      Serial.print(meteorPos);
      Serial.println(")");
      break;
    }
    case CANDY_CANE_MODE: {
      Serial.print("Candy Cane (Offset: ");
      Serial.print(candyOffset);
      Serial.println(")");
      break;
    }
    case OFF_MODE:
      Serial.println("LEDs OFF");
      break;
    default: Serial.println("Unknown"); break;
  }
  
  if (songState == PLAYING_SONG) {
    Serial.print("Playing Song: ");
    Serial.println(songNames[currentSong]);
  } else {
    Serial.print("Next Song: ");
    Serial.println(songNames[currentSong]);
  }
  Serial.println("==================\n");
  Serial.flush();
}

// Helper function to format uptime as string
String getUptimeString() {
    uint64_t totalSeconds = ((uint64_t)totalUptimeHigh << 32) | totalUptimeLow;
    char buffer[50];
    snprintf(buffer, sizeof(buffer), "%llu days %02llu:%02llu:%02llu",
        totalSeconds / 86400,
        (totalSeconds % 86400) / 3600,
        (totalSeconds % 3600) / 60,
        totalSeconds % 60);
    return String(buffer);
}