#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <EEPROM.h>
#include <ArduinoJson.h>
#include <HDC2080.h>

// WiFi credentials - UPDATE THESE WITH YOUR NETWORK
const char* ssid = "WIFICRT";
const char* password = "JuCasSan27@#*";

// Fan control pins
#define FAN1_PIN 25      // GPIO25 for FAN1_output
#define FAN2_PIN 26      // GPIO26 for FAN2_output
#define LED_PIN 13       // ESP32 built-in LED
#define WATCHDOG_PIN 5   // GPIO5 for watchdog output

// PWM configuration
#define FAN_PWM_FREQ 25000     // 25kHz PWM frequency
#define FAN_PWM_CHANNEL_1 0    // PWM channel for fan 1
#define FAN_PWM_CHANNEL_2 1    // PWM channel for fan 2
#define LED_PWM_CHANNEL 2      // PWM channel for LED
#define FAN_PWM_RESOLUTION 8   // 8-bit resolution (0-255)
#define LED_PWM_RESOLUTION 8   // 8-bit resolution (0-255)

// Runtime configuration
#define RUNTIME_HOURS 8        // Run for 8 hours
const unsigned long RUNTIME_MS = RUNTIME_HOURS * 60UL * 60UL * 1000UL;

// LED dimming configuration
#define LED_START_BRIGHTNESS 255
#define LED_END_BRIGHTNESS 0
#define DIMMING_START_PERCENT 75

// Watchdog configuration
#define WATCHDOG_INTERVAL 10000 // 10 seconds

// EEPROM configuration
#define EEPROM_SIZE 512
#define EEPROM_FAN1_ADDR 0
#define EEPROM_FAN2_ADDR 4
#define EEPROM_MAGIC_ADDR 8
#define EEPROM_ENABLED_ADDR 12  // Store fan enabled state
#define EEPROM_TIMER_ADDR 16    // Store auto timer enabled state
#define EEPROM_MAGIC_VALUE 0xABCD

// Settings save delay
#define SETTINGS_SAVE_DELAY 15000 // 15 seconds

// Global variables
unsigned long startTime = 0;
unsigned long lastWatchdogKick = 0;
unsigned long lastStatusReport = 0;
unsigned long lastSettingsChange = 0;
bool settingsChanged = false;
bool wifiConnected = false;
bool fansEnabled = true;  // New: Fan enable/disable state
unsigned long ledFlashStart = 0;  // New: LED flash timing
bool ledFlashing = false;  // New: LED flash state
bool autoTimerEnabled = true;  // New: Auto timer enable/disable

// Fan speeds (112-255, which is 44%-100%)
int fan1Speed = 220;
int fan2Speed = 220;

// HDC2080 sensors
#define ADDR_SENSOR1 0x40
#define ADDR_SENSOR2 0x41
HDC2080 sensor1(ADDR_SENSOR1);
HDC2080 sensor2(ADDR_SENSOR2);

// Web server
WebServer server(80);

// Function prototypes
void updateLedBrightness(float percentComplete);
void kickWatchdog();
void reportStatus(unsigned long elapsedTime);
void setupWiFi();
void setupWebServer();
void handleRoot();
void handleAPI();
void handleSetSpeed();
void handleToggleFans();  // New: Toggle fans on/off
void handleToggleTimer(); // New: Toggle auto timer
void loadSettings();
void saveSettings();
void checkSettingsSave();
void flashLED();  // New: Flash LED on command
void updateFanOutputs();  // New: Update actual fan outputs

void setup() {
  Serial.begin(115200);
  delay(2000);

  // Initialize EEPROM
  EEPROM.begin(EEPROM_SIZE);
  loadSettings();

  // Initialize I2C communication for both sensors
  sensor1.begin();
  sensor2.begin();

  // Begin with a device reset for both sensors
  sensor1.reset();
  sensor2.reset();

  // Configure Measurements for both sensors
  sensor1.setMeasurementMode(TEMP_AND_HUMID);
  sensor1.setRate(ONE_HZ);
  sensor1.setTempRes(FOURTEEN_BIT);
  sensor1.setHumidRes(FOURTEEN_BIT);

  sensor2.setMeasurementMode(TEMP_AND_HUMID);
  sensor2.setRate(ONE_HZ);
  sensor2.setTempRes(FOURTEEN_BIT);
  sensor2.setHumidRes(FOURTEEN_BIT);

  // Begin measuring for both sensors
  sensor1.triggerMeasurement();
  sensor2.triggerMeasurement();

  // Configure PWM for Fan 1
  ledcSetup(FAN_PWM_CHANNEL_1, FAN_PWM_FREQ, FAN_PWM_RESOLUTION);
  ledcAttachPin(FAN1_PIN, FAN_PWM_CHANNEL_1);

  // Configure PWM for Fan 2
  ledcSetup(FAN_PWM_CHANNEL_2, FAN_PWM_FREQ, FAN_PWM_RESOLUTION);
  ledcAttachPin(FAN2_PIN, FAN_PWM_CHANNEL_2);

  // Configure PWM for LED
  ledcSetup(LED_PWM_CHANNEL, 5000, LED_PWM_RESOLUTION);
  ledcAttachPin(LED_PIN, LED_PWM_CHANNEL);

  // Configure Watchdog pin
  pinMode(WATCHDOG_PIN, OUTPUT);
  digitalWrite(WATCHDOG_PIN, HIGH);

  // Set fan speeds to loaded values
  updateFanOutputs();

  // Set LED to full brightness initially
  ledcWrite(LED_PWM_CHANNEL, LED_START_BRIGHTNESS);

  // Setup WiFi and Web Server
  setupWiFi();
  setupWebServer();

  Serial.printf("Fans running at PWM values: Fan1=%d, Fan2=%d (Enabled: %s)\n", 
                fan1Speed, fan2Speed, fansEnabled ? "Yes" : "No");
  Serial.printf("Fans will run for %d hours\n", RUNTIME_HOURS);
  Serial.printf("LED will start dimming when %d%% of runtime has elapsed\n", DIMMING_START_PERCENT);

  // Record start time
  startTime = millis();
  lastWatchdogKick = startTime;
  lastStatusReport = startTime;
}

void loop() {
  unsigned long currentTime = millis();
  unsigned long elapsedTime = currentTime - startTime;
  float percentComplete = (float)elapsedTime / RUNTIME_MS * 100.0;

  // Handle web server only if WiFi is connected
  if (WiFi.status() == WL_CONNECTED) {
    server.handleClient();
  }

  // Handle LED flashing
  if (ledFlashing && (millis() - ledFlashStart > 200)) {
    ledFlashing = false;
    // Don't restore LED here - let updateLedBrightness handle it
  }

  // Update LED brightness (this will override flash if needed)
  if (!ledFlashing) {
    updateLedBrightness(percentComplete);
  }

  // Check if settings should be saved
  checkSettingsSave();

  // Status report every 10 seconds
  if (currentTime - lastStatusReport >= 10000) {
    reportStatus(elapsedTime);
    lastStatusReport = currentTime;
  }

  // Check if runtime is complete
  if (autoTimerEnabled && elapsedTime >= RUNTIME_MS) {
    fansEnabled = false;
    updateFanOutputs();
    ledcWrite(LED_PWM_CHANNEL, 0);
    Serial.println("Runtime complete. Fans stopped.");
    while (1) {
      if (WiFi.status() == WL_CONNECTED) {
        server.handleClient();
      }
      delay(1000);
    }
  }

  // Watchdog kick
  if (currentTime - lastWatchdogKick >= WATCHDOG_INTERVAL) {
    kickWatchdog();
    lastWatchdogKick = currentTime;
  }

  delay(100);
}

void setupWiFi() {
  // Add more detailed WiFi debugging
  Serial.println("Starting WiFi connection...");
  Serial.print("SSID: ");
  Serial.println(ssid);
  Serial.print("Password length: ");
  Serial.println(strlen(password));
  
  WiFi.mode(WIFI_STA);  // Set WiFi to station mode
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 30) {
    delay(1000);
    Serial.print(".");
    Serial.print(" Status: ");
    Serial.print(WiFi.status());
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println();
    Serial.println("WiFi connected successfully!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.print("Signal strength (RSSI): ");
    Serial.println(WiFi.RSSI());
    wifiConnected = true;
  } else {
    Serial.println();
    Serial.println("Failed to connect to WiFi!");
    Serial.print("WiFi Status: ");
    Serial.println(WiFi.status());
    
    // Print common status codes
    switch(WiFi.status()) {
      case WL_NO_SSID_AVAIL:
        Serial.println("Error: SSID not found");
        break;
      case WL_CONNECT_FAILED:
        Serial.println("Error: Connection failed (wrong password or other issue)");
        break;
      case WL_CONNECTION_LOST:
        Serial.println("Error: Connection lost");
        break;
      case WL_DISCONNECTED:
        Serial.println("Error: Disconnected");
        break;
      default:
        Serial.println("Error: Unknown WiFi error");
    }
    
    // Try to scan for available networks
    Serial.println("Scanning for available networks...");
    int n = WiFi.scanNetworks();
    if (n == 0) {
      Serial.println("No networks found");
    } else {
      Serial.print(n);
      Serial.println(" networks found:");
      for (int i = 0; i < n; ++i) {
        Serial.print(i + 1);
        Serial.print(": ");
        Serial.print(WiFi.SSID(i));
        Serial.print(" (");
        Serial.print(WiFi.RSSI(i));
        Serial.print(")");
        Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " " : "*");
      }
    }
    wifiConnected = false;
  }
}

void setupWebServer() {
  // Only start web server if WiFi is connected
  if (WiFi.status() == WL_CONNECTED) {
    server.on("/", handleRoot);
    server.on("/api/data", handleAPI);
    server.on("/api/setspeed", HTTP_POST, handleSetSpeed);
    server.on("/api/toggle", HTTP_POST, handleToggleFans);  // Toggle endpoint
    server.on("/api/toggletimer", HTTP_POST, handleToggleTimer); // Timer toggle endpoint
    
    server.begin();
    Serial.println("Web server started");
    Serial.print("Access the web interface at: http://");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("Web server NOT started - WiFi not connected");
  }
}

void handleRoot() {
  String html = R"(
<!DOCTYPE html>
<html>
<head>
    <title>Fan Controller</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
        * {
            box-sizing: border-box;
        }
        body { 
            font-family: Arial, sans-serif; 
            margin: 0;
            padding: 10px; 
            background-color: #1a1a1a; 
            color: #e0e0e0;
            min-height: 100vh;
        }
        .container { 
            max-width: 1200px; 
            margin: 0 auto; 
            background: #2d2d2d; 
            padding: 15px; 
            border-radius: 10px; 
            box-shadow: 0 2px 20px rgba(0,0,0,0.5); 
            border: 1px solid #404040;
        }
        .sensor-grid { 
            display: grid; 
            grid-template-columns: repeat(auto-fit, minmax(280px, 1fr)); 
            gap: 15px; 
            margin-bottom: 20px; 
        }
        .sensor-card { 
            background: #3a3a3a; 
            padding: 15px; 
            border-radius: 8px; 
            border: 1px solid #505050;
            box-shadow: 0 2px 8px rgba(0,0,0,0.3);
            min-height: 120px;
        }
        .controls-grid {
            display: grid;
            grid-template-columns: 1fr;
            gap: 15px;
        }
        .fan-control { 
            background: #2a3f5f; 
            padding: 15px; 
            border-radius: 8px; 
            border: 1px solid #4a6fa5;
        }
        .toggle-container {
            background: #4a2a2a;
            padding: 15px;
            border-radius: 8px;
            border: 1px solid #8a4a4a;
            text-align: center;
        }
        .toggle-btn {
            background: #d32f2f;
            color: white;
            border: none;
            padding: 15px 25px;
            border-radius: 8px;
            font-size: 16px;
            font-weight: bold;
            cursor: pointer;
            transition: all 0.3s;
            min-width: 150px;
            margin: 5px;
            touch-action: manipulation;
            user-select: none;
        }
        .toggle-btn:hover, .toggle-btn:active {
            background: #b71c1c;
            transform: scale(0.98);
        }
        .toggle-btn.enabled {
            background: #388e3c;
        }
        .toggle-btn.enabled:hover, .toggle-btn.enabled:active {
            background: #2e7d32;
        }
        .slider-container { 
            margin: 15px 0; 
        }
        .slider { 
            width: 100%; 
            height: 35px; 
            border-radius: 8px; 
            background: #555; 
            outline: none;
            -webkit-appearance: none;
            appearance: none;
            cursor: pointer;
            touch-action: manipulation;
        }
        .slider::-webkit-slider-thumb {
            -webkit-appearance: none;
            appearance: none;
            width: 35px;
            height: 35px;
            border-radius: 50%;
            background: #2196F3;
            cursor: pointer;
            box-shadow: 0 3px 8px rgba(0,0,0,0.4);
            transition: all 0.2s;
        }
        .slider::-webkit-slider-thumb:active {
            transform: scale(1.1);
            box-shadow: 0 4px 12px rgba(33, 150, 243, 0.4);
        }
        .slider::-moz-range-thumb {
            width: 35px;
            height: 35px;
            border-radius: 50%;
            background: #2196F3;
            cursor: pointer;
            border: none;
            box-shadow: 0 3px 8px rgba(0,0,0,0.4);
        }
        .status { 
            background: linear-gradient(135deg, #4a4a1a, #5a5a2a); 
            padding: 15px; 
            border-radius: 12px; 
            margin-bottom: 20px;
            border: 1px solid #6a6a2a;
            box-shadow: 0 4px 12px rgba(0,0,0,0.3);
        }
        .timer-display {
            display: grid;
            grid-template-columns: 1fr auto 1fr;
            gap: 15px;
            align-items: center;
            margin: 15px 0;
        }
        .time-box {
            background: rgba(0,0,0,0.3);
            padding: 12px;
            border-radius: 8px;
            text-align: center;
            min-height: 60px;
            display: flex;
            flex-direction: column;
            justify-content: center;
        }
        .time-label {
            font-size: 11px;
            color: #888;
            margin-bottom: 5px;
            font-weight: 500;
        }
        .time-value {
            font-size: 18px;
            font-weight: bold;
            color: #fff;
            font-family: 'Courier New', monospace;
        }
        .progress-container {
            text-align: center;
        }
        .progress-circle {
            width: 80px;
            height: 80px;
            border-radius: 50%;
            background: conic-gradient(#2196F3 0deg, #555 0deg);
            display: flex;
            align-items: center;
            justify-content: center;
            margin: 0 auto 10px;
            position: relative;
            transition: all 0.3s ease;
        }
        .progress-inner {
            width: 60px;
            height: 60px;
            border-radius: 50%;
            background: #2d2d2d;
            display: flex;
            align-items: center;
            justify-content: center;
            font-size: 13px;
            font-weight: bold;
            color: #2196F3;
        }
        .timer-toggle {
            background: #2196F3;
            color: white;
            border: none;
            padding: 10px 18px;
            border-radius: 6px;
            font-size: 12px;
            cursor: pointer;
            transition: all 0.3s;
            touch-action: manipulation;
            user-select: none;
        }
        .timer-toggle:hover, .timer-toggle:active {
            background: #1976D2;
            transform: scale(0.98);
        }
        .timer-toggle.disabled {
            background: #666;
        }
        .timer-toggle.disabled:hover, .timer-toggle.disabled:active {
            background: #555;
        }
        .value { 
            font-size: 22px; 
            font-weight: bold; 
            color: #64B5F6; 
            margin: 5px 0;
        }
        .label { 
            font-size: 13px; 
            color: #b0b0b0; 
            margin-bottom: 5px;
        }
        h1 { 
            color: #f0f0f0; 
            text-align: center;
            text-shadow: 0 2px 4px rgba(0,0,0,0.3);
            margin: 10px 0 20px 0;
            font-size: 24px;
        }
        h2 { 
            color: #d0d0d0; 
            margin: 0 0 15px 0; 
            font-size: 18px;
        }
        label {
            color: #e0e0e0;
            font-weight: 500;
            font-size: 14px;
            display: block;
            margin-bottom: 8px;
        }
        
        /* Mobile Specific Styles */
        @media (max-width: 768px) {
            body {
                padding: 5px;
            }
            .container {
                padding: 12px;
            }
            .sensor-grid {
                grid-template-columns: 1fr;
                gap: 12px;
            }
            .timer-display {
                grid-template-columns: 1fr;
                gap: 12px;
                text-align: center;
            }
            .time-box {
                padding: 15px;
            }
            .time-value {
                font-size: 20px;
            }
            .progress-circle {
                width: 90px;
                height: 90px;
                margin: 10px auto;
            }
            .progress-inner {
                width: 70px;
                height: 70px;
                font-size: 14px;
            }
            h1 {
                font-size: 20px;
                margin: 5px 0 15px 0;
            }
            h2 {
                font-size: 16px;
            }
            .value {
                font-size: 24px;
            }
            .toggle-btn {
                width: 100%;
                max-width: 300px;
                padding: 18px 25px;
                font-size: 18px;
                margin: 8px auto;
                display: block;
            }
            .timer-toggle {
                padding: 12px 20px;
                font-size: 14px;
            }
            .slider {
                height: 40px;
            }
            .slider::-webkit-slider-thumb {
                width: 40px;
                height: 40px;
            }
            .slider::-moz-range-thumb {
                width: 40px;
                height: 40px;
            }
        }
        
        /* Large Screen Optimization */
        @media (min-width: 1024px) {
            .controls-grid {
                grid-template-columns: 1fr 1fr;
                gap: 20px;
            }
            .sensor-grid {
                grid-template-columns: repeat(2, 1fr);
            }
            .timer-display {
                gap: 30px;
            }
            .progress-circle {
                width: 100px;
                height: 100px;
            }
            .progress-inner {
                width: 80px;
                height: 80px;
                font-size: 16px;
            }
        }
        
        /* Touch-friendly hover states */
        @media (hover: none) {
            .toggle-btn:hover {
                background: #d32f2f;
                transform: none;
            }
            .toggle-btn.enabled:hover {
                background: #388e3c;
            }
            .timer-toggle:hover {
                background: #2196F3;
                transform: none;
            }
            .timer-toggle.disabled:hover {
                background: #666;
            }
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>Fan Controller Dashboard</h1>
        
        <div class="status">
            <h2>System Status</h2>
            <div class="timer-display">
                <div class="time-box">
                    <div class="time-label">ELAPSED</div>
                    <div class="time-value" id="elapsed-time">00:00:00</div>
                </div>
                <div class="progress-container">
                    <div class="progress-circle" id="progress-circle">
                        <div class="progress-inner" id="progress-text">0%</div>
                    </div>
                    <button id="timer-toggle" class="timer-toggle">TIMER ENABLED</button>
                </div>
                <div class="time-box">
                    <div class="time-label">REMAINING</div>
                    <div class="time-value" id="remaining-time">08:00:00</div>
                </div>
            </div>
        </div>

        <div class="sensor-grid">
            <div class="sensor-card">
                <h2>Sensor 1</h2>
                <div class="label">Temperature</div>
                <div class="value" id="temp1">--&deg;C</div>
                <div class="label">Humidity</div>
                <div class="value" id="hum1">--%</div>
            </div>
            
            <div class="sensor-card">
                <h2>Sensor 2</h2>
                <div class="label">Temperature</div>
                <div class="value" id="temp2">--&deg;C</div>
                <div class="label">Humidity</div>
                <div class="value" id="hum2">--%</div>
            </div>
        </div>

        <div class="controls-grid">
            <div class="toggle-container">
                <h2>Fan Control</h2>
                <button id="fan-toggle" class="toggle-btn enabled">FANS ENABLED</button>
            </div>

            <div class="fan-control">
                <h2>Fan Speed Control</h2>
                <div class="slider-container">
                    <label for="fan1">Fan 1 Speed: <span id="fan1-value">0</span>%</label>
                    <input type="range" id="fan1" class="slider" min="112" max="255" value="220">
                </div>
                <div class="slider-container">
                    <label for="fan2">Fan 2 Speed: <span id="fan2-value">0</span>%</label>
                    <input type="range" id="fan2" class="slider" min="112" max="255" value="220">
                </div>
            </div>
        </div>
    </div>

    <script>
        function updateData() {
            fetch('/api/data')
                .then(response => response.json())
                .then(data => {
                    document.getElementById('temp1').innerHTML = data.temp1.toFixed(1) + '&deg;C';
                    document.getElementById('hum1').textContent = data.hum1.toFixed(1) + '%';
                    document.getElementById('temp2').innerHTML = data.temp2.toFixed(1) + '&deg;C';
                    document.getElementById('hum2').textContent = data.hum2.toFixed(1) + '%';
                    
                    // Update timer display
                    updateTimerDisplay(data);
                    
                    // Update fan toggle button
                    const toggleBtn = document.getElementById('fan-toggle');
                    if (data.fansEnabled) {
                        toggleBtn.textContent = 'FANS ENABLED';
                        toggleBtn.className = 'toggle-btn enabled';
                    } else {
                        toggleBtn.textContent = 'FANS DISABLED';
                        toggleBtn.className = 'toggle-btn';
                    }
                    
                    // Update timer toggle button
                    const timerBtn = document.getElementById('timer-toggle');
                    if (data.autoTimerEnabled) {
                        timerBtn.textContent = 'TIMER ENABLED';
                        timerBtn.className = 'timer-toggle';
                    } else {
                        timerBtn.textContent = 'TIMER DISABLED';
                        timerBtn.className = 'timer-toggle disabled';
                    }
                    
                    // Update fan speed sliders if they haven't been changed by user
                    if (!document.getElementById('fan1').matches(':focus')) {
                        document.getElementById('fan1').value = Math.max(112, data.fan1Speed);
                    }
                    if (!document.getElementById('fan2').matches(':focus')) {
                        document.getElementById('fan2').value = Math.max(112, data.fan2Speed);
                    }
                    
                    updateFanSpeedDisplay();
                })
                .catch(error => console.error('Error:', error));
        }

        function updateFanSpeedDisplay() {
            const fan1Value = document.getElementById('fan1').value;
            const fan2Value = document.getElementById('fan2').value;
            // Convert PWM (112-255) to percentage (44-100%)
            document.getElementById('fan1-value').textContent = Math.round(((fan1Value - 112) / (255 - 112)) * 56 + 44);
            document.getElementById('fan2-value').textContent = Math.round(((fan2Value - 112) / (255 - 112)) * 56 + 44);
        }

        function setFanSpeed(fan, speed) {
            fetch('/api/setspeed', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json',
                },
                body: JSON.stringify({
                    fan: fan,
                    speed: parseInt(speed)
                })
            });
        }

        function toggleFans() {
            fetch('/api/toggle', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json',
                },
                body: JSON.stringify({})
            });
        }

        function toggleTimer() {
            fetch('/api/toggletimer', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json',
                },
                body: JSON.stringify({})
            });
        }

        function updateTimerDisplay(data) {
            // Update elapsed and remaining time
            document.getElementById('elapsed-time').textContent = data.elapsedFormatted;
            if (data.autoTimerEnabled) {
                document.getElementById('remaining-time').textContent = data.remainingFormatted;
            } else {
                document.getElementById('remaining-time').textContent = 'DISABLED';
            }
            
            // Update progress circle
            const progressCircle = document.getElementById('progress-circle');
            const progressText = document.getElementById('progress-text');
            const percentage = data.autoTimerEnabled ? data.percentComplete : 0;
            
            const degrees = (percentage / 100) * 360;
            progressCircle.style.background = `conic-gradient(#2196F3 ${degrees}deg, #555 ${degrees}deg)`;
            progressText.textContent = Math.round(percentage) + '%';
        }

        // Event listeners
        document.getElementById('fan1').addEventListener('input', function() {
            updateFanSpeedDisplay();
            setFanSpeed(1, this.value);
        });

        document.getElementById('fan2').addEventListener('input', function() {
            updateFanSpeedDisplay();
            setFanSpeed(2, this.value);
        });

        document.getElementById('fan-toggle').addEventListener('click', function() {
            toggleFans();
        });

        document.getElementById('timer-toggle').addEventListener('click', function() {
            toggleTimer();
        });

        // Initial load and periodic updates
        updateData();
        setInterval(updateData, 2000);
        updateFanSpeedDisplay();
    </script>
</body>
</html>
)";
  
  server.send(200, "text/html", html);
}

void handleAPI() {
  unsigned long currentTime = millis();
  unsigned long elapsedTime = currentTime - startTime;
  unsigned long remainingTime = RUNTIME_MS - elapsedTime;
  
  // Format elapsed time
  unsigned long hoursElapsed = elapsedTime / 3600000;
  unsigned long minutesElapsed = (elapsedTime % 3600000) / 60000;
  unsigned long secondsElapsed = (elapsedTime % 60000) / 1000;
  
  // Format remaining time
  unsigned long hoursRemaining = remainingTime / 3600000;
  unsigned long minutesRemaining = (remainingTime % 3600000) / 60000;
  unsigned long secondsRemaining = (remainingTime % 60000) / 1000;
  
  String status = String("Elapsed: ") + 
                  String(hoursElapsed) + ":" + 
                  String(minutesElapsed) + ":" + 
                  String(secondsElapsed) + 
                  " | Remaining: " + 
                  String(hoursRemaining) + ":" + 
                  String(minutesRemaining) + ":" + 
                  String(secondsRemaining);

  // Format time strings for display
  char elapsedStr[16];
  char remainingStr[16];
  snprintf(elapsedStr, sizeof(elapsedStr), "%02lu:%02lu:%02lu", hoursElapsed, minutesElapsed, secondsElapsed);
  snprintf(remainingStr, sizeof(remainingStr), "%02lu:%02lu:%02lu", hoursRemaining, minutesRemaining, secondsRemaining);

  JsonDocument doc;
  doc["temp1"] = sensor1.readTemp();
  doc["hum1"] = sensor1.readHumidity();
  doc["temp2"] = sensor2.readTemp();
  doc["hum2"] = sensor2.readHumidity();
  doc["fan1Speed"] = fan1Speed;
  doc["fan2Speed"] = fan2Speed;
  doc["fansEnabled"] = fansEnabled;
  doc["autoTimerEnabled"] = autoTimerEnabled;
  doc["status"] = status;
  doc["elapsedFormatted"] = elapsedStr;
  doc["remainingFormatted"] = remainingStr;
  doc["percentComplete"] = autoTimerEnabled ? (float)elapsedTime / RUNTIME_MS * 100.0 : 0.0;
  
  String response;
  serializeJson(doc, response);
  server.send(200, "application/json", response);
}

void handleSetSpeed() {
  if (server.hasArg("plain")) {
    JsonDocument doc;
    deserializeJson(doc, server.arg("plain"));
    
    int fan = doc["fan"];
    int speed = doc["speed"];
    
    // Constrain speed to valid range (minimum 44% = 112 PWM)
    speed = constrain(speed, 112, 255);
    
    if (fan == 1) {
      fan1Speed = speed;
    } else if (fan == 2) {
      fan2Speed = speed;
    }
    
    // Update actual fan outputs
    updateFanOutputs();
    
    // Flash LED to indicate command received
    flashLED();
    
    // Mark settings as changed
    settingsChanged = true;
    lastSettingsChange = millis();
    
    Serial.printf("Fan %d speed set to %d\n", fan, speed);
    
    server.send(200, "text/plain", "OK");
  } else {
    server.send(400, "text/plain", "Bad Request");
  }
}

void handleToggleFans() {
  fansEnabled = !fansEnabled;
  updateFanOutputs();
  flashLED();
  
  // Mark settings as changed
  settingsChanged = true;
  lastSettingsChange = millis();
  
  Serial.printf("Fans %s\n", fansEnabled ? "ENABLED" : "DISABLED");
  
  server.send(200, "text/plain", "OK");
}

void handleToggleTimer() {
  autoTimerEnabled = !autoTimerEnabled;
  flashLED();
  
  // Mark settings as changed
  settingsChanged = true;
  lastSettingsChange = millis();
  
  Serial.printf("Auto Timer %s\n", autoTimerEnabled ? "ENABLED" : "DISABLED");
  
  server.send(200, "text/plain", "OK");
}

void loadSettings() {
  uint16_t magic = EEPROM.readUShort(EEPROM_MAGIC_ADDR);
  
  if (magic == EEPROM_MAGIC_VALUE) {
    // Valid settings found
    fan1Speed = EEPROM.readUShort(EEPROM_FAN1_ADDR);
    fan2Speed = EEPROM.readUShort(EEPROM_FAN2_ADDR);
    fansEnabled = EEPROM.readBool(EEPROM_ENABLED_ADDR);
    autoTimerEnabled = EEPROM.readBool(EEPROM_TIMER_ADDR);
    
    // Constrain to valid range (minimum 44% = 112 PWM)
    fan1Speed = constrain(fan1Speed, 112, 255);
    fan2Speed = constrain(fan2Speed, 112, 255);
    
    Serial.printf("Settings loaded from EEPROM: Fan1=%d, Fan2=%d, Enabled=%s, Timer=%s\n", 
                  fan1Speed, fan2Speed, fansEnabled ? "Yes" : "No", autoTimerEnabled ? "Yes" : "No");
  } else {
    // No valid settings, use defaults
    fan1Speed = 220;
    fan2Speed = 220;
    fansEnabled = true;
    autoTimerEnabled = true;
    Serial.println("No valid settings found, using defaults");
  }
}

void saveSettings() {
  EEPROM.writeUShort(EEPROM_FAN1_ADDR, fan1Speed);
  EEPROM.writeUShort(EEPROM_FAN2_ADDR, fan2Speed);
  EEPROM.writeBool(EEPROM_ENABLED_ADDR, fansEnabled);
  EEPROM.writeBool(EEPROM_TIMER_ADDR, autoTimerEnabled);
  EEPROM.writeUShort(EEPROM_MAGIC_ADDR, EEPROM_MAGIC_VALUE);
  EEPROM.commit();
  
  Serial.printf("Settings saved to EEPROM: Fan1=%d, Fan2=%d, Enabled=%s, Timer=%s\n", 
                fan1Speed, fan2Speed, fansEnabled ? "Yes" : "No", autoTimerEnabled ? "Yes" : "No");
}

void checkSettingsSave() {
  if (settingsChanged && (millis() - lastSettingsChange >= SETTINGS_SAVE_DELAY)) {
    saveSettings();
    settingsChanged = false;
  }
}

void updateLedBrightness(float percentComplete) {
  if (percentComplete < DIMMING_START_PERCENT) {
    ledcWrite(LED_PWM_CHANNEL, LED_START_BRIGHTNESS);
    return;
  }
  float dimmingProgress = (percentComplete - DIMMING_START_PERCENT) / (100.0 - DIMMING_START_PERCENT);
  dimmingProgress = fmax(0.0, fmin(1.0, dimmingProgress));
  int brightness = LED_START_BRIGHTNESS - dimmingProgress * (LED_START_BRIGHTNESS - LED_END_BRIGHTNESS);
  ledcWrite(LED_PWM_CHANNEL, brightness);
}

void kickWatchdog() {
  digitalWrite(WATCHDOG_PIN, LOW);
  delay(1);
  digitalWrite(WATCHDOG_PIN, HIGH);
}

void reportStatus(unsigned long elapsedTime) {
  unsigned long hoursElapsed = elapsedTime / 3600000;
  unsigned long minutesElapsed = (elapsedTime % 3600000) / 60000;
  unsigned long secondsElapsed = (elapsedTime % 60000) / 1000;
  unsigned long remainingTime = RUNTIME_MS - elapsedTime;
  unsigned long hoursRemaining = remainingTime / 3600000;
  unsigned long minutesRemaining = (remainingTime % 3600000) / 60000;
  unsigned long secondsRemaining = (remainingTime % 60000) / 1000;
  
  Serial.println("----------------------------------------------------------");
  Serial.print("Elapsed Time: ");
  Serial.printf("%02lu:%02lu:%02lu", hoursElapsed, minutesElapsed, secondsElapsed);
  Serial.print(", Remaining Time: ");
  Serial.printf("%02lu:%02lu:%02lu", hoursRemaining, minutesRemaining, secondsRemaining);
  Serial.print(" (");
  Serial.print((float)elapsedTime / RUNTIME_MS * 100.0);
  Serial.println("%)");

  Serial.println("Sensor 1:");
  Serial.print("  Temperature: ");
  Serial.print(sensor1.readTemp());
  Serial.print(" C, Humidity: ");
  Serial.println(sensor1.readHumidity());

  Serial.println("Sensor 2:");
  Serial.print("  Temperature: ");
  Serial.print(sensor2.readTemp());
  Serial.print(" C, Humidity: ");
  Serial.println(sensor2.readHumidity());
  
  Serial.printf("Fan Speeds: Fan1=%d, Fan2=%d (Enabled: %s)\n", 
                fan1Speed, fan2Speed, fansEnabled ? "Yes" : "No");
  
  // Show WiFi status
  if (wifiConnected && WiFi.status() == WL_CONNECTED) {
    Serial.print("WiFi: Connected (");
    Serial.print(WiFi.localIP());
    Serial.println(")");
  } else {
    Serial.println("WiFi: Disconnected");
  }
  
  Serial.println("----------------------------------------------------------");

  sensor1.triggerMeasurement();
  sensor2.triggerMeasurement();
}

void flashLED() {
  ledFlashing = true;
  ledFlashStart = millis();
  ledcWrite(LED_PWM_CHANNEL, 255);  // Full brightness flash
}

void updateFanOutputs() {
  if (fansEnabled) {
    ledcWrite(FAN_PWM_CHANNEL_1, fan1Speed);
    ledcWrite(FAN_PWM_CHANNEL_2, fan2Speed);
  } else {
    ledcWrite(FAN_PWM_CHANNEL_1, 0);
    ledcWrite(FAN_PWM_CHANNEL_2, 0);
  }
}