# ESP8266 WiFi LED Matrix Display

<div align="center">

![Project Status](https://img.shields.io/badge/Status-Active-brightgreen)
![License](https://img.shields.io/badge/License-MIT-blue)
![Platform](https://img.shields.io/badge/Platform-ESP8266-orange)
![Arduino](https://img.shields.io/badge/Arduino-Compatible-blue)

**A professional WiFi-controlled LED matrix display system with web interface, OTA updates, and comprehensive security features**

[Features](#-features) â€¢ [Installation](#-installation) â€¢ [Configuration](#-configuration) â€¢ [API](#-api-reference) â€¢ [Contributing](#-contributing)

</div>

## ðŸ“‹ Table of Contents

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

## ðŸŽ¯ Overview

This project implements a sophisticated WiFi-controlled LED matrix display system using an ESP8266 microcontroller and the MD_Parola library. The system provides a modern web interface for controlling display content, effects, and settings, complete with authentication, over-the-air updates, and robust network management.

### Key Highlights

- **Modern Web Interface**: Responsive design with Material Design icons
- **Secure Authentication**: Session-based login with rate limiting
- **Real-time Control**: Live text updates and effect changes
- **OTA Updates**: Firmware updates via web interface
- **Network Resilience**: Automatic WiFi reconnection with graceful degradation
- **Professional UI**: Clean, intuitive control panel with visual feedback

## âœ¨ Features

### Display Control
- ðŸ“ **Live Text Updates**: Real-time message changes without page refresh
- ðŸŽ¨ **28 Visual Effects**: Comprehensive animation library including scroll, fade, wipe, and more
- ðŸ”† **Brightness Control**: 15 levels of brightness adjustment (1-15)
- âš¡ **Speed Control**: 10 speed levels for animations (1-10)
- ðŸ• **Auto Clock Display**: Automatic time display when no custom message is set

### Web Interface
- ðŸŽ¨ **Modern Design**: Material Design UI with responsive layout
- ðŸ“± **Mobile Friendly**: Optimized for desktop and mobile devices
- ðŸ”’ **Secure Login**: Username/password authentication with session management
- ðŸ‘ï¸ **Real-time Status**: Live updates of time, IP, uptime, and network status
- ðŸŽ›ï¸ **Interactive Controls**: Slider controls for brightness and speed
- ðŸŽ¯ **Effect Gallery**: Visual grid of all available text effects

### Security & Management
- ðŸ›¡ï¸ **Rate Limiting**: Prevents brute force attacks with IP-based blocking
- ðŸ”‘ **Session Management**: Secure token-based authentication
- â° **Auto-logout**: 10-minute session timeout for security
- ðŸ”„ **OTA Updates**: Secure firmware updates via web interface
- ðŸ“Š **System Monitoring**: Real-time system status and diagnostics

### Network Features
- ðŸ“¡ **WiFi Management**: Automatic connection with reconnection handling
- ðŸŒ **NTP Time Sync**: Automatic time synchronization
- ðŸ“ **Network Discovery**: Displays current IP and network information
- ðŸ”„ **Graceful Degradation**: Display management during network outages

## ðŸ”§ Hardware Requirements

### Essential Components
- **ESP8266 Development Board** (NodeMCU, Wemos D1 Mini, etc.)
- **LED Matrix Display** (4x MAX7219/MAX7221 modules, FC16 compatible)
- **Connecting Wires** (Dupont/Jumper wires)
- **Power Supply** (5V, minimum 2A recommended)

### Wiring Connections

| ESP8266 Pin | MAX7219 Pin | Description |
|-------------|-------------|-------------|
| D5          | CLK         | Clock Signal |
| D7          | DIN         | Data Input |
| D8          | CS          | Chip Select |
| 3.3V        | VCC         | Power (3.3V) |
| GND         | GND         | Ground |

### Hardware Configuration
- **Matrix Type**: FC16_HW (configurable in code)
- **Matrix Count**: 4 modules (configurable)
- **Display Size**: 32x8 pixels total (4 modules Ã— 8x8)

## ðŸš€ Installation

### Prerequisites
1. **Arduino IDE** (version 1.8.0 or later) or **PlatformIO**
2. **ESP8266 Arduino Core** (latest version)
3. **Required Libraries** (see Library Dependencies section)

### Library Dependencies

Install the following libraries through Arduino IDE Library Manager:

```cpp
// Core Libraries
#include <ESP8266WiFi.h>           // ESP8266 WiFi support
#include <ESP8266WebServer.h>      // Web server functionality
#include <ESP8266HTTPUpdateServer.h> // OTA update server
#include <ArduinoOTA.h>            // Over-the-air updates
#include <EEPROM.h>                // Settings persistence

// Display Libraries
#include <MD_Parola.h>             // Text display and effects
#include <MD_MAX72XX.h>            // MAX72xx hardware control
#include <SPI.h>                   // SPI communication

// Time and Network
#include <NTPClient.h>             // Network time protocol
#include <WiFiUdp.h>               // UDP communication
```

### Installation Steps

1. **Clone the Repository**
   ```bash
   git clone https://github.com/sadion/esp8266-parola-wifi-matrix.git
   cd esp8266-parola-wifi-matrix
   ```

2. **Install Libraries**
   - Open Arduino IDE
   - Go to **Tools â†’ Manage Libraries**
   - Search and install each required library

3. **Configure Hardware**
   - Connect your LED matrix according to the wiring diagram
   - Verify all connections are secure

4. **Upload the Code**
   - Open `parola_matrix.ino` in Arduino IDE
   - Select your ESP8266 board from **Tools â†’ Board**
   - Select the correct port from **Tools â†’ Port**
   - Click **Upload**

## âš™ï¸ Configuration

### Network Configuration

Update the following variables in the code:

```cpp
// WiFi Network Credentials
const char *ssid = "YourWiFiNetwork";        // Your WiFi network name
const char *password = "YourWiFiPassword";   // Your WiFi password

// Admin Credentials
const char *adminUser = "admin";             // Web interface username
const char *adminPass = "your_password";     // Web interface password
```

### Display Settings

```cpp
// Hardware Configuration
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW    // Matrix hardware type
#define MAX_DEVICES 4                        // Number of matrix modules
#define CLK_PIN D5                           // Clock pin
#define DATA_PIN D7                          // Data pin
#define CS_PIN D8                            // Chip select pin
```
#### 🔁 Reset to Default Button

You can reset all settings directly from the web interface using the **Reset** button located in the top-right dashboard header (beside the Logout icon).

**What the Reset button does:**
- Clears any saved text message
- Resets brightness, scroll speed, and display effect to defaults
- Applies settings immediately
- Returns display to clock-only mode if no message is set

> 💡 Tip: When the message is empty, the LED matrix switches to show the real-time clock automatically—sourced via NTP.

This makes it easy to restore your device to a known, clean state without needing to reflash firmware or manually clear memory.

## ðŸ–¥ï¸ Web Interface

### Accessing the Interface

1. **Connect to WiFi**: Ensure your ESP8266 is connected to your network
2. **Find IP Address**: Check your router's admin panel or serial monitor
3. **Open Browser**: Navigate to `http://[ESP8266_IP_ADDRESS]`
4. **Login**: Use your configured admin credentials

### Interface Features

#### Dashboard Sections

1. **Status Cards**
   - Current time with automatic updates
   - Network information (IP address, SSID)
   - System uptime since last reboot

2. **Effect Gallery**
   - Visual grid of all 28 available effects
   - Click any effect to apply instantly
   - Icons represent each effect type

3. **Message Control**
   - Live text input with instant updates
   - 63-character maximum length
   - Automatic sanitization of special characters

4. **Brightness & Speed Controls**
   - Interactive slider controls
   - Visual feedback with color coding
   - Instant application of changes

5. **Firmware Update**
   - Drag-and-drop firmware upload
   - Progress indication
   - Automatic reboot after successful update

## ðŸ“¡ API Reference

### Authentication

All API endpoints (except login) require authentication:

```http
GET /endpoint HTTP/1.1
Authorization: Bearer [session_token]
```

### Endpoints

#### Login
```http
POST /login
Content-Type: application/x-www-form-urlencoded

username=admin&password=your_password
```

**Response:**
```json
{
  "token": "abc123def456"
}
```

#### Get Status
```http
GET /status
Authorization: Bearer [token]
```

**Response:**
```json
{
  "brightness": 5,
  "speed": 5,
  "effect": 0,
  "message": "Hello World",
  "ip": "192.168.1.100",
  "curtime": "02:30:PM",
  "ssid": "MyNetwork",
  "uptime": "01h 23m 45s"
}
```

#### Set Brightness
```http
POST /setBright
Authorization: Bearer [token]
Content-Type: application/x-www-form-urlencoded

value=10
```

#### Set Speed
```http
POST /setSpeed
Authorization: Bearer [token]
Content-Type: application/x-www-form-urlencoded

value=7
```

#### Set Effect
```http
POST /setEffect
Authorization: Bearer [token]
Content-Type: application/x-www-form-urlencoded

value=12
```

#### Set Message
```http
POST /setMessage
Authorization: Bearer [token]
Content-Type: application/x-www-form-urlencoded

value=Hello%20World
```

#### Firmware Update
```http
POST /update
Authorization: Bearer [token]
Content-Type: multipart/form-data

firmware=[binary_file]
```

### Effect Index Reference

| Index | Effect Name | Description |
|-------|-------------|-------------|
| 0 | No Effect | Static text display |
| 1 | Print | Simple print effect |
| 2 | Scroll Up | Text scrolls upward |
| 3 | Scroll Down | Text scrolls downward |
| 4 | Scroll Left | Text scrolls left |
| 5 | Scroll Right | Text scrolls right |
| 6 | Slice | Slicing transition |
| 7 | Mesh | Mesh pattern effect |
| 8 | Fade | Fade in/out effect |
| 9 | Dissolve | Dissolve transition |
| 10 | Blinds | Venetian blind effect |
| 11 | Random | Random pattern |
| 12 | Wipe | Wipe transition |
| 13 | Wipe Cursor | Wipe with cursor |
| 14 | Scan Horiz | Horizontal scan |
| 15 | Scan HorizX | Extended horizontal scan |
| 16 | Scan Vert | Vertical scan |
| 17 | Scan VertX | Extended vertical scan |
| 18 | Opening | Opening curtain |
| 19 | Opening Cursor | Opening with cursor |
| 20 | Closing | Closing curtain |
| 21 | Closing Cursor | Closing with cursor |
| 22 | Scroll Up Left | Diagonal scroll up-left |
| 23 | Scroll Up Right | Diagonal scroll up-right |
| 24 | Scroll Down Left | Diagonal scroll down-left |
| 25 | Scroll Down Right | Diagonal scroll down-right |
| 26 | Grow Up | Growing upward |
| 27 | Grow Down | Growing downward |

## ðŸ” Security Features

### Authentication System
- **Session-based Authentication**: Secure token generation and validation
- **Session Timeout**: Automatic logout after 10 minutes of inactivity
- **Rate Limiting**: Maximum 3 login attempts per IP address
- **Temporary Blocking**: 30-second block after failed attempts

### Network Security
- **Input Sanitization**: All user inputs are sanitized to prevent injection
- **Memory Management**: Automatic cleanup of old login attempt records
- **Secure Headers**: Proper HTTP headers for security

### Data Protection
- **EEPROM Storage**: Settings securely stored in non-volatile memory
- **Network Isolation**: System continues operating during network outages
- **Graceful Degradation**: Display functions maintained during connectivity issues

## ðŸ”§ Troubleshooting

### Common Issues

#### WiFi Connection Problems
**Symptoms**: Device not connecting to WiFi
**Solutions**:
- Verify SSID and password in code
- Check WiFi signal strength
- Ensure 2.4GHz network (ESP8266 doesn't support 5GHz)
- Try power cycling the device

#### Display Not Working
**Symptoms**: LED matrix not showing anything
**Solutions**:
- Verify wiring connections
- Check power supply (5V, 2A minimum)
- Confirm hardware type setting (`HARDWARE_TYPE`)
- Test with basic MD_Parola examples

#### Web Interface Not Loading
**Symptoms**: Cannot access web interface
**Solutions**:
- Find correct IP address (check serial monitor)
- Verify device is on same network
- Clear browser cache
- Try different browser

#### Login Issues
**Symptoms**: Cannot login to web interface
**Solutions**:
- Verify admin credentials in code
- Wait 30 seconds if blocked by rate limiting
- Check browser developer console for errors
- Restart device to reset sessions

### Debug Mode

Enable debug output by monitoring the serial port at 9600 baud:

```cpp
Serial.begin(9600);
```

### Factory Reset

To reset all settings to defaults:
1. Hold the reset button while powering on
2. Or reflash the firmware
3. Settings will return to factory defaults

## ðŸ¤ Contributing

We welcome contributions to improve this project! Here's how you can help:

### Ways to Contribute
- ðŸ› **Bug Reports**: Submit issues with detailed descriptions
- âœ¨ **Feature Requests**: Suggest new functionality
- ðŸ”§ **Code Contributions**: Submit pull requests with improvements
- ðŸ“š **Documentation**: Help improve documentation and examples
- ðŸ§ª **Testing**: Test on different hardware configurations

### Development Setup
1. Fork the repository
2. Create a feature branch: `git checkout -b feature-name`
3. Make your changes
4. Test thoroughly
5. Submit a pull request

### Coding Standards
- Follow Arduino IDE formatting conventions
- Use meaningful variable and function names
- Add comments for complex logic
- Test on actual hardware before submitting

## ðŸ“„ License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

### MIT License Summary
- âœ… Commercial use allowed
- âœ… Modification allowed
- âœ… Distribution allowed
- âœ… Private use allowed
- â— License and copyright notice required

## ðŸ™ Acknowledgments

### Libraries and Frameworks
- **[MD_Parola](https://github.com/MajicDesigns/MD_Parola)** by MajicDesigns - LED matrix text display library
- **[MD_MAX72XX](https://github.com/MajicDesigns/MD_MAX72XX)** by MajicDesigns - MAX72xx hardware control
- **[ESP8266 Arduino Core](https://github.com/esp8266/Arduino)** - ESP8266 Arduino framework
- **[NTPClient](https://github.com/arduino-libraries/NTPClient)** - Network time synchronization

### Design Resources
- **[Material Design Icons](https://fonts.google.com/icons)** - UI icons
- **[Google Fonts](https://fonts.google.com/)** - Typography

### Community
- Arduino Forum community for support and inspiration
- ESP8266 developers for the excellent platform
- Contributors and testers who helped improve this project

---

<div align="center">

**Made with ♥️ by [@wordwires](https://github.com/wordwires)**

â­ **Star this repository if you found it helpful!**

[Report Bug](https://github.com/sadion/esp8266-parola-wifi-matrix/issues) â€¢ [Request Feature](https://github.com/sadion/esp8266-parola-wifi-matrix/issues) â€¢ [Discussions](https://github.com/sadion/esp8266-parola-wifi-matrix/discussions)

</div>
