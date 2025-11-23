#include <Arduino.h>
#include <FastLED.h>
#include <Preferences.h>
#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include "christmas_songs.h"

Preferences preferences;

// Web server and DNS
WebServer server(80);
DNSServer dnsServer;
bool wifiAPEnabled = false;
const byte DNS_PORT = 53;
unsigned long lastClientConnectTime = 0;

// WiFi timeout variables
unsigned long wifiAPStartTime = 0;
const unsigned long WIFI_TIMEOUT = 300000; // 5 minutes in milliseconds
bool wifiTimeoutEnabled = true;

// Hardware Configuration
#define RGB_PIN 3
#define BUZZER 10
#define BUTTON1 4
#define BUTTON2 5
#define BATT_SENSE 0
#define LDR_PIN 2
#define NUM_LEDS 8
#define TOP_LED 7

// Battery voltage thresholds
#define BATT_AAA_MIN 1.5
#define BATT_AAA_MAX 2.75
#define BATT_NO_DETECT 0.5

// Brightness levels
#define BRIGHTNESS_USB 60
#define BRIGHTNESS_BATTERY 60

// Timer Configuration
#define TIMER_ON_DURATION 21600
#define TIMER_CYCLE_DURATION 86400

// LED Array
CRGB leds[NUM_LEDS];

// Button Handling
bool button1State = HIGH;
bool lastButton1State = HIGH;
bool button2State = HIGH;
bool lastButton2State = HIGH;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;
unsigned long button1PressStartTime = 0;
bool button1LongPressDetected = false;
const unsigned long longPressTime = 2000;

// WiFi easter egg
bool bothButtonsPressed = false;
unsigned long bothButtonsPressStart = 0;
const unsigned long bothButtonsHoldTime = 1000;
bool wifiMessageSent = false;

// Power Management
enum PowerSource {
  POWER_USB,
  POWER_AAA
};
PowerSource currentPowerSource = POWER_USB;
int currentBrightness = BRIGHTNESS_USB;

// Settings Management
bool settingsChanged = false;
unsigned long lastSaveTime = 0;
const unsigned long saveInterval = 30000;

// Uptime tracking
uint32_t totalUptimeLow = 0;
uint32_t totalUptimeHigh = 0;
unsigned long lastMillisCheck = 0;

// Timer System
bool timerEnabled = false;
uint32_t cycleStartUptimeLow = 0;
uint32_t cycleStartUptimeHigh = 0;
bool manualOverride = false;

// Mode indicator animation
bool showingModeIndicator = false;
unsigned long modeIndicatorStartTime = 0;
const unsigned long MODE_INDICATOR_DURATION = 2000;
CRGB savedLedsBeforeIndicator[NUM_LEDS];

// Display Mode Enum
enum DisplayMode {
  STATIC_COLOR,
  RANDOM_SCATTER,
  RAINBOW_MODE,
  SNAKE_MODE,
  RANDOM_BLINK,
  CHASE_MODE,
  WAVE_MODE,
  FADE_RANDOM,
  SPARKLE_MODE,
  FIREWORK_MODE,
  METEOR_MODE,
  CANDY_CANE_MODE,
  OFF_MODE
};
DisplayMode currentMode = STATIC_COLOR;

// Color Options
int currentColorIndex = 0;
CRGB colorOptions[] = {
  CRGB::Red,
  CRGB::Green,
  CRGB::Green
};
#define NUM_COLORS (sizeof(colorOptions) / sizeof(colorOptions[0]))

// Pattern Variables
unsigned long lastPatternUpdate = 0;
unsigned long patternUpdateInterval = 50;

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

uint8_t fadeProgress = 0;
CRGB currentFadeColors[NUM_LEDS];
CRGB targetFadeColors[NUM_LEDS];
bool needNewFadeTarget = true;

uint8_t snakeHeadPos = 0;
const uint8_t snakeLength = 3;

uint8_t randomLEDs[3] = {0};
unsigned long lastRandomUpdate = 0;

uint8_t chasePos = 0;
uint8_t waveOffset = 0;

uint8_t sparklePositions[NUM_LEDS];
uint8_t sparkleBrightness[NUM_LEDS];
uint8_t sparkleColors[NUM_LEDS];

struct Firework {
  int8_t position;
  uint8_t phase;
  uint8_t brightness;
  bool isRed;
};
Firework currentFirework = {-1, 0, 0, true};

int8_t meteorPos = -1;
uint8_t meteorTail[3];
bool meteorIsRed = true;

uint8_t candyOffset = 0;
const uint8_t stripeWidth = 2;

// Song State
enum SongState {
  IDLE,
  PLAYING_SONG
};
SongState songState = IDLE;
ChristmasSong currentSong = SANTA_CLAUS_IS_COMIN;
Song currentSongData;

unsigned long lastNoteTime = 0;
uint16_t currentNote = 0;
uint16_t currentNoteDuration = 0;

// Monitoring
unsigned long lastBatteryCheck = 0;
unsigned long lastSensorOutput = 0;
const unsigned long batteryCheckInterval = 10000;

// HTML page for captive portal with controls - Christmas themed with snowflakes
const char htmlPage1[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>Christmas PCB Control</title>
<style>
body {
  margin: 0;
  padding: 20px;
  font-family: 'Arial', sans-serif;
  background: linear-gradient(135deg, #1a2a1a 0%, #0d1f0d 100%);
  color: #fff;
  overflow-x: hidden;
  position: relative;
  min-height: 100vh;
}

/* Snowflakes */
.snowflake {
  position: absolute;
  top: -10px;
  color: #ffffff;
  font-size: 1em;
  user-select: none;
  pointer-events: none;
  animation-name: fall;
  animation-timing-function: linear;
  animation-iteration-count: infinite;
}

@keyframes fall {
  to {
    transform: translateY(100vh) rotate(360deg);
  }
}

.container {
  max-width: 500px;
  margin: 0 auto;
  background: rgba(139, 0, 0, 0.15);
  border-radius: 15px;
  padding: 20px;
  backdrop-filter: blur(10px);
  border: 2px solid #8B0000;
  box-shadow: 0 8px 32px rgba(139, 0, 0, 0.3);
  position: relative;
  z-index: 1;
}

h1 {
  text-align: center;
  font-size: 2.2em;
  margin: 0 0 20px 0;
  color: #ff6b6b;
  text-shadow: 0 2px 4px rgba(0, 0, 0, 0.5);
  background: linear-gradient(45deg, #ff6b6b, #ffd93d);
  -webkit-background-clip: text;
  -webkit-text-fill-color: transparent;
  background-clip: text;
}

.greeting {
  text-align: center;
  font-size: 1.3em;
  margin: 20px 0;
  padding: 15px;
  background: rgba(34, 139, 34, 0.3);
  border-radius: 10px;
  border: 1px solid #228B22;
  color: #90EE90;
  font-weight: bold;
}

.control-group {
  margin: 20px 0;
  padding: 15px;
  background: rgba(34, 139, 34, 0.2);
  border-radius: 10px;
  border: 1px solid #228B22;
}

.control-group label {
  display: block;
  margin-bottom: 8px;
  font-weight: bold;
  color: #ffd93d;
}

select, input[type=range] {
  width: 100%;
  padding: 10px;
  border-radius: 8px;
  border: 2px solid #8B0000;
  background: rgba(255, 255, 255, 0.95);
  color: #2d5016;
  font-size: 16px;
  transition: all 0.3s ease;
}

select:focus, input[type=range]:focus {
  outline: none;
  border-color: #ff6b6b;
  box-shadow: 0 0 10px rgba(255, 107, 107, 0.5);
}

input[type=range] {
  padding: 0;
  height: 25px;
  background: linear-gradient(to right, #228B22, #ff6b6b);
}

input[type=range]::-webkit-slider-thumb {
  appearance: none;
  width: 25px;
  height: 25px;
  border-radius: 50%;
  background: #ffd93d;
  border: 2px solid #8B0000;
  cursor: pointer;
}

.value-display {
  text-align: center;
  font-size: 1.2em;
  margin-top: 5px;
  color: #ffd93d;
  font-weight: bold;
}

button {
  width: 100%;
  padding: 14px;
  margin: 10px 0;
  border: none;
  border-radius: 10px;
  font-size: 16px;
  font-weight: bold;
  cursor: pointer;
  transition: all 0.3s ease;
  text-transform: uppercase;
  letter-spacing: 1px;
}

button:hover {
  transform: translateY(-2px);
  box-shadow: 0 5px 15px rgba(0, 0, 0, 0.3);
}

button:active {
  transform: translateY(0);
}

.btn-apply {
  background: linear-gradient(45deg, #228B22, #32CD32);
  color: white;
  border: 2px solid #006400;
}

.btn-song {
  background: linear-gradient(45deg, #8B0000, #FF0000);
  color: white;
  border: 2px solid #800000;
}

.status {
  text-align: center;
  padding: 12px;
  margin: 10px 0;
  background: rgba(255, 215, 0, 0.2);
  border-radius: 8px;
  font-size: 0.9em;
  border: 1px solid #FFD700;
  color: #FFD700;
}

.christmas-header {
  text-align: center;
  font-size: 3em;
  margin-bottom: 10px;
  animation: glow 2s ease-in-out infinite alternate;
}

@keyframes glow {
  from {
    text-shadow: 0 0 10px #ff0000, 0 0 20px #ff0000, 0 0 30px #ff6b6b;
  }
  to {
    text-shadow: 0 0 20px #00ff00, 0 0 30px #00ff00, 0 0 40px #90ee90;
  }
}

.ornament {
  display: inline-block;
  margin: 0 10px;
  animation: bounce 2s infinite;
}

@keyframes bounce {
  0%, 100% { transform: translateY(0); }
  50% { transform: translateY(-10px); }
}

.battery-info {
  text-align: center;
  padding: 10px;
  margin: 10px 0;
  background: rgba(255, 255, 255, 0.1);
  border-radius: 8px;
  border: 1px solid #ffd93d;
  color: #ffd93d;
  font-size: 0.9em;
}
</style>
</head>
<body>
<div id="snowflakes"></div>

<div class="container">
  <div class="christmas-header">
    Christmas BALL PCB
  </div>
  
  <h1>Control Panel</h1>
  <div class="greeting">Fijne kerst Casper! 🎅</div>

  <div class="control-group">
    <label>✨ Brightness</label>
    <input type="range" id="brightness" min="10" max="255" value="60" oninput="updateBrightness(this.value)">
    <div class="value-display" id="brightnessVal">60</div>
  </div>

  <div class="control-group">
    <label>🎨 LED Pattern</label>
    <select id="pattern">
)rawliteral";

const char htmlPage2[] PROGMEM = R"rawliteral(
      <option value="0">Static Red</option>
      <option value="1">Static Green</option>
      <option value="2">Random Scatter</option>
      <option value="3">Rainbow</option>
      <option value="4">Snake</option>
      <option value="5">Random Blink</option>
      <option value="6">Chase</option>
      <option value="7">Wave</option>
      <option value="8">Fade Random</option>
      <option value="9">Sparkle</option>
      <option value="10">Firework</option>
      <option value="11">Meteor</option>
      <option value="12">Candy Cane</option>
      <option value="13">Off</option>
    </select>
  </div>

  <div class="control-group">
    <label>⏰ 6-Hours-ON Timer</label>
    <select id="timer">
      <option value="0">Disabled</option>
      <option value="1">Enabled</option>
    </select>
  </div>

  <button class="btn-apply" onclick="applySettings()">Apply Settings</button>

  <div class="control-group">
    <label>🎵 Play Christmas Song</label>
    <select id="song">
      <option value="0">Santa Claus Is Coming</option>
      <option value="1">Jingle Bells</option>
      <option value="2">We Wish You Merry Xmas</option>
      <option value="3">Silent Night</option>
      <option value="4">Rudolph</option>
      <option value="5">O Christmas Tree</option>
      <option value="6">O Come All Ye Faithful</option>
      <option value="7">O Little Town</option>
      <option value="8">The First Noel</option>
      <option value="9">We Three Kings</option>
      <option value="10">White Christmas</option>
      <option value="11">Away in a Manger</option>
      <option value="12">Carol of the Bells</option>
      <option value="13">Deck the Halls</option>
      <option value="14">God Rest Ye</option>
      <option value="15">Go Tell It</option>
      <option value="16">Hark the Herald</option>
      <option value="17">Joy to the World</option>
    </select>
    <button class="btn-song" onclick="playSong()">Play Song 🎶</button>
    <button class="btn-song" onclick="stopSong()">Stop Song ⏹️</button>
  </div>

  <div class="status" id="status">Ready to spread Christmas cheer! 🎄</div>
</div>

<script>
// Snowflake animation
function createSnowflakes() {
  const snowflakesContainer = document.getElementById('snowflakes');
  const snowflakes = ['❄', '❅', '❆', '•'];
  
  for (let i = 0; i < 50; i++) {
    const snowflake = document.createElement('div');
    snowflake.className = 'snowflake';
    snowflake.textContent = snowflakes[Math.floor(Math.random() * snowflakes.length)];
    snowflake.style.left = Math.random() * 100 + 'vw';
    snowflake.style.animationDuration = (Math.random() * 5 + 5) + 's';
    snowflake.style.animationDelay = Math.random() * 5 + 's';
    snowflake.style.opacity = Math.random() * 0.7 + 0.3;
    snowflake.style.fontSize = (Math.random() * 10 + 10) + 'px';
    snowflakesContainer.appendChild(snowflake);
  }
}

function updateBrightness(val) {
  document.getElementById('brightnessVal').innerText = val;
}

function applySettings() {
  var b = document.getElementById('brightness').value;
  var p = document.getElementById('pattern').value;
  var t = document.getElementById('timer').value;
  
  fetch('/set?brightness=' + b + '&pattern=' + p + '&timer=' + t)
    .then(r => r.text())
    .then(d => {
      document.getElementById('status').innerText = d + ' 🎄';
    })
    .catch(e => {
      document.getElementById('status').innerText = 'Error! ❌';
    });
}

function playSong() {
  var s = document.getElementById('song').value;
  fetch('/play?song=' + s)
    .then(r => r.text())
    .then(d => {
      document.getElementById('status').innerText = d + ' 🎵';
    })
    .catch(e => {
      document.getElementById('status').innerText = 'Error! ❌';
    });
}

function stopSong() {
  fetch('/stop')
    .then(r => r.text())
    .then(d => {
      document.getElementById('status').innerText = d + ' ⏹️';
    })
    .catch(e => {
      document.getElementById('status').innerText = 'Error! ❌';
    });
}

// Initialize snowflakes when page loads
window.onload = createSnowflakes;
</script>
</body>
</html>
)rawliteral";

// Function prototypes
void markSettingsChanged();
void saveToMemory();
void loadSettings();
void checkPowerSource();
void startWiFiAP();
void stopWiFiAP();
void handleRoot();
void handleNotFound();
uint64_t getTotalUptimeSeconds();
uint32_t getElapsedCycleSeconds();
bool isInOnPhase();
void activateTimer();
void deactivateTimer();
void updateTimerState();
void showModeIndicator();
void updateModeIndicator();
bool shouldShowLEDs();
void printPowerStatus();
void outputSensorData();
void handleButton1LongPress();
void checkButtons();
void handleButton1Press();
void handleButton2Press();
void handleBothButtonsPress();
void updatePatterns();
void updateDisplay();
void startSong();
void stopSong();
void updateSong();
void turnOffAllLEDs();
void checkWiFiTimeout();

void WiFiStationConnected(WiFiEvent_t event, WiFiEventInfo_t info) {
  Serial.println("Client connected to AP!");
  lastClientConnectTime = millis();
}

void WiFiStationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info) {
  Serial.println("Client disconnected from AP");
}

void checkWiFiTimeout() {
  if (wifiAPEnabled && wifiTimeoutEnabled) {
    unsigned long currentTime = millis();
    
    // Check if WiFi has been active for more than 5 minutes
    if (currentTime - wifiAPStartTime >= WIFI_TIMEOUT) {
      Serial.println("WiFi timeout reached - stopping AP to save battery");
      stopWiFiAP();
    }
    
    // Optional: Also stop if no clients connected for 2 minutes
    if (WiFi.softAPgetStationNum() == 0) {
      if (currentTime - lastClientConnectTime >= 120000 && lastClientConnectTime > 0) {
        Serial.println("No clients connected for 2 minutes - stopping AP");
        stopWiFiAP();
      }
    }
  }
}

void handleRoot() {
  String html = String(FPSTR(htmlPage1)) + String(FPSTR(htmlPage2));
  server.send(200, "text/html", html);
}

void handleSet() {
  if (server.hasArg("brightness")) {
    int brightness = server.arg("brightness").toInt();
    if (brightness >= 10 && brightness <= 255) {
      currentBrightness = brightness;
      FastLED.setBrightness(currentBrightness);
      Serial.print("Brightness set to: ");
      Serial.println(brightness);
    }
  }
  
  if (server.hasArg("pattern")) {
    int pattern = server.arg("pattern").toInt();
    if (pattern >= 0 && pattern <= 13) {
      if (pattern <= 1) {
        currentMode = STATIC_COLOR;
        currentColorIndex = pattern;
      } else {
        currentMode = (DisplayMode)(pattern - 1);
      }
      updateDisplay();
      Serial.print("Pattern set to: ");
      Serial.println(pattern);
    }
  }
  
  if (server.hasArg("timer")) {
    int timer = server.arg("timer").toInt();
    if (timer == 0 && timerEnabled) {
      deactivateTimer();
    } else if (timer == 1 && !timerEnabled) {
      activateTimer();
    }
  }
  
  markSettingsChanged();
  server.send(200, "text/plain", "Settings applied!");
}

void handlePlay() {
  if (server.hasArg("song")) {
    int songIndex = server.arg("song").toInt();
    if (songIndex >= 0 && songIndex < NUM_CHRISTMAS_SONGS) {
      currentSong = (ChristmasSong)songIndex;
      startSong();
      server.send(200, "text/plain", "Playing: " + String(songNames[songIndex]));
      Serial.print("Playing song: ");
      Serial.println(songNames[songIndex]);
      return;
    }
  }
  server.send(400, "text/plain", "Invalid song");
}

void handleStop() {
  if (songState == PLAYING_SONG) {
    stopSong();
    server.send(200, "text/plain", "Song stopped");
    Serial.println("Song stopped via web");
  } else {
    server.send(200, "text/plain", "No song playing");
  }
}

void handleNotFound() {
  // Redirect all requests to root for captive portal
  server.sendHeader("Location", "http://192.168.4.1", true);
  server.send(302, "text/plain", "");
}

void startWiFiAP() {
  if (!wifiAPEnabled) {
    Serial.println("Starting WiFi AP...");
    
    // Register WiFi event handlers
    WiFi.onEvent(WiFiStationConnected, ARDUINO_EVENT_WIFI_AP_STACONNECTED);
    WiFi.onEvent(WiFiStationDisconnected, ARDUINO_EVENT_WIFI_AP_STADISCONNECTED);
    
    WiFi.mode(WIFI_AP);
    WiFi.softAP("Kerstbal_Casper");
    
    // Configure AP with specific IP for better captive portal detection
    IPAddress local_IP(192,168,4,1);
    IPAddress gateway(192,168,4,1);
    IPAddress subnet(255,255,255,0);
    WiFi.softAPConfig(local_IP, gateway, subnet);
    
    delay(100);
    
    // Start DNS server for captive portal
    dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());
    
    // Setup web server with control endpoints
    server.on("/", handleRoot);
    server.on("/set", handleSet);           // Settings control
    server.on("/play", handlePlay);         // Play song
    server.on("/stop", handleStop);         // Stop song
    server.on("/generate_204", handleRoot);  // Android captive portal check
    server.on("/fwlink", handleRoot);         // Microsoft captive portal check
    server.on("/hotspot-detect.html", handleRoot); // iOS/macOS captive portal
    server.on("/canonical.html", handleRoot); // Ubuntu
    server.on("/success.txt", handleRoot);    // Firefox
    server.on("/connecttest.txt", handleRoot); // Windows
    server.onNotFound(handleNotFound);
    server.begin();
    
    wifiAPEnabled = true;
    wifiAPStartTime = millis();
    
    Serial.println("WiFi AP Started!");
    Serial.println("Connect to: Kerstgroet_Joel");
    Serial.print("Then open browser to: http://");
    Serial.println(WiFi.softAPIP());
    Serial.println("Or just navigate to any website - it should redirect!");
  }
}

void stopWiFiAP() {
  if (wifiAPEnabled) {
    server.stop();
    dnsServer.stop();
    WiFi.softAPdisconnect(true);
    WiFi.mode(WIFI_OFF);
    wifiAPEnabled = false;
    Serial.println("WiFi AP Stopped");
  }
}

void markSettingsChanged() {
  settingsChanged = true;
}

void saveToMemory() {
  unsigned long currentMillis = millis();
  
  if (currentMillis < lastSaveTime) {
    lastSaveTime = currentMillis;
  }
  
  if (currentMillis - lastSaveTime >= saveInterval && settingsChanged) {
    preferences.begin("xmas-pcb", false);
    
    preferences.putUChar("displayMode", (uint8_t)currentMode);
    preferences.putUChar("colorIndex", currentColorIndex);
    preferences.putUChar("songIndex", (uint8_t)currentSong);
    preferences.putULong("uptimeLow", totalUptimeLow);
    preferences.putULong("uptimeHigh", totalUptimeHigh);
    
    preferences.putBool("timerEnabled", timerEnabled);
    preferences.putULong("cycleStartLow", cycleStartUptimeLow);
    preferences.putULong("cycleStartHigh", cycleStartUptimeHigh);
    
    preferences.end();
    
    lastSaveTime = currentMillis;
    settingsChanged = false;
    Serial.println("Settings saved");
  }
}

void loadSettings() {
  preferences.begin("xmas-pcb", true);
  
  currentMode = (DisplayMode)preferences.getUChar("displayMode", STATIC_COLOR);
  currentColorIndex = preferences.getUChar("colorIndex", 0);
  currentSong = (ChristmasSong)preferences.getUChar("songIndex", SANTA_CLAUS_IS_COMIN);
  totalUptimeLow = preferences.getULong("uptimeLow", 0);
  totalUptimeHigh = preferences.getULong("uptimeHigh", 0);
  
  timerEnabled = preferences.getBool("timerEnabled", false);
  cycleStartUptimeLow = preferences.getULong("cycleStartLow", 0);
  cycleStartUptimeHigh = preferences.getULong("cycleStartHigh", 0);
  
  preferences.end();
  
  lastMillisCheck = millis();
  
  Serial.println("Settings loaded");
  Serial.print("Timer Mode: ");
  Serial.println(timerEnabled ? "ENABLED" : "DISABLED");
  
  if (timerEnabled) {
    uint32_t elapsed = getElapsedCycleSeconds();
    Serial.print("Cycle elapsed: ");
    Serial.print(elapsed / 3600);
    Serial.print("h ");
    Serial.print((elapsed % 3600) / 60);
    Serial.println("m");
    Serial.print("Currently: ");
    Serial.println(isInOnPhase() ? "ON phase" : "OFF phase");
  }
}

void checkPowerSource() {
  int reading = analogRead(BATT_SENSE);
  float voltage = (reading / 4095.0) * 3.3;
  
  // Print battery voltage to terminal
  Serial.print("Battery voltage: ");
  Serial.print(voltage);
  Serial.print("V - ");
  
  if (voltage < BATT_NO_DETECT) {
    currentPowerSource = POWER_USB;
    currentBrightness = BRIGHTNESS_USB;
    Serial.println("USB Power");
  } else if (voltage >= BATT_AAA_MIN && voltage <= BATT_AAA_MAX) {
    currentPowerSource = POWER_AAA;
    currentBrightness = BRIGHTNESS_BATTERY;
    
    // Calculate battery percentage (rough estimate for AAA)
    int batteryPercent = map((int)(voltage * 100), (int)(BATT_AAA_MIN * 100), (int)(BATT_AAA_MAX * 100), 0, 100);
    batteryPercent = constrain(batteryPercent, 0, 100);
    
    Serial.print("Battery Power (");
    Serial.print(batteryPercent);
    Serial.println("%)");
    
    // Auto-disable WiFi if battery voltage is getting low
    if (voltage < (BATT_AAA_MIN + 0.2) && wifiAPEnabled) { // 1.7V threshold
      Serial.println("Battery voltage low - disabling WiFi to conserve power");
      stopWiFiAP();
    }
  } else {
    currentPowerSource = POWER_AAA;
    currentBrightness = BRIGHTNESS_BATTERY;
    Serial.println("Battery Power (unknown level)");
  }
  
  FastLED.setBrightness(currentBrightness);
}

uint64_t getTotalUptimeSeconds() {
  return ((uint64_t)totalUptimeHigh << 32) | totalUptimeLow;
}

uint32_t getElapsedCycleSeconds() {
  uint64_t currentUptime = getTotalUptimeSeconds();
  uint64_t cycleStart = ((uint64_t)cycleStartUptimeHigh << 32) | cycleStartUptimeLow;
  
  uint64_t elapsed = currentUptime - cycleStart;
  
  if (elapsed >= TIMER_CYCLE_DURATION) {
    elapsed = elapsed % TIMER_CYCLE_DURATION;
    
    cycleStartUptimeLow = totalUptimeLow - (uint32_t)elapsed;
    cycleStartUptimeHigh = totalUptimeHigh;
    if (cycleStartUptimeLow > totalUptimeLow) {
      cycleStartUptimeHigh--;
    }
    markSettingsChanged();
  }
  
  return (uint32_t)elapsed;
}

bool isInOnPhase() {
  if (!timerEnabled) return true;
  
  uint32_t elapsed = getElapsedCycleSeconds();
  return elapsed < TIMER_ON_DURATION;
}

void activateTimer() {
  timerEnabled = true;
  
  cycleStartUptimeLow = totalUptimeLow;
  cycleStartUptimeHigh = totalUptimeHigh;
  
  manualOverride = false;
  
  Serial.println("Timer ACTIVATED - 6h ON / 18h OFF cycle started");
  Serial.println("LEDs will be ON for next 6 hours");
  
  markSettingsChanged();
}

void deactivateTimer() {
  timerEnabled = false;
  manualOverride = false;
  
  Serial.println("Timer DEACTIVATED - Manual control restored");
  
  markSettingsChanged();
}

void updateTimerState() {
  if (!timerEnabled) return;
  
  if (manualOverride && isInOnPhase()) {
    manualOverride = false;
    Serial.println("Manual override cleared - back to timer schedule");
  }
}

void showModeIndicator() {
  for(int i = 0; i < NUM_LEDS; i++) {
    savedLedsBeforeIndicator[i] = leds[i];
  }
  
  showingModeIndicator = true;
  modeIndicatorStartTime = millis();
  
  Serial.print("Mode indicator: Timer ");
  Serial.println(timerEnabled ? "ENABLED" : "DISABLED");
}

void updateModeIndicator() {
  if (!showingModeIndicator) return;
  
  unsigned long elapsed = millis() - modeIndicatorStartTime;
  
  if (elapsed >= MODE_INDICATOR_DURATION) {
    showingModeIndicator = false;
    for(int i = 0; i < NUM_LEDS; i++) {
      leds[i] = savedLedsBeforeIndicator[i];
    }
    return;
  }
  
  for(int i = 0; i < NUM_LEDS - 1; i++) {
    leds[i] = CRGB::Black;
  }
  
  float progress = (float)elapsed / MODE_INDICATOR_DURATION;
  
  if (timerEnabled) {
    uint8_t brightness = (uint8_t)(progress * 255);
    leds[TOP_LED] = CRGB::Green;
    leds[TOP_LED].nscale8(brightness);
  } else {
    uint8_t brightness = (uint8_t)((1.0 - progress) * 255);
    leds[TOP_LED] = CRGB::Red;
    leds[TOP_LED].nscale8(brightness);
  }
}

bool shouldShowLEDs() {
  if (showingModeIndicator) return true;
  if (songState == PLAYING_SONG) return true;
  if (!timerEnabled) return true;
  if (manualOverride) return true;
  return isInOnPhase();
}

void printPowerStatus() {
  Serial.print("Power: ");
  Serial.print(currentPowerSource == POWER_USB ? "USB" : "Battery");
  Serial.print(", Brightness: ");
  Serial.print((currentBrightness * 100) / 255);
  Serial.println("%");
}

void checkButtons() {
  bool reading1 = digitalRead(BUTTON1);
  bool reading2 = digitalRead(BUTTON2);

  if (reading1 != lastButton1State || reading2 != lastButton2State) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading1 == LOW && reading2 == LOW) {
      if (!bothButtonsPressed) {
        bothButtonsPressed = true;
        bothButtonsPressStart = millis();
        wifiMessageSent = false;
      } else if (!wifiMessageSent && (millis() - bothButtonsPressStart) >= bothButtonsHoldTime) {
        handleBothButtonsPress();
        wifiMessageSent = true;
      }
    } else {
      if (bothButtonsPressed) {
        bothButtonsPressed = false;
        wifiMessageSent = false;
      }
      
      if (reading1 != button1State) {
        button1State = reading1;
        
        if (button1State == LOW) {
          button1PressStartTime = millis();
          button1LongPressDetected = false;
        } else {
          unsigned long pressDuration = millis() - button1PressStartTime;
          
          if (!button1LongPressDetected && pressDuration < longPressTime) {
            handleButton1Press();
          }
          
          button1LongPressDetected = false;
        }
      }
      
      if (button1State == LOW && !button1LongPressDetected && 
          (millis() - button1PressStartTime) >= longPressTime) {
        button1LongPressDetected = true;
        handleButton1LongPress();
      }

      if (reading2 != button2State) {
        button2State = reading2;
        if (button2State == LOW) {
          handleButton2Press();
        }
      }
    }
  }

  lastButton1State = reading1;
  lastButton2State = reading2;
}

void handleBothButtonsPress() {
  Serial.println("Both buttons held - Starting WiFi AP!");
  
  startWiFiAP();
  
  // Visual feedback
  for(int i = 0; i < 3; i++) {
    fill_solid(leds, NUM_LEDS, CRGB::Yellow1);
    FastLED.show();
    delay(200);
    turnOffAllLEDs();
    FastLED.show();
    delay(200);
  }
  
  updateDisplay();
}

void handleButton1Press() {
  if (timerEnabled && !isInOnPhase() && !manualOverride) {
    manualOverride = true;
    Serial.println("Manual override - LEDs ON (timer still active)");
  }
  
  if (currentMode == STATIC_COLOR) {
    currentColorIndex = (currentColorIndex + 1) % NUM_COLORS;
    if (currentColorIndex == NUM_COLORS - 1) {
      currentMode = RANDOM_SCATTER;
      Serial.println("Mode: RANDOM SCATTER");
    } else {
      Serial.print("Color: ");
      Serial.println(currentColorIndex == 0 ? "Red" : "Green");
    }
  } else {
    currentMode = (DisplayMode)((int)currentMode + 1);
    if (currentMode > OFF_MODE) {
      currentMode = STATIC_COLOR;
      currentColorIndex = 0;
      Serial.println("Mode: STATIC RED");
    } else {
      Serial.print("Mode: ");
      switch(currentMode) {
        case RANDOM_SCATTER: Serial.println("SCATTER"); break;
        case RAINBOW_MODE: Serial.println("RAINBOW"); break;
        case SNAKE_MODE: Serial.println("SNAKE"); break;
        case RANDOM_BLINK: Serial.println("BLINK"); break;
        case CHASE_MODE: Serial.println("CHASE"); break;
        case WAVE_MODE: Serial.println("WAVE"); break;
        case FADE_RANDOM: Serial.println("FADE"); break;
        case SPARKLE_MODE: Serial.println("SPARKLE"); break;
        case FIREWORK_MODE: Serial.println("FIREWORK"); break;
        case METEOR_MODE: Serial.println("METEOR"); break;
        case CANDY_CANE_MODE: Serial.println("CANDY"); break;
        case OFF_MODE: Serial.println("OFF"); break;
        default: break;
      }
    }
  }
  updateDisplay();
  markSettingsChanged();
}

void handleButton1LongPress() {
  if (timerEnabled) {
    deactivateTimer();
  } else {
    activateTimer();
  }
  
  showModeIndicator();
  updateDisplay();
}

void handleButton2Press() {
  if (songState == PLAYING_SONG) {
    Serial.println("Stop song");
    stopSong();
    
    currentSong = (ChristmasSong)((int)currentSong + 1);
    if (currentSong >= NUM_CHRISTMAS_SONGS) {
      currentSong = SANTA_CLAUS_IS_COMIN;
    }
    markSettingsChanged();
  } else {
    Serial.println("Play song");
    startSong();
  }
}

void updateRandomScatter() {
  for(int i = 0; i < NUM_LEDS; i++) {
    if(random8(4) == 0) {
      leds[i] = random8(2) == 0 ? CRGB::Red : CRGB::Green;
    }
  }
}

void updateFadeRandom() {
  if (needNewFadeTarget) {
    for(int i = 0; i < NUM_LEDS; i++) {
      currentFadeColors[i] = leds[i];
      targetFadeColors[i] = random8(2) == 0 ? CRGB::Red : CRGB::Green;
    }
    needNewFadeTarget = false;
    fadeProgress = 0;
  }
  
  fadeProgress += 4;
  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = blend(currentFadeColors[i], targetFadeColors[i], fadeProgress);
  }
  
  if(fadeProgress >= 255) {
    needNewFadeTarget = true;
  }
}

void updateSparklePattern() {
  if(random8(3) == 0) {
    uint8_t pos = random8(NUM_LEDS);
    if(sparkleBrightness[pos] == 0) {
      sparklePositions[pos] = pos;
      sparkleBrightness[pos] = 255;
      sparkleColors[pos] = random8(2);
    }
  }
  
  for(int i = 0; i < NUM_LEDS; i++) {
    if(sparkleBrightness[i] > 0) {
      leds[i] = sparkleColors[i] == 0 ? CRGB::Red : CRGB::Green;
      leds[i].nscale8(sparkleBrightness[i]);
      sparkleBrightness[i] = (sparkleBrightness[i] * 3) >> 2;
    } else {
      leds[i] = CRGB::Black;
    }
  }
}

void updateFireworkPattern() {
  turnOffAllLEDs();
  
  if(currentFirework.position == -1) {
    currentFirework.position = 0;
    currentFirework.phase = 0;
    currentFirework.brightness = 255;
    currentFirework.isRed = random8(2) == 0;
  }
  
  switch(currentFirework.phase) {
    case 0:
      leds[currentFirework.position] = currentFirework.isRed ? CRGB::Red : CRGB::Green;
      leds[currentFirework.position].nscale8(currentFirework.brightness);
      currentFirework.position++;
      if(currentFirework.position >= NUM_LEDS/2) {
        currentFirework.phase = 1;
      }
      break;
      
    case 1:
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
      
    case 2:
      currentFirework.position = -1;
      break;
  }
}

void updateMeteorPattern() {
  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i].nscale8(192);
  }
  
  if(meteorPos == -1) {
    meteorPos = NUM_LEDS;
    meteorIsRed = !meteorIsRed;
    for(int i = 0; i < 3; i++) {
      meteorTail[i] = 255 - (i * 64);
    }
  }
  
  meteorPos--;
  
  for(int i = 0; i < 3; i++) {
    int pos = meteorPos + i;
    if(pos >= 0 && pos < NUM_LEDS) {
      leds[pos] = meteorIsRed ? CRGB::Red : CRGB::Green;
      leds[pos].nscale8(meteorTail[i]);
    }
  }
  
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

void updateRainbowPattern() {
  static uint8_t colorStep = 0;
  colorStep++;
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = (((colorStep + i) % 2) == 0) ? CRGB::Red : CRGB::Green;
  }
}

void updateSnakePattern() {
  turnOffAllLEDs();
  snakeHeadPos = (snakeHeadPos + 1) % NUM_LEDS;
  
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
  if (currentTime - lastRandomUpdate >= 600) {
    lastRandomUpdate = currentTime;
    
    for (int i = 0; i < 3; i++) {
      if (randomLEDs[i] < NUM_LEDS) {
        leds[randomLEDs[i]] = CRGB::Black;
      }
    }
    
    for (int i = 0; i < 3; i++) {
      randomLEDs[i] = random8(NUM_LEDS);
      leds[randomLEDs[i]] = random8(2) == 0 ? CRGB::Red : CRGB::Green;
    }
  }
}

void updateChasePattern() {
  turnOffAllLEDs();
  static uint8_t chaseColorIndex = 0;
  CRGB chaseColor = (chaseColorIndex == 0) ? CRGB::Red : CRGB::Green;
  leds[chasePos] = chaseColor;
  chasePos = (chasePos + 1) % NUM_LEDS;
  if (chasePos == 0) chaseColorIndex = (chaseColorIndex + 1) % 2;
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

void updatePatterns() {
  unsigned long currentTime = millis();
  
  if (showingModeIndicator) {
    updateModeIndicator();
    return;
  }
  
  if (!shouldShowLEDs()) {
    turnOffAllLEDs();
    return;
  }
  
  switch (currentMode) {
    case RANDOM_SCATTER: patternUpdateInterval = SCATTER_SPEED; break;
    case SPARKLE_MODE: patternUpdateInterval = SPARKLE_SPEED; break;
    case FIREWORK_MODE: patternUpdateInterval = FIREWORK_SPEED; break;
    case METEOR_MODE: patternUpdateInterval = METEOR_SPEED; break;
    case CANDY_CANE_MODE: patternUpdateInterval = CANDY_SPEED; break;
    case RAINBOW_MODE: patternUpdateInterval = RAINBOW_SPEED; break;
    case SNAKE_MODE: patternUpdateInterval = SNAKE_SPEED; break;
    case CHASE_MODE: patternUpdateInterval = CHASE_SPEED; break;
    case WAVE_MODE: patternUpdateInterval = WAVE_SPEED; break;
    case FADE_RANDOM: patternUpdateInterval = FADE_SPEED; break;
    default: patternUpdateInterval = 50; break;
  }
  
  if (currentTime - lastPatternUpdate >= patternUpdateInterval) {
    lastPatternUpdate = currentTime;
    
    switch (currentMode) {
      case STATIC_COLOR:
        fill_solid(leds, NUM_LEDS, colorOptions[currentColorIndex]);
        break;
      case RANDOM_SCATTER: updateRandomScatter(); break;
      case RAINBOW_MODE: updateRainbowPattern(); break;
      case SNAKE_MODE: updateSnakePattern(); break;
      case RANDOM_BLINK: updateRandomBlinkPattern(currentTime); break;
      case CHASE_MODE: updateChasePattern(); break;
      case WAVE_MODE: updateWavePattern(); break;
      case FADE_RANDOM: updateFadeRandom(); break;
      case SPARKLE_MODE: updateSparklePattern(); break;
      case FIREWORK_MODE: updateFireworkPattern(); break;
      case METEOR_MODE: updateMeteorPattern(); break;
      case CANDY_CANE_MODE: updateCandyCanePattern(); break;
      case OFF_MODE: turnOffAllLEDs(); break;
    }
  }
}

void updateDisplay() {
  if (!shouldShowLEDs()) {
    turnOffAllLEDs();
    return;
  }
  
  turnOffAllLEDs();
  
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
    case SPARKLE_MODE:
      memset(sparkleBrightness, 0, sizeof(sparkleBrightness));
      break;
    case FIREWORK_MODE:
      currentFirework.position = -1;
      break;
    case METEOR_MODE:
      meteorPos = -1;
      break;
    case FADE_RANDOM:
      needNewFadeTarget = true;
      break;
    default:
      break;
  }
}

void startSong() {
  songState = PLAYING_SONG;
  currentSongData = getSongData(currentSong);
  Serial.print("Playing: ");
  Serial.println(songNames[currentSong]);
}

void stopSong() {
  songState = IDLE;
  noTone(BUZZER);
  updateDisplay();
}

void updateSong() {
  if (songState == PLAYING_SONG) {
    unsigned long currentTime = millis();
    
    if (currentTime - lastNoteTime >= currentNoteDuration) {
      if (currentNote >= currentSongData.size * 2) {
        songState = IDLE;
        noTone(BUZZER);
        currentNote = 0;
        
        currentSong = (ChristmasSong)((int)currentSong + 1);
        if (currentSong >= NUM_CHRISTMAS_SONGS) {
          currentSong = SANTA_CLAUS_IS_COMIN;
        }
        return;
      }
      
      uint16_t wholeNote = (60000 * 4) / currentSongData.baseTempo;
      int8_t noteType = pgm_read_word(&currentSongData.melody[currentNote + 1]);
      if (noteType > 0) {
        currentNoteDuration = wholeNote / noteType;
      } else {
        currentNoteDuration = wholeNote / abs(noteType) * 1.5;
      }
      
      tone(BUZZER, pgm_read_word(&currentSongData.melody[currentNote]), currentNoteDuration * 0.9);
      lastNoteTime = currentTime;
      currentNote += 2;
    }
  }
}

void outputSensorData() {
  Serial.println("\n=== Status ===");
  
  if (timerEnabled) {
    uint32_t elapsed = getElapsedCycleSeconds();
    uint32_t remaining;
    
    if (isInOnPhase()) {
      remaining = TIMER_ON_DURATION - elapsed;
      Serial.print("Timer: ON phase - ");
      Serial.print(remaining / 3600);
      Serial.print("h ");
      Serial.print((remaining % 3600) / 60);
      Serial.println("m remaining");
    } else {
      remaining = TIMER_CYCLE_DURATION - elapsed;
      Serial.print("Timer: OFF phase - ");
      Serial.print(remaining / 3600);
      Serial.print("h ");
      Serial.print((remaining % 3600) / 60);
      Serial.println("m until ON");
    }
    
    if (manualOverride) {
      Serial.println("Manual Override: ACTIVE");
    }
  } else {
    Serial.println("Timer: DISABLED");
  }
  
  Serial.print("LEDs: ");
  Serial.println(shouldShowLEDs() ? "ON" : "OFF");
  
  Serial.print("Power: ");
  Serial.println(currentPowerSource == POWER_USB ? "USB" : "Battery");
  
  Serial.print("WiFi AP: ");
  Serial.println(wifiAPEnabled ? "Active" : "Inactive");
  
  Serial.println("==============\n");
  Serial.flush();
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n\n=== Christmas PCB ===");
  Serial.println("=== 6h Timer System ==============");
  Serial.println("=== Hold both buttons for WiFi AP ===");
  
  pinMode(BUZZER, OUTPUT);
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);
  pinMode(BATT_SENSE, INPUT);
  pinMode(LDR_PIN, INPUT);
  
  uint32_t seed = 0;
  for(int i = 0; i < 16; i++) {
    seed = (seed << 2) | (analogRead(LDR_PIN) & 0x03);
    delay(1);
  }
  randomSeed(seed);
  
  // Disable WiFi by default
  WiFi.mode(WIFI_OFF);
  
  checkPowerSource();
  
  FastLED.addLeds<WS2812B, RGB_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(currentBrightness);
  
  loadSettings();
  updateDisplay();
  FastLED.show();
  
  Serial.println("\nReady!");
  Serial.println("Button 1 SHORT: Change pattern");
  Serial.println("Button 1 LONG: Toggle 6h timer");
  Serial.println("Button 2: Play/Stop songs");
  Serial.println("Both buttons (1s): WiFi AP with message");
  printPowerStatus();
}

void loop() {
  static unsigned long lastHeartbeat = 0;
  unsigned long currentMillis = millis();
  
  if (currentMillis - lastHeartbeat >= 1000) {
    totalUptimeLow++;
    if (totalUptimeLow == 0) totalUptimeHigh++;
    
    if (currentMillis < lastMillisCheck) {
      markSettingsChanged();
    }
    lastMillisCheck = currentMillis;
    
    lastHeartbeat = currentMillis;
    markSettingsChanged();
  }
  
  // Check WiFi timeout
  if (wifiAPEnabled) {
    checkWiFiTimeout();
  }
  
  updateTimerState();
  checkButtons();
  updateSong();
  updatePatterns();
  
  // Handle WiFi AP requests
  if (wifiAPEnabled) {
    dnsServer.processNextRequest();
    server.handleClient();
  }
  
  if (millis() - lastBatteryCheck > batteryCheckInterval) {
    checkPowerSource();
    lastBatteryCheck = millis();
  }
  
  if (millis() - lastSensorOutput > 10000) {
    outputSensorData();
    lastSensorOutput = millis();
  }
  
  saveToMemory();
  
  FastLED.show();
}