# ESP8266 WiFi LED Matrix Display 🤯💡

<div align="center">

![Project Status](https://img.shields.io/badge/Status-Active-brightgreen)
![License](https://img.shields.io/badge/License-MIT-blue)
![Platform](https://img.shields.io/badge/Platform-ESP8266-orange)
![Arduino](https://img.shields.io/badge/Arduino-Compatible-blue)

**Because why settle for boring LED displays when you can have WiFi-controlled chaos?**

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
- [Release & Binary](#-release--direct-binary)

## 🎯 Overview

This project lets your ESP8266 control an LED matrix display like a boss. Features a web interface, OTA updates, real-time text updates, and more security than your email account. All without breaking your fragile little heart.  

### Key Highlights

- **AP Mode**: Because configuring WiFi is too mainstream  
- **Modern Web Interface**: Looks fancy, works… mostly  
- **Secure Authentication**: Login sessions and rate-limits, because hackers exist  
- **Live Updates**: Messages and effects change without your intervention (or consent)  
- **OTA Updates**: Flash firmware without touching your precious ESP8266  
- **Network Resilience**: It tries to reconnect when your WiFi dies, like a clingy ex  

## ✨ Features

### Display Control

- 🖊️ **Live Text Updates** – Type, it shows, no magic involved  
- 🎨 **28 Visual Effects** – Because 1 effect is for quitters  
- 🔧 **Brightness Control** – 15 levels of blinding glory  
- ⚡ **Speed Control** – Slow to insane, your choice  
- 🕐 **Auto Clock** – Because you clearly need reminders  

### Web Interface

![Web Interface Preview](https://github.com/sddion/Parola_/blob/main/2025.png)

- 🎨 Material Design UI (fancy icons for fancy people)  
- 📱 Works on mobile, because tiny screens are fun  
- 🔒 Login protected, because trolls exist  
- 👁️ Live status, IP, uptime, SSID, because you need to flex  

### Security & Management

- 🛡️ IP rate-limiting – Three wrong logins? See ya for 30s  
- 🔐 Bearer token login – Fancy words, same result  
- ⏱️ Auto logout – So lazy people don’t hog sessions  
- 🧰 OTA Upload – Upgrade firmware without leaving your couch  

### Network Features

- 🏠 Automatic AP Mode  
- 📶 Auto WiFi Reconnect  
- ⚙️ Web-based WiFi Configuration  
- 🌐 NTP Clock Sync – Because your ESP8266 cares about punctuality  
- 🔍 IP Reporting – Flex your local network  
- 🧩 Fail-safe Display Suspension – Clears screen when disconnected  

## 🛠️ Hardware Requirements

- ✅ ESP8266 (NodeMCU, Wemos D1 Mini)  
- ✅ 4x MAX7219 8x8 LED matrix (FC16)  
- ✅ Jumper wires  
- ✅ 5V 2A+ power supply  

| ESP8266 Pin | MAX7219 Pin | Purpose       |
|-------------|-------------|---------------|
| D5          | CLK         | Clock signal  |
| D7          | DIN         | Data input    |
| D8          | CS          | Chip select   |
| 3.3V        | VCC         | Power         |
| GND         | GND         | Ground        |

**Hint:** Mess up wiring, you get… nothing.  

## 🚀 Installation

### Quick Flash (Pre-compiled `.bin`)

Because why compile when you can download?  

1. Download ESP Flash Tool or esptool.py  
2. Grab the binary: [Direct Download](https://github.com/sddion/Parola_/releases/download/v1/parola_v1.ino.bin)  
3. Flash command:  

```bash
esptool.py --port COM3 --baud 460800 write_flash --flash_size=detect 0 parola_v1.ino.bin
````

### Development Installation

1. Arduino IDE 1.8+ or PlatformIO
2. ESP8266 Board Package installed
3. Libraries installed:

```cpp
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

4. Open `.ino`, connect board, upload

## ⚙️ Configuration

* Default AP: `Parola` / `parola123`
* Web Login: `admin` / `admin@parola`
* Manual: edit `ssid`, `password`, `adminUser`, `adminPass` in code

### Display Setup

```cpp
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define CLK_PIN D5
#define DATA_PIN D7
#define CS_PIN D8
```

Reset button resets brightness, speed, effects, and shows clock. Because sometimes, we need a clean slate.

## 🖥️ Web Interface

* Connect to AP `Parola` → open browser → login
* Dashboard: Status, WiFi, Effects, Sliders, OTA, Reset
* Mobile-friendly, because your thumbs deserve it

## 📡 API Reference

* Base URL: `http://192.168.4.1` (AP) or `http://[device-ip]` (WiFi)
* Login, set brightness, speed, effect, messages, and OTA
* Tokens required because we’re fancy

## 🔐 Security Features

* IP login lockout
* Session token expiration
* Input sanitization
* Automatic reconnection

## 🧯 Troubleshooting

| Issue               | Fix                         |
| ------------------- | --------------------------- |
| Matrix not lighting | Check wiring, HARDWARE_TYPE |
| WiFi not connecting | Confirm SSID/password       |
| Web UI unreachable  | Check IP via Serial Monitor |
| Login blocked       | Wait 30s, retry             |
| Display garbled     | Reduce speed, check power   |

## 🎨 Effect Indexes

0–27: Various scrolls, fades, wipes… basically everything except magic.

## 🙌 Contributing

* PRs, effects, bug reports, docs improvements
* Fork → branch → commit → PR

## 📄 License

MIT. Free to modify, commercialize, and brag. Retain license notice if you want to be legal-ish.

## 🙏 Acknowledgments

* [MD_Parola](https://github.com/MajicDesigns/MD_Parola)
* [ESP8266 Arduino Core](https://github.com/esp8266/Arduino)
* [Google Fonts & Icons](https://fonts.google.com/icons)

---

## 📦 Release & Direct Binary

* GitHub Release: [v1](https://github.com/sddion/Parola_/releases/tag/v1)
* Direct `.bin` Download: [parola_v1.ino.bin](https://github.com/sddion/Parola_/releases/download/v1/parola_v1.ino.bin)

> Flash this and pretend you know what you’re doing.

<div align="center">

Made with ❤️ by [@sddion](https://github.com/sddion)
⭐ Star this repo if it didn’t blow your mind

[🐛 Report Bug](https://github.com/sddion/Parola_/issues) • [💡 Request Feature](https://github.com/sddion/Parola_/issues) • [💬 Discussions](https://github.com/sddion/Parola_/discussions)

</div>
