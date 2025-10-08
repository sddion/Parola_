# Installation Guide

## Prerequisites

Before installing this ESP8266 WiFi LED Matrix Display project, ensure you have the following:

### Software Requirements

1. **Arduino IDE** (version 1.8.0 or later)
   - Download from [arduino.cc](https://www.arduino.cc/en/software)
   - Or use **PlatformIO** as an alternative

2. **ESP8266 Arduino Core**
   - Install via Arduino IDE Board Manager
   - URL: `https://arduino.esp8266.com/stable/package_esp8266com_index.json`

### Hardware Requirements

- ESP8266 development board (NodeMCU, Wemos D1 Mini, etc.)
- LED Matrix display (4x MAX7219/MAX7221 modules)
- Connecting wires
- 5V power supply (minimum 2A)

## Step-by-Step Installation

### Step 1: Install Arduino IDE and ESP8266 Core

1. Download and install Arduino IDE
2. Open Arduino IDE â†’ File â†’ Preferences
3. Add ESP8266 board manager URL:
   ```
   https://arduino.esp8266.com/stable/package_esp8266com_index.json
   ```
4. Go to Tools â†’ Board â†’ Boards Manager
5. Search for "ESP8266" and install the latest version

### Step 2: Install Required Libraries

Open Arduino IDE and install these libraries via Library Manager:

```cpp
// Required Libraries List
MD_Parola              // LED matrix text display
MD_MAX72XX             // MAX72xx hardware control
NTPClient              // Network time protocol
ArduinoOTA             // Over-the-air updates
WiFiManager (optional) // WiFi configuration helper
```

**Installation via Library Manager:**
1. Tools â†’ Manage Libraries
2. Search for each library name
3. Click "Install" for each library

### Step 3: Hardware Connections

Connect your LED matrix to ESP8266:

| ESP8266 Pin | MAX7219 Pin | Wire Color | Description |
|-------------|-------------|------------|-------------|
| D5          | CLK         | Yellow     | Clock Signal |
| D7          | DIN         | Green      | Data Input |
| D8          | CS          | Blue       | Chip Select |
| 3.3V        | VCC         | Red        | Power |
| GND         | GND         | Black      | Ground |

### Step 4: Configure the Code

1. Download the project files
2. Open `parola_matrix.ino` in Arduino IDE
3. Update configuration variables:

```cpp
// WiFi Configuration
const char *ssid = "YourWiFiNetwork";
const char *password = "YourWiFiPassword";

// Admin Credentials
const char *adminUser = "admin";
const char *adminPass = "admin@parola";
```

### Step 5: Upload the Code

1. Connect ESP8266 to computer via USB
2. Select board: Tools â†’ Board â†’ ESP8266 Boards â†’ (your board)
3. Select port: Tools â†’ Port â†’ (your port)
4. Click Upload button (arrow icon)

### Step 6: First Boot and Testing

1. Open Serial Monitor (9600 baud)
2. Press reset button on ESP8266
3. Wait for WiFi connection message
4. Note the IP address displayed
5. Open web browser and navigate to the IP address

## Troubleshooting Installation

### Common Issues

**ESP8266 not detected:**
- Install CH340/CP2102 USB drivers
- Try different USB cable
- Check board selection in Arduino IDE

**Library compilation errors:**
- Ensure all required libraries are installed
- Update to latest library versions
- Check for conflicting library versions

**Upload failures:**
- Press and hold FLASH button during upload
- Lower upload speed in Tools â†’ Upload Speed
- Try different USB port

**WiFi connection issues:**
- Verify SSID and password
- Ensure 2.4GHz network (ESP8266 doesn't support 5GHz)
- Check signal strength

## Alternative Installation Methods

### PlatformIO Installation

1. Install Visual Studio Code
2. Install PlatformIO extension
3. Create new project with ESP8266 platform
4. Add dependencies to `platformio.ini`:

```ini
[env:nodemcuv2]
platform = espressif8266
board = nodemcuv2
framework = arduino
lib_deps =
    majicdesigns/MD_Parola@^3.7.0
    majicdesigns/MD_MAX72XX@^3.4.0
    arduino-libraries/NTPClient@^3.2.1
```

### Manual Library Installation

If Library Manager fails:

1. Download library ZIP files from GitHub
2. Arduino IDE â†’ Sketch â†’ Include Library â†’ Add .ZIP Library
3. Select downloaded ZIP files

## Verification

After successful installation:

1. **Serial Monitor Output:**
   ```
   Connecting to WiFi...
   WiFi connected. IP: 192.168.1.100
   Server started
   ```

2. **LED Matrix Display:**
   - Shows "Time/Date/Day" message
   - Display should be clearly visible

3. **Web Interface:**
   - Accessible at ESP8266 IP address
   - Login page should appear
   - Dashboard loads after authentication
