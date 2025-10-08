
# ESP8266 WiFi LED Matrix Display

<div align="center">

![Project Status](https://img.shields.io/badge/Status-Active-brightgreen)
![License](https://img.shields.io/badge/License-MIT-blue)
![Platform](https://img.shields.io/badge/Platform-ESP8266-orange)
![Arduino](https://img.shields.io/badge/Arduino-Compatible-blue)

**A professional WiFi-controlled LED matrix display system with web interface, OTA updates, and comprehensive security features**

[Features](#-features) • [Installation](#-installation) • [Configuration](#-configuration) • [API](#-api-reference) • [Contributing](#-contributing)

</div>

## 📚 Table of Contents

- [Overview](#-overview)
- [Features](#-features)
- [Hardware Requirements](#-hardware-requirements)
- [Installation](#-installation)
- [Configuration](#-configuration)
- [Web Interface](#-web-interface)
- [API Reference](#-api-reference)
- [Security Features](#-security-features)
- [Troubleshooting](#-troubleshooting)
- [Contributing](#-contributing)
- [License](#-license)
- [Acknowledgments](#-acknowledgments)

## 🎯 Overview

This project implements a sophisticated WiFi-controlled LED matrix display system using an ESP8266 microcontroller and the MD_Parola library. The system provides a modern web interface for controlling display content, effects, and settings — complete with authentication, over-the-air updates, and robust network management.

### Key Highlights

- **AP Mode Support**: Automatic Access Point mode when no WiFi configured
- **WiFi Configuration**: Easy WiFi setup through web interface  
- **Modern Web Interface**: Responsive design with Material Design icons
- **Secure Authentication**: Session-based login with rate limiting
- **Real-time Control**: Live text updates and effect changes
- **OTA Updates**: Firmware updates via web interface
- **Network Resilience**: Automatic WiFi reconnection with graceful degradation
- **Professional UI**: Clean, intuitive control panel with visual feedback

## ✨ Features

### Display Control

- 🖊️ **Live Text Updates**: Real-time message changes without page refresh
- 🎨 **28 Visual Effects**: Comprehensive animation library including scroll, fade, wipe, and more
- 🔧 **Brightness Control**: 15 levels of brightness adjustment (1–15)
- ⚡ **Speed Control**: 10 speed levels for animations (1–10)
- 🕐 **Auto Clock Display**: Automatically shows clock when message is cleared

### Web Interface

![Web Interface Preview](https://github.com/sddion/Parola_/blob/main/2025.png)

- 🎨 **Modern Design**: Material Design UI  
- 📱 **Mobile Friendly**: Responsive for multiple screen sizes  
- 🔒 **Secure Login**: Authenticated access with session management  
- 👁️ **Live Status**: IP address, uptime, current time, and SSID  
- 🎯 **Interactive Controls**: Effect grid, sliders, input fields  
- 🔁 **Reset Button**: One-click factory reset

### Security & Management

- 🛡️ **IP Rate Limiting**: Blocks repeated failed login attempts
- 🔐 **Bearer Token Login**: Secure, expirable sessions
- ⏱️ **Session Timeout**: Auto logout after inactivity
- 🧰 **OTA Firmware Upload**: In-browser firmware upgrade
- 📊 **Live Monitoring**: Real-time system feedback

### Network Features

- 🏠 **Access Point Mode**: Automatic AP mode when no WiFi configured
- 📶 **Auto WiFi Reconnect**: Resumes stable operation after drops
- ⚙️ **WiFi Configuration**: Set WiFi credentials through web interface
- 🌐 **NTP Clock Sync**: Accurate time from online servers
- 🔍 **IP Reporting**: Visible on dashboard
- 🧩 **Fail-safe Display Suspension**: Clears matrix when disconnected

## 🛠️ Hardware Requirements

### Components

- ✅ ESP8266 Board (e.g., NodeMCU, Wemos D1 Mini)
- ✅ 4 x 8x8 LED Matrix with MAX7219 (FC16)
- ✅ Jumper wires
- ✅ 5V Power Supply (min 2A)

### Wiring Table

| ESP8266 Pin | MAX7219 Pin | Purpose         |
|-------------|-------------|-----------------|
| D5          | CLK         | Clock signal     |
| D7          | DIN         | Serial data in   |
| D8          | CS          | Chip select      |
| 3.3V        | VCC         | Power input      |
| GND         | GND         | Ground connection|

### Configuration Notes

- `HARDWARE_TYPE` should be `MD_MAX72XX::FC16_HW`
- `MAX_DEVICES` = 4 (for 4 matrix modules)
- Typical layout = 32x8 display area

## 🚀 Installation

### Quick Installation (Pre-compiled Binary)

For quick setup, you can flash the pre-compiled binary:

1. **Download ESP Flash Tool** or use **esptool.py**
2. **Use the provided binary**: `esp8266.esp8266.nodemcuv2/sketch_oct08_parola.ino.bin`
3. **Flash command**:
   ```bash
   esptool.py --port COM3 --baud 460800 write_flash --flash_size=detect 0 sketch_oct08_parola.ino.bin
   ```

### Development Installation

#### Requirements

- Arduino IDE 1.8+ / PlatformIO
- Installed ESP8266 Board Package
- Required Libraries

#### Required Libraries

Install via Arduino Library Manager:

```
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ArduinoOTA.h>
#include <EEPROM.h>
#include <MD_Parola.h>
#include <MD_MAX72XX.h>
#include <SPI.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
```

### Setup Steps

```
git clone https://github.com/sadion/Parola_.git
```

- Open `.ino` in IDE
- Connect board, select COM port
- Upload and open Serial Monitor (9600 baud)

## ⚙️ Configuration

### First Time Setup (AP Mode)

When first powered on or if WiFi credentials are not set:

1. **Device creates WiFi network**: `Parola` (Password: `parola123`)
2. **Connect to AP**: Join the "Parola" network from your device
3. **Open web browser**: Navigate to `http://192.168.4.1`
4. **Login**: Use default credentials
   - **Username**: `admin`
   - **Password**: `admin`
5. **Configure WiFi**: Use the WiFi Configuration section to set your network
6. **Device restarts**: Automatically connects to your WiFi network

### Default Credentials

- **WiFi AP**: `Parola` / `parola123` 
- **Web Login**: `admin` / `admin`
- **AP Mode IP**: `192.168.4.1`

### Manual Configuration (Development)

Edit in source code if compiling yourself:

```cpp
const char* ssid = "YourNetwork";
const char* password = "YourPassword";

const char* adminUser = "admin";
const char* adminPass = "admin";
```

### Display Setup

```
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define CLK_PIN D5
#define DATA_PIN D7
#define CS_PIN D8
```

### 🔁 Reset to Defaults

A built-in **Reset** button is located in the web dashboard (top-right corner beside Logout).

**What it resets:**

- Brightness to `5`
- Speed to `5`
- Effect to `'No Effect'`
- Message to default `"Welcome to Parola_"` (or empty)
- Automatically shows real-time clock when empty

> 💡 Tip: Use this feature for easily returning the device to a fresh state — no reflash required.

## 🖥️ Web Interface

### Access Guide

#### First Time (AP Mode)
1. Connect to WiFi network `Parola` (password: `parola123`)
2. Open `http://192.168.4.1` in browser
3. Login with `admin` / `admin`
4. Configure your WiFi in the WiFi Configuration section

#### Normal Operation  
1. Open device IP in browser (shown on your router's connected devices)
2. Login with web credentials (`admin` / `admin`)
3. Use dashboard controls to update message, change effects, or reset

### Dashboard Sections

- **Status Cards**: Time, Uptime, IP, SSID, Connection Mode
- **WiFi Configuration**: Set WiFi credentials (visible in AP mode or when needed)
- **Effect Gallery**: 28 visual animations with Material Icons
- **Sliders**: Brightness (1–15), Speed (1–10)
- **Message Input**: Real-time text updates (max 63 chars)
- **OTA Updater**: Upload and replace firmware (.bin files)
- **Reset Button**: One-click restore to default settings

## 📡 API Reference

**Base URLs:**
- AP Mode: `http://192.168.4.1`
- WiFi Mode: `http://[device-ip]`

All API calls require a valid token:

### Authentication

```
POST /login
Content-Type: application/x-www-form-urlencoded

username=admin&password=admin123
```

On success:

```
{
  "token": "your_token_string"
}
```

Use in Authorization header:

```
Authorization: Bearer your_token_string
```

### Endpoints

| Endpoint       | Method | Description              |
|----------------|--------|--------------------------|
| `/status`      | GET    | Fetch current settings   |
| `/setWifi`     | POST   | Configure WiFi (restarts)|
| `/setBright`   | POST   | Set brightness           |
| `/setSpeed`    | POST   | Set animation speed      |
| `/setEffect`   | POST   | Select effect            |
| `/setMessage`  | POST   | Update display text      |
| `/update`      | POST   | Upload OTA firmware      |

## 🎨 Effect Indexes

| Index | Effect Name       |
|-------|------------------|
| 0     | No Effect         |
| 1     | Print             |
| 2     | Scroll Up         |
| 3     | Scroll Down       |
| 4     | Scroll Left       |
| 5     | Scroll Right      |
| 6     | Slice             |
| 7     | Mesh              |
| 8     | Fade              |
| 9     | Dissolve          |
| 10    | Blinds            |
| 11    | Random            |
| 12    | Wipe              |
| 13    | Wipe Cursor       |
| 14    | Scan Horiz        |
| 15    | Scan HorizX       |
| 16    | Scan Vert         |
| 17    | Scan VertX        |
| 18    | Opening           |
| 19    | Opening Cursor    |
| 20    | Closing           |
| 21    | Closing Cursor    |
| 22    | Scroll Up Left    |
| 23    | Scroll Up Right   |
| 24    | Scroll Down Left  |
| 25    | Scroll Down Right |
| 26    | Grow Up           |
| 27    | Grow Down         |

## 🔐 Security Features

- IP-based login lockout (3 failures → blocked for 30s)
- Session token expires after 10 minutes
- Input sanitization prevents code injection
- Device reconnection logic ensures safe sleep/display clear

## 🧯 Troubleshooting

| Issue                     | Solution                        |
|---------------------------|---------------------------------|
| “Matrix not lighting up”  | Check wiring & HARDWARE_TYPE    |
| “WiFi not connecting”     | Confirm SSID, password           |
| “Web UI unreachable”      | Confirm IP via Serial Monitor    |
| “Login blocked”           | Wait 30s timeout & retry         |
| “Display garbled”         | Reduce speed, check power supply|

Use `Serial.begin(9600);` for basic logging.

## 🙌 Contributing

We welcome PRs & community feedback!

### ⭐ How to Help

- Report bugs 🐞
- Add new effects ✨
- Improve docs 📖
- Test ESP8266/ESP32 boards 🔬

### Dev Setup

1. Fork repository
2. `git checkout -b feature-xyz`
3. Test on board
4. PR with meaningful commit log

## 📄 License

Licensed under MIT.

✔ Free for commercial or personal use  
✔ Modify and fork as needed  
🚫 Must retain license notice

## 🙏 Acknowledgments

- [MD_Parola](https://github.com/MajicDesigns/MD_Parola)
- [ESP8266 Arduino Core](https://github.com/esp8266/Arduino)
- [Google Fonts & Icons](https://fonts.google.com/icons)

---

<div align="center">

Made with ❤️ by [@wordwires](https://github.com/wordwires)  
⭐ Star this repo if it helped you!

[🐛 Report Bug](https://github.com/sadion/esp8266-parola-wifi-matrix/issues) • [💡 Request Feature](https://github.com/sadion/esp8266-parola-wifi-matrix/issues) • [💬 Discussions](https://github.com/sadion/esp8266-parola-wifi-matrix/discussions)

</div>
