
# 🎄 ESP32-C3 LED & Music Controller

This project provides hardware and firmware for an ESP32-C3 based Christmas-themed PCB that controls an array of addressable LEDs and plays Christmas songs via a buzzer. It includes local controls via two buttons, an automatic 6-hour timer, battery monitoring, and as an extra: a WiFi-based configuration interface.

***

## ✨ Features

* **LED Control:** Drives **8 WS2812B (or compatible) addressable RGB LEDs** using the **FastLED** library.
* **Christmas Music:** Plays a selection of classic **Christmas songs** through a dedicated **buzzer**.
* **Timer Functionality:** Features a 6-hour "ON" timer that cycles every 24 hours (this can be toggled on/off by holding button 1).
* **Local Control:** Two physical buttons for easy control of patterns, music, and the timer.
* **WiFi Configuration:** A temporary **Access Point (AP) mode** for changing persistent settings via a simple web interface.
* **Ambient Light Sensing:** Support for an LDR (Light Dependent Resistor) to potentially adjust brightness based on the environment. (not implemented)

***

## 🛠️ Hardware Requirements

The firmware is designed for a custom board based on the **ESP32-C3** microcontroller.

| Component | Pin (GPIO) | Description |
| :--- | :--- | :--- |
| **RGB LED Data** | **3** | Data line for the WS2812B LED strip/array (8 LEDs total). |
| **Buzzer/Speaker** | **10** | Output for playing musical tones. |
| **Button 1** | **4** | Multi-function button (Pattern change / Timer toggle). |
| **Button 2** | **5** | Music control (Play/Stop). |
| **Battery Sense** | **0** | Analog input for battery voltage monitoring. |
| **LDR Pin** | **2** | Analog input for ambient light sensing (Light Dependent Resistor). |

***

## ⚙️ Software Setup & Building

This project is built using the **PlatformIO** ecosystem within Visual Studio Code or as a standalone CLI tool.

### Dependencies

The `platformio.ini` file automatically manages these dependencies:

1.  **Espressif 32 Platform**
2.  **Arduino Framework**
3.  **FastLED** (for controlling the addressable LEDs)

### Build Instructions

1.  Install **PlatformIO** (VS Code extension or CLI).
2.  Open the project folder.
3.  Ensure your environment matches the configuration in `platformio.ini`: `[env:esp32-c3-devkitm-1]`.
4.  Use the PlatformIO commands to build and upload the firmware:
    ```bash
    # Build the project
    pio run
    
    # Upload to the device (assuming it is connected)
    pio run --target upload
    ```

***

## 🕹️ Usage & Controls

The device is controlled primarily through the two onboard buttons.

| Control | Action | Function |
| :--- | :--- | :--- |
| **Button 1 (Short Press)** | Press and release quickly. | **Change LED Pattern:** Cycles through the available light effects. |
| **Button 1 (Long Press)** | Hold for 1 second. | **Toggle 6h Timer:** Enables or disables the automatic 6-hour run timer. |
| **Button 2 (Any Press)** | Press to toggle. | **Play/Stop Songs:** Toggles the Christmas music playback. |
| **Both Buttons** | Hold simultaneously for 1 second. | **Toggle WiFi AP Mode:** Enters the configuration mode for web-based settings. |

***

## 🌐 Configuration (WiFi Access Point Mode)

The device supports over-the-air (OTA) configuration using a temporary WiFi Access Point.

### Entering AP Mode

1.  Press and **hold both Button 1 and Button 2 simultaneously** for approximately one second.
2.  The device will disable standard WiFi, enable the Access Point, and print a message to the serial console.

### Web Configuration

1.  On a smartphone or computer, search for the new WiFi network created by the device.
2.  Connect to the Access Point. The AP name will typically be a unique identifier.
3.  Once connected, a **captive portal** should open automatically, or you can navigate to `http://192.168.4.1` in your browser.
4.  The web interface will allow you to modify settings stored in the **ESP32**.

### AP Timeout

For security and power-saving, the WiFi Access Point will automatically **turn off after 5 minutes** (300,000 milliseconds) of inactivity. To re-enable it, repeat the two-button press. Wifi will only work when connected to a USB power source (not using batteries)