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

### Step 3: Install LittleFS Upload Plugin

This project requires web files (HTML, CSS, JavaScript) to be uploaded to the ESP8266's filesystem.

1. **Download the LittleFS Upload Plugin:**
   - Go to: https://github.com/earlephilhower/arduino-esp8266littlefs-plugin/releases
   - Download the latest `.zip` file

2. **Install the plugin:**
   ```bash
   # Create tools directory if it doesn't exist
   mkdir -p ~/Arduino/tools/
   
   # Extract the downloaded zip to this location
   unzip ESP8266LittleFS-*.zip -d ~/Arduino/tools/
   ```

3. **Restart Arduino IDE**

### Step 4: Hardware Connections

Connect your LED matrix to ESP8266:

| ESP8266 Pin | MAX7219 Pin | Wire Color | Description |
|-------------|-------------|------------|-------------|
| D5          | CLK         | Yellow     | Clock Signal |
| D7          | DIN         | Green      | Data Input |
| D8          | CS          | Blue       | Chip Select |
| 3.3V        | VCC         | Red        | Power |
| GND         | GND         | Black      | Ground |

### Step 5: Configure the Code

1. Download the project files
2. Open `parola_matrix.ino` in Arduino IDE
3. Update configuration variables:

```cpp
// WiFi Configuration
const char *ssid = "YourWiFiNetwork";
const char *password = "YourWiFiPassword";

// Admin Credentials
const char *adminUser = "admin"; 
const char *adminPass = "your_secure_password";
```

### Step 6: Upload Web Files to Filesystem

**IMPORTANT:** Upload these files BEFORE uploading the sketch code.

1. Open `Parola_WifiMatrix.ino` in Arduino IDE
2. Connect your ESP8266 via USB
3. Configure filesystem size:
   - **Tools** → **Flash Size** → Select size with FS (e.g., "4MB (FS:2MB OTA:~1019KB)")
4. Upload data files:
   - **Tools** → **ESP8266 LittleFS Data Upload**
5. Wait for "LittleFS Image Uploaded" message

**Files being uploaded from `data/` folder:**
- `index.html` - Main web interface
- `styles.css` - All CSS styles  
- `app.js` - JavaScript functionality

### Step 7: Upload the Sketch Code

1. Connect ESP8266 to computer via USB
2. Select board: Tools â†’ Board â†’ ESP8266 Boards â†’ (your board)
3. Select port: Tools â†’ Port â†’ (your port)
4. Click Upload button (arrow icon)

### Step 8: First Boot and Testing

1. Open Serial Monitor (9600 baud)
2. Press reset button on ESP8266
3. Wait for WiFi connection message
4. Note the IP address displayed
5. Open web browser and navigate to the IP address

### Step 9: Access Web Interface

**First Boot (AP Mode):**
If no WiFi credentials are set or connection fails:
1. Device creates WiFi network: `Parola` (password: `parola123`)
2. Connect to this network
3. Open browser: `http://192.168.4.1`
4. Login with: `admin` / `admin`
5. Configure your WiFi in the WiFi Configuration section

**After WiFi Configuration:**
1. Find device IP on your network (check Serial Monitor or router)
2. Open browser: `http://[device-ip]`
3. Login with: `admin` / `admin`

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
- Device will automatically start in AP mode if connection fails

**"File Not Found" error in web browser:**
- Web files not uploaded to LittleFS
- Solution: Upload data files using **Tools** → **ESP8266 LittleFS Data Upload**
- Ensure `data/` folder contains: index.html, styles.css, app.js

**"LittleFS mount failed" in Serial Monitor:**
- Filesystem not formatted or corrupted
- Solution: Re-upload data files using plugin
- Check Flash Size has FS space allocated

**LittleFS Plugin not showing in Tools menu:**
- Plugin not installed correctly
- Solution: Check `~/Arduino/tools/` contains the plugin folder
- Restart Arduino IDE after installation

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
board_build.filesystem = littlefs
lib_deps = 
    majicdesigns/MD_Parola@^3.7.0
    majicdesigns/MD_MAX72XX@^3.4.0
    arduino-libraries/NTPClient@^3.2.1
```

Upload filesystem and code:
```bash
# Upload web files to filesystem
pio run --target uploadfs

# Upload sketch
pio run --target upload
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
   - Shows "Welcome to Parola_" message
   - Display should be clearly visible

3. **Web Interface:**
   - Accessible at ESP8266 IP address
   - Login page should appear
   - Dashboard loads after authentication

## Important Notes

⚠️ **Upload Order Matters:**
1. First: Upload data files (LittleFS)
2. Then: Upload sketch code

⚠️ **Data files are persistent** - They survive sketch updates

✅ **Only need to upload data files once** - Unless you change HTML/CSS/JS files

✅ **To update only web interface** - Just re-upload data files, no need to reflash sketch

## Next Steps

- [Web Interface Usage](README.md#web-interface)
- [WiFi Configuration Guide](README.md#-first-time-setup-ap-mode)
- [API Documentation](README.md#-api-reference)
