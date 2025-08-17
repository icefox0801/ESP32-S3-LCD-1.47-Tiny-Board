# ESP32-S3 Weather Display

A modern weather display system using ESP32-S3-LCD-1.47-Tiny-Board with LVGL graphics and Home Assistant integration.

![ESP32-S3](https://img.shields.io/badge/ESP32-S3-blue)
![LVGL](https://img.shields.io/badge/LVGL-9.3.0-green)
![PlatformIO](https://img.shields.io/badge/PlatformIO-Compatible-orange)
![Status](https://img.shields.io/badge/Build-Successful-brightgreen)

## ✨ Features

- 🎨 **Modern LVGL UI**: Dark theme with colorful weather indicators
- 📶 **WiFi Connectivity**: Secure credential management system
- 🏠 **Home Assistant Integration**: Real-time weather data via REST API
- 🌤️ **Complete Weather Display**: Temperature, humidity, pressure, wind
- 🔧 **Modular Architecture**: Clean, maintainable code structure
- 💡 **PWM Backlight Control**: Adjustable display brightness
- 🔄 **Auto-refresh**: Automatic weather updates every 30-60 seconds

## 🛠️ Hardware

- **Board**: ESP32-S3-LCD-1.47-Tiny-Board
- **Display**: 172x320 ST7789 TFT LCD
- **Memory**: 16MB SPIRAM with Octal SPI
- **Framework**: Arduino ESP32

### Pin Configuration
```
MOSI: 45  │  SCLK: 40  │  CS: 42
DC: 41    │  RST: 39   │  BL: 48 (PWM)
```

## 🚀 Quick Start

### 1. Clone and Configure WiFi
```bash
git clone <your-repo>
cd ESP32-S3-LCD-1.47-Tiny-Board/src/
cp secrets_example.h secrets.h
```

### 2. Edit WiFi Credentials
Edit `src/secrets.h` with your information:
```cpp
#define WIFI_SSID "Your_WiFi_Network"
#define WIFI_PASSWORD "your_password"
#define HA_SERVER_IP "192.168.1.100"
#define HA_BEARER_TOKEN "your_home_assistant_token"
```

### 3. Get Home Assistant Token
1. Open Home Assistant web interface
2. Go to **Profile** → **Long-lived access tokens**
3. Create new token named "ESP32 Weather Display"
4. Copy token to `secrets.h`

**Security Note**:
- `secrets.h` is automatically ignored by git for security
- Never commit real credentials to version control
- Always use the example file as a template

### 4. Build and Upload
```bash
pio run --target upload
```

## 📁 Project Structure

```
src/
├── config.h                 # Main configuration
├── main.cpp                 # Application entry point
├── secrets.h                # Credentials (gitignored)
├── secrets_example.h        # Template for credentials
├── lvgl/                    # LVGL display system
│   ├── lvgl_setup.h/.cpp   # Display initialization
├── ui/                      # User interface components
│   ├── ui_main.h/.cpp      # Main UI components
│   └── ui_weather.h/.cpp   # Weather display UI
├── wifi/                    # WiFi management
│   ├── wifi_setup.h/.cpp   # WiFi connection handling
└── weather/                 # Weather integration
    └── weather_api.h/.cpp  # Home Assistant API client
```

## ⚙️ Detailed Configuration

### WiFi Setup
The WiFi configuration is managed through a secure credential system:

1. **Copy the example file:**
   ```bash
   cd src/
   cp secrets_example.h secrets.h
   ```

2. **Edit secrets.h with your actual credentials:**
   - Replace `YOUR_WIFI_NETWORK_NAME` with your WiFi SSID
   - Replace `YOUR_WIFI_PASSWORD` with your WiFi password
   - Replace `192.168.1.100` with your Home Assistant server IP
   - Replace `YOUR_HOME_ASSISTANT_BEARER_TOKEN_HERE` with your actual HA token

3. **Files in main src/ directory:**
   - `secrets_example.h` - Template with placeholder values (tracked by git)
   - `secrets.h` - Actual credentials (ignored by git)

### LVGL Display Setup
The LVGL system provides professional graphics rendering:

**Display Settings:**
- **Resolution**: 172x320 pixels
- **Controller**: ST7789
- **Interface**: SPI (40MHz)
- **Buffer**: 20 lines (optimized for memory usage)

**Key Features:**
- ✅ **Modular initialization** - Step-by-step setup with error checking
- ✅ **Color testing** - RGB test sequence on startup
- ✅ **Comprehensive logging** - Detailed serial output with status indicators
- ✅ **Optimized SPI** - 40MHz performance settings
- ✅ **Advanced UI** - Screen management, status bar, interactive widgets

**API Functions:**
- `lvgl_setup()` - Complete LVGL initialization
- `ui_create_main_screen()` - Create main application screen
- `ui_create_status_bar()` - Add status bar to current screen
- `ui_switch_to_screen()` - Navigate between screens

**Performance:**
- **Buffer Size**: 20 lines balance memory usage and smoothness
- **SPI Speed**: 40MHz optimized for ESP32-S3
- **Refresh Rate**: ~30Hz with current settings
- **Memory Usage**: ~10KB for LVGL buffer + widget memory

## 🌡️ Weather Data

The system displays weather information from Home Assistant's `weather.forecast_home` entity:

| Data | Description |
|------|-------------|
| **State** | Current weather condition (sunny, cloudy, rainy, etc.) |
| **Temperature** | Current temperature in °C |
| **Humidity** | Relative humidity percentage |
| **Pressure** | Atmospheric pressure in hPa |
| **Wind** | Wind speed and direction |

## 🔒 Security Features

- ✅ WiFi credentials stored in gitignored file
- ✅ Example template provided for easy setup
- ✅ No hardcoded secrets in version control
- ✅ Secure Bearer token authentication with Home Assistant
- ✅ Automatic credential validation

## 🎨 Display Features

- **Dark Theme**: Modern UI design optimized for readability
- **Weather Icons**: Symbolic representation of weather conditions
- **Real-time Updates**: Automatic data refresh with timestamps
- **Status Indicators**: Connection status and last update time
- **Error Handling**: Graceful fallback when data is unavailable

## ⚙️ Technical Stack

| Component | Version | Purpose |
|-----------|---------|---------|
| **LVGL** | 9.3.0 | Graphics and UI framework |
| **ArduinoJson** | 7.x | JSON parsing for API responses |
| **ESP32 HTTPClient** | Latest | REST API communication |
| **PlatformIO** | Latest | Build system and dependency management |

## 🔧 Development

### Build Requirements
- PlatformIO IDE or CLI
- ESP32 Arduino framework
- Git (for version control)

### Memory Usage
- **Flash**: ~1.1MB (36.4% of 3MB)
- **RAM**: ~119KB (36.5% of 327KB)
- **Features**: Hardware CDC enabled for debugging

### Architecture
- Modular design for easy maintenance
- Comprehensive error handling and logging
- Auto-reconnection for WiFi and Home Assistant
- Optimized for ESP32-S3 with SPIRAM

## 🚧 Future Enhancements

- [ ] Weather forecast display (7-day outlook)
- [ ] Touch interface for settings configuration
- [ ] Multiple weather locations support
- [ ] Weather alerts and notifications
- [ ] Configurable update intervals via UI
- [ ] OTA firmware updates
- [ ] Weather history graphs
- [ ] Custom themes and layouts
- [ ] Touch input support for LVGL
- [ ] Animation library integration
- [ ] Custom LVGL widgets
- [ ] Settings persistence
- [ ] Multi-language support

## 🔧 Advanced Development

### Adding Custom UI Components
1. Declare function in `src/ui/ui_main.h`
2. Implement function in `src/ui/ui_main.cpp`
3. Call from main application or weather display

### Creating New Screens
1. Create screen object in `ui_main.cpp`
2. Add creation function following existing patterns
3. Use `ui_switch_to_screen()` for navigation
4. Add event handlers for user interaction

### Modifying Display Settings
1. Update pin definitions in `src/lvgl/lvgl_setup.h`
2. Adjust buffer size based on available memory
3. Modify resolution constants if using different display
4. Test with `lvgl_test_colors()` function

### Weather Data Customization
- Modify `weather_api.cpp` to fetch additional entities
- Update `ui_weather.cpp` to display new data fields
- Add custom weather icons or animations
- Implement forecast data display

## 📝 Configuration

Key configuration options in `src/config.h`:
```cpp
// Weather update intervals
#define WEATHER_UPDATE_INTERVAL_MS (60000)  // 60 seconds

// Display settings
#define BACKLIGHT_PWM_FREQ 5000
#define BACKLIGHT_PWM_RESOLUTION 8

// Debug settings
#define DEBUG_WEATHER_API true
#define DEBUG_LVGL_UI true
```

## 🤝 Contributing

1. Fork the repository
2. Create a feature branch: `git checkout -b feature/amazing-feature`
3. Commit changes: `git commit -m 'Add amazing feature'`
4. Push to branch: `git push origin feature/amazing-feature`
5. Open a Pull Request

## 📄 License

This project is open source. Feel free to use and modify for your own projects.

## 🆘 Troubleshooting

### WiFi Connection Issues
- Verify SSID and password in `secrets.h`
- Check if WiFi network is 2.4GHz (ESP32 limitation)
- Monitor serial output for connection status
- Ensure `secrets.h` exists (copy from `secrets_example.h`)
- Check that Home Assistant server is reachable on the network

### Home Assistant API Issues
- Verify Bearer token is correct and not expired
- Check Home Assistant server IP address
- Ensure `weather.forecast_home` entity exists
- Test API endpoint manually: `http://[HA_IP]:8123/api/states/weather.forecast_home`
- Check Home Assistant logs for authentication errors

### Display Issues
- **No display output**: Check pin connections match definitions in `lvgl_setup.h`
- **Garbled display**: Verify SPI initialization and frequency settings
- **Blank screen**: Test with color sequence, monitor serial output for error messages
- **Flickering**: Check power supply and reduce SPI frequency if unstable

### Performance Issues
- **Slow refresh**: Reduce buffer size if memory constrained
- **Crashes**: Monitor heap usage with `esp_get_free_heap_size()`
- **Memory errors**: Reduce buffer size or widget count, enable SPIRAM if available
- **UI lag**: Simplify UI components or lower refresh rate

### Build Issues
- Update PlatformIO to latest version
- Clean build: `pio run --target clean`
- Check all dependencies are installed
- Verify ESP32 Arduino framework version compatibility
- Clear PlatformIO cache if dependency issues persist

### Upload Issues
- Check COM port selection and driver installation
- Ensure no other serial monitors are connected
- Try different USB cable or port
- Hold BOOT button during upload if needed
- Verify board is in download mode

### Common Error Messages
- **"File access denied"**: Close any serial monitors or other programs using the COM port
- **"Package not found"**: Run `pio platform update` to update packages
- **"LVGL assertion failed"**: Check display initialization order and buffer allocation
- **"WiFi connection timeout"**: Verify network credentials and signal strength

---

**Created**: August 21, 2025
**Version**: 1.0.0
**Framework**: Arduino ESP32
**Graphics**: LVGL 9.3.0
**Status**: ✅ Production Ready
