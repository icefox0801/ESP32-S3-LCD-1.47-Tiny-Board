# ESP32-S3 Weather Display

A modern weather display system using ESP32-S3-LCD-1.47-Tiny-Board with LVGL graphics and WeatherAPI.com integration.

![ESP32-S3](https://img.shields.io/badge/ESP32-S3-blue)
![LVGL](https://img.shields.io/badge/LVGL-9.3.0-green)
![WeatherAPI](https://img.shields.io/badge/WeatherAPI.com-Integrated-blue)
![PlatformIO](https://img.shields.io/badge/PlatformIO-Compatible-orange)
![Status](https://img.shields.io/badge/Build-Successful-brightgreen)

## ✨ Features

- 🎨 **Modern LVGL UI**: Dark theme with emoji weather icons
- 📶 **WiFi Connectivity**: Secure credential management system
- � **WeatherAPI.com Integration**: Reliable weather data with optimized API calls
- 🌤️ **Complete Weather Display**: Current temp, min/max, humidity, pressure, wind
- 🔧 **Modular Architecture**: Clean, maintainable code structure
- 💡 **PWM Backlight Control**: Adjustable display brightness
- 🔄 **Auto-refresh**: Automatic weather updates every 10 minutes

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

### 1. Clone and Configure
```bash
git clone <your-repo>
cd ESP32-S3-LCD-1.47-Tiny-Board/src/weather/
cp secrets_example.h secrets.h
```

### 2. Edit Configuration
Edit `src/weather/secrets.h` with your information:
```cpp
#define WEATHER_API_KEY "your_weatherapi_key_here"
#define WEATHER_LOCATION "Beijing"  // City name or coordinates
#define WEATHER_UNITS "metric"      // metric, imperial
```

### 3. Get WeatherAPI.com Key
1. Sign up at [WeatherAPI.com](https://www.weatherapi.com/)
2. Get your free API key (1M calls/month)
3. Copy key to `secrets.h`
4. Set your location (city name, coordinates, or postcode)

**Security Note**:
- `secrets.h` is automatically ignored by git for security
- Never commit real API keys to version control
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
    ├── weather_api.h/.cpp  # WeatherAPI.com client
    ├── secrets.h           # API credentials (gitignored)
    └── secrets_example.h   # Configuration template
```

## ⚙️ Detailed Configuration

### Configuration
The system configuration is managed through secure credential files:

1. **Copy the configuration template:**
   ```bash
   cd src/weather/
   cp secrets_example.h secrets.h
   ```

2. **Edit secrets.h with your credentials:**
   - Add your WeatherAPI.com API key
   - Set your location (city name, coordinates, or postcode)
   - Choose units (metric for °C, imperial for °F)

3. **Configuration files:**
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

The system displays weather information from **WeatherAPI.com** with optimized API calls:

| Data | Description | Source |
|------|-------------|---------|
| **Temperature** | Current temperature in °C | Current weather |
| **Min/Max** | Today's temperature range | Forecast data |
| **Condition** | Weather condition with emoji icons | Current weather |
| **Humidity** | Relative humidity percentage | Current weather |
| **Pressure** | Atmospheric pressure in hPa | Current weather |
| **Wind** | Wind speed (m/s) and direction | Current weather |

### Weather API Configuration
Edit `src/weather/secrets.h` with your WeatherAPI.com credentials:
```cpp
#define WEATHER_API_KEY "your_weatherapi_key_here"
#define WEATHER_LOCATION "Beijing"  // City name or coordinates
#define WEATHER_UNITS "metric"      // metric, imperial
```

## 🔒 Security Features

- ✅ API credentials stored in gitignored file
- ✅ Example template provided for easy setup
- ✅ No hardcoded secrets in version control
- ✅ Secure HTTPS API communication with WeatherAPI.com
- ✅ Automatic API key validation

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
- Auto-reconnection for WiFi and WeatherAPI.com
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
- Verify WiFi network is 2.4GHz (ESP32 limitation)
- Monitor serial output for connection status
- Check network connectivity and internet access

### WeatherAPI.com Issues
- Verify API key is correct and active in `src/weather/secrets.h`
- Check location string format (city name, coordinates, or postcode)
- Ensure internet connectivity for API access
- Test API endpoint manually: `http://api.weatherapi.com/v1/current.json?key=YOUR_KEY&q=Beijing`
- Monitor serial output for HTTP response codes and error messages

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
