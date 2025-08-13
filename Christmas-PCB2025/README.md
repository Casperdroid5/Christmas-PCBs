# 🎄 Christmas PCB Project

A festive microcontroller-based Christmas decoration featuring 8 addressable RGB LEDs and a buzzer for playing Christmas songs. This project combines beautiful light patterns with classic holiday music for the perfect seasonal display.

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Platform](https://img.shields.io/badge/Platform-ESP32-blue.svg)](https://www.espressif.com/en/products/socs/esp32)
[![FastLED](https://img.shields.io/badge/FastLED-Compatible-green.svg)](https://fastled.io/)

## ✨ Features

### 🌈 LED Animations
- **12 Beautiful Patterns**: Static colors, rainbow, snake, random blink, chase, wave, fade, sparkle, firework, meteor, and candy cane modes
- **Christmas Colors**: Red and green color schemes throughout all patterns
- **Adaptive Brightness**: Automatic brightness adjustment based on power source (USB vs battery)
- **Light Sensor Control**: LEDs automatically turn off in bright environments to save power

### 🎵 Music Playback
- **18 Classic Christmas Songs** including:
  - Santa Claus Is Coming to Town
  - Jingle Bells
  - We Wish You a Merry Christmas
  - Silent Night
  - Rudolph the Red-Nosed Reindeer
  - O Christmas Tree
  - And 12 more beloved classics!
- **Non-blocking playback**: LEDs continue animating during music
- **Auto-advance**: Cycles through songs automatically

### 🔧 Smart Controls
- **Button 1**: 
  - **Short press**: Cycle through LED patterns and colors
  - **Long press (2s)**: Toggle between auto light sensor mode and always-on mode
- **Button 2**: Play/stop songs and cycle through playlist
- **Visual feedback**: LED flashes indicate mode changes (Green = always on, Red = light sensor control)

### 💾 Persistent Memory
- **Settings saved to flash**: LED patterns, colors, songs, and modes persist across power cycles
- **Uptime tracking**: Keeps track of total device runtime across all sessions
- **Auto-save**: Settings automatically saved every 30 seconds

### 🔋 Power Management
- **Dual power support**: USB (5V) or 3xAAA batteries (4.5V)
- **Automatic detection**: Switches brightness based on detected power source
- **Battery-friendly**: Reduced brightness and disabled WiFi on battery power
- **Light sensor**: Automatically disables LEDs in bright environments to conserve battery

### 🌙 Smart Light Control
- **Automatic re-enable**: LEDs turn back on after 5 seconds of darkness
- **Manual override**: Long press Button 1 to permanently enable/disable light sensor control
- **Hysteresis**: Prevents flickering when light levels fluctuate

## 🛠️ Hardware Requirements

### Core Components
- **ESP32 microcontroller**
- **8x WS2812B addressable RGB LEDs** (Christmas tree or strip configuration)
- **Passive buzzer** for music playback
- **LDR (Light Dependent Resistor)** for ambient light sensing
- **2x Tactile buttons** for user input
- **Battery voltage divider circuit** for power source detection

### GPIO Pin Configuration
```cpp
#define RGB_PIN 3      // WS2812B LED data pin
#define BUZZER 10      // Buzzer pin
#define BUTTON1 4      // Pattern/Color button
#define BUTTON2 5      // Music control button
#define BATT_SENSE 0   // Battery voltage sensing
#define LDR_PIN 2      // Light sensor pin
```

### Power Specifications
- **USB Power**: 5V via USB-C connector
- **Battery Power**: 3xAAA batteries (4.5V nominal)
- **LED Count**: 8 WS2812B LEDs
- **Brightness Levels**: 50% (USB) / 25% (Battery)

## 📚 Software Dependencies

### Required Libraries
```cpp
#include <Arduino.h>      // Core Arduino framework
#include <FastLED.h>      // LED control library
#include <Preferences.h>  // ESP32 NVS storage
```

### Additional Files
- `christmas_songs.h` - Complete collection of 18 Christmas songs with note definitions
- `pitches.h` - Musical note frequency definitions

## 🚀 Getting Started

### 1. Hardware Setup
1. Connect the ESP32 according to the GPIO pin configuration
2. Wire the WS2812B LEDs in series (8 LEDs total)
3. Connect the passive buzzer to GPIO 10
4. Install buttons on GPIO 4 and 5 with pull-up resistors
5. Connect the LDR voltage divider to GPIO 2
6. Set up battery voltage sensing on GPIO 0

### 2. Software Installation
1. Clone this repository:
   ```bash
   git clone https://github.com/yourusername/christmas-pcb.git
   ```
2. Open in Arduino IDE or PlatformIO
3. Install required libraries:
   - FastLED library
   - ESP32 board package
4. Upload the code to your ESP32

### 3. Usage
- **Power on** the device via USB or batteries
- **Button 1** cycles through LED patterns and colors
- **Button 2** controls music playback
- **Long press Button 1** to toggle light sensor mode
- Device remembers all settings across power cycles

## 🎮 User Interface

### Button Controls
| Action | Function |
|--------|----------|
| Button 1 - Short Press | Cycle LED patterns/colors |
| Button 1 - Long Press (2s) | Toggle light sensor control |
| Button 2 - Short Press | Play/stop music & cycle songs |

### Visual Feedback
| LED Flash Pattern | Meaning |
|-------------------|---------|
| 2x Green Flashes | Always-on mode enabled |
| 2x Red Flashes | Light sensor control enabled |

### LED Patterns
1. **Static Color** - Solid red or green
2. **Random Scatter** - Random red/green pattern
3. **Rainbow** - Alternating red/green wave
4. **Snake** - Moving snake pattern
5. **Random Blink** - Random LEDs blinking
6. **Chase** - Single LED chasing around
7. **Wave** - Sine wave brightness pattern
8. **Fade Random** - Smooth color transitions
9. **Sparkle** - Twinkling effect
10. **Firework** - Explosive patterns
11. **Meteor** - Shooting star effect
12. **Candy Cane** - Rotating stripes

## 🎵 Music Library

The device includes 18 classic Christmas songs:

1. Santa Claus Is Coming to Town
2. Jingle Bells
3. We Wish You a Merry Christmas
4. Silent Night
5. Rudolph the Red-Nosed Reindeer
6. O Christmas Tree
7. O Come All Ye Faithful
8. O Little Town of Bethlehem
9. The First Noel
10. We Three Kings
11. White Christmas
12. Away in a Manger
13. Carol of the Bells
14. Deck the Halls
15. God Rest Ye Merry Gentlemen
16. Go Tell It on the Mountain
17. Hark the Herald Angels Sing
18. Joy to the World

## 🔧 Configuration

### Customizable Settings
```cpp
// Brightness levels
#define BRIGHTNESS_USB 128      // 50% for USB power
#define BRIGHTNESS_BATTERY 60   // 25% for battery power

// Light sensor threshold
#define LDR_THRESHOLD 2000      // Adjust based on environment
#define LDR_HYSTERESIS 200      // Prevents flickering

// Timing constants
#define longPressTime 2000      // 2 seconds for long press
#define darknessReEnableDelay 5000  // 5 seconds darkness timer
```

### Pattern Speed Controls
Each LED pattern has adjustable speed settings:
```cpp
const unsigned long RAINBOW_SPEED = 600;
const unsigned long SNAKE_SPEED = 800;
const unsigned long CHASE_SPEED = 720;
// ... and more
```

## 📊 Serial Monitor Output

The device provides detailed status information via serial output:
- Real-time sensor readings (light level, battery voltage)
- Current LED pattern and song information
- Power source detection and brightness levels
- Uptime tracking and heartbeat messages
- Button press events and mode changes

## 🔋 Power Consumption

### Typical Power Usage
- **USB Power**: ~200-300mA at 5V (LEDs at 50% brightness)
- **Battery Power**: ~150-200mA at 4.5V (LEDs at 25% brightness)
- **Standby** (LEDs off): ~50mA

### Battery Life Estimation
With 3x AAA alkaline batteries (2500mAh capacity):
- **Continuous operation**: 12-16 hours
- **With light sensor**: 24-48 hours (depending on usage pattern)

## 🤝 Contributing

Contributions are welcome! Please feel free to submit pull requests or open issues for:
- New LED patterns
- Additional Christmas songs
- Hardware improvements
- Bug fixes
- Documentation updates

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🎁 Acknowledgments

- **FastLED Library** - For excellent LED control capabilities
- **Christmas Songs Collection** - Based on work by Andy Tran (MIT License)
- **ESP32 Community** - For comprehensive documentation and examples

## 📞 Support

If you encounter any issues or have questions:
1. Check the [Issues](https://github.com/yourusername/christmas-pcb/issues) section
2. Review the serial monitor output for diagnostic information
3. Ensure all hardware connections are correct
4. Verify library dependencies are installed

---

**🎄 Happy Holidays! 🎄**

*Made with ❤️ for the Christmas spirit*