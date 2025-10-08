# Data Upload Instructions for ESP8266

This project requires HTML, CSS, and JavaScript files to be uploaded to the ESP8266's LittleFS filesystem.

## 📁 Files Structure

The `data/` folder contains:
- `index.html` - Main web interface
- `styles.css` - All CSS styles
- `app.js` - JavaScript functionality

## 🔧 Method 1: Using Arduino IDE Plugin (Recommended)

### Step 1: Install LittleFS Upload Plugin

1. Download the plugin:
   - Go to: https://github.com/earlephilhower/arduino-esp8266littlefs-plugin/releases
   - Download the latest `.zip` file

2. Install the plugin:
   ```bash
   # Create tools directory if it doesn't exist
   mkdir -p ~/Arduino/tools/
   
   # Extract the downloaded zip to this location
   unzip ESP8266LittleFS-*.zip -d ~/Arduino/tools/
   ```

3. Restart Arduino IDE

### Step 2: Upload Files to ESP8266

1. Open `Parola_WifiMatrix.ino` in Arduino IDE
2. Connect your ESP8266 via USB
3. Select your board and port:
   - **Tools** → **Board** → **NodeMCU 1.0 (ESP-12E Module)** (or your board)
   - **Tools** → **Port** → Select your COM/ttyUSB port
4. Configure filesystem:
   - **Tools** → **Flash Size** → Select size with FS (e.g., "4MB (FS:2MB OTA:~1019KB)")
5. Upload files:
   - **Tools** → **ESP8266 LittleFS Data Upload**
6. Wait for "LittleFS Image Uploaded" message

### Step 3: Upload Sketch

After uploading the data files:
1. Click **Upload** button (or Ctrl+U)
2. Wait for sketch to compile and upload
3. Open Serial Monitor (115200 baud) to see boot messages

## 🔧 Method 2: Using Command Line (Advanced)

### Prerequisites
```bash
pip install esptool
```

### Create LittleFS Image
```bash
# Install mklittlefs tool
wget https://github.com/earlephilhower/mklittlefs/releases/download/3.0.0/x86_64-linux-gnu-mklittlefs-295fe9b.tar.gz
tar -xzf x86_64-linux-gnu-mklittlefs-295fe9b.tar.gz

# Create filesystem image (2MB size)
./mklittlefs -c data/ -s 0x200000 littlefs.bin
```

### Upload to ESP8266
```bash
# Find your port (usually /dev/ttyUSB0 or COM3)
ls /dev/ttyUSB*

# Upload filesystem (offset depends on flash size - 0x200000 for 4MB)
esptool.py --port /dev/ttyUSB0 --baud 460800 write_flash 0x200000 littlefs.bin
```

## 🔧 Method 3: Using PlatformIO

If using PlatformIO:

1. Add to `platformio.ini`:
   ```ini
   [env:nodemcuv2]
   platform = espressif8266
   board = nodemcuv2
   framework = arduino
   board_build.filesystem = littlefs
   ```

2. Upload filesystem:
   ```bash
   pio run --target uploadfs
   ```

3. Upload code:
   ```bash
   pio run --target upload
   ```

## ✅ Verification

After uploading, access the web interface:

1. **First Boot (AP Mode)**:
   - Connect to WiFi: `Parola` (password: `parola123`)
   - Open browser: `http://192.168.4.1`
   - Login: `admin` / `admin`

2. **After WiFi Configuration**:
   - Find device IP on your network
   - Open browser: `http://[device-ip]`
   - Login: `admin` / `admin`

## 🐛 Troubleshooting

### "File Not Found" Error
- **Cause**: Web files not uploaded to LittleFS
- **Solution**: Follow upload steps above

### "LittleFS mount failed"
- **Cause**: Filesystem not formatted or corrupted
- **Solution**: Re-upload data files using plugin

### Plugin Not Showing in Tools Menu
- **Cause**: Plugin not installed correctly
- **Solution**: Check `~/Arduino/tools/` contains the plugin folder

### Serial Monitor Shows "404 Not Found"
- **Cause**: Files uploaded but with wrong names
- **Solution**: Ensure files in `data/` folder are named exactly:
  - `index.html`
  - `styles.css`
  - `app.js`

## 📝 Important Notes

⚠️ **Always upload data files BEFORE or AFTER sketch upload, not during**

⚠️ **Data upload will ERASE existing filesystem data**

⚠️ **Ensure correct Flash Size is selected** - Must have FS space allocated

✅ **Data files are persistent** - They survive sketch updates

✅ **Only need to upload once** - Unless you change HTML/CSS/JS files

## 🔄 Updating Web Files Only

To update just the web interface without reflashing the sketch:

1. Modify files in `data/` folder
2. Run **ESP8266 LittleFS Data Upload**
3. No need to upload sketch again

## 📚 Additional Resources

- [LittleFS Plugin Documentation](https://github.com/earlephilhower/arduino-esp8266littlefs-plugin)
- [ESP8266 Arduino Core Docs](https://arduino-esp8266.readthedocs.io/en/latest/filesystem.html)
- [PlatformIO LittleFS](https://docs.platformio.org/en/latest/platforms/espressif8266.html#uploading-files-to-filesystem)