# ESP32-S3 Weather Display Project

## Overview

This project is a modern weather display system built for the ESP32-S3-LCD-1.47-Tiny-Board, featuring LVGL 9.4.0 graphics, WeatherAPI.com integration, and high-quality PNG weather icons with day/night variants.

## Project Structure

```
├── .vscode/                    # VS Code configuration
├── data/
│   └── icons/                 # 64 weather PNG icons (64x64, day/night)
├── include/
│   ├── lv_conf.h              # LVGL configuration
│   └── README
├── lib/                       # Project libraries
├── resources/
│   └── icons/                 # Source SVG files and converter
├── src/
│   ├── config.h               # Main configuration (intervals, debug flag)
│   ├── debug.h                # Debug macro system
│   ├── main.cpp               # Application entry with millis() timer
│   ├── lvgl/                  # LVGL display setup
│   │   ├── lvgl_setup.h/.cpp
│   │   └── lvgl_fs_spiffs.h/.cpp
│   ├── ui/                    # User interface
│   │   ├── ui_weather.h/.cpp  # Weather display with helper methods
│   │   └── weather_icons.h/.cpp
│   ├── weather/               # Weather API integration
│   │   ├── weather_api.h/.cpp # WeatherAPI.com client
│   │   ├── secrets.h          # API credentials (gitignored)
│   │   └── secrets_example.h
│   └── wifi/                  # WiFi management
│       ├── wifi_setup.h/.cpp
│       ├── wifi_secrets.h     # WiFi credentials (gitignored)
│       └── wifi_secrets_example.h
├── test/                      # Unit tests
├── .editorconfig
├── .gitignore
├── platformio.ini
├── README.md                  # Main documentation
└── CLAUDE.md                  # This file
```

## Hardware Specifications

- **Microcontroller**: ESP32-S3 (Dual-core Xtensa LX7, 240MHz)
- **Display**: 1.47-inch ST7789 TFT LCD (172x320 pixels)
- **Memory**: 8MB Flash, 16MB SPIRAM (Octal SPI)
- **Framework**: Arduino ESP32
- **Platform**: Espressif32
- **Interface**: SPI (40MHz)
- **Backlight**: PWM controlled (Pin 48)

## Development Environment

### PlatformIO Configuration

The project uses PlatformIO with the following configuration:
- **Platform**: espressif32 @ 6.12.0
- **Board**: esp32s3-devkitc-1
- **Framework**: arduino
- **Key Libraries**:
  - LVGL @ 9.4.0
  - ArduinoJson @ 7.4.2
  - Adafruit ST7789 Library @ 1.11.0
  - HTTPClient, WiFi (ESP32 built-in)

### Code Formatting

This project enforces consistent code formatting:
- **Indentation**: 2 spaces (no tabs)
- **Auto-format**: Enabled on save
- **Trailing whitespace**: Automatically trimmed
- **Final newline**: Automatically inserted

## Getting Started

### Prerequisites

1. **PlatformIO IDE** or **PlatformIO Core**
2. **VS Code** (recommended) with PlatformIO extension

### Building the Project

```bash
# Build the project
pio run

# Upload filesystem (weather icon PNGs)
pio run --target uploadfs

# Upload firmware to device
pio run --target upload --upload-port COM4

# Monitor serial output
pio device monitor
```

### Current Build Stats
- **Flash Usage**: 45.1% (1,418,781 / 3,145,728 bytes)
- **RAM Usage**: 56.7% (185,936 / 327,680 bytes)
- **Build Status**: ✅ Successful

### Debugging

The project includes debug configurations and a conditional logging system:

**Debug System (src/debug.h):**
- `DEBUG_ENABLED` flag in `config.h` (0=production, 1=debug)
- `DEBUG_LOG(msg)` - Debug messages (only when DEBUG_ENABLED=1)
- `DEBUG_LOGF(fmt, ...)` - Formatted debug messages
- `LOG_INFO(msg)` - Always-on info messages
- `LOG_ERROR(msg)` - Always-on error messages

**PlatformIO Debug:**
- PIO Debug (with pre-debug tasks)
- PIO Debug (skip pre-debug)
- PIO Debug (without uploading)

## Development Guidelines

### Code Style

- Use 2 spaces for indentation
- Follow Arduino/C++ naming conventions
- Keep functions small and focused
- Add comments for complex logic

### File Organization

- Place hardware-specific code in separate modules
- Use the `include/` directory for custom headers (like lv_conf.h)
- Keep `main.cpp` focused on setup and loop functions
- Use helper methods to break down large functions
- Store secrets in gitignored files with example templates
- Organize by functionality: lvgl/, ui/, weather/, wifi/

## Hardware Features

### ESP32-S3 Capabilities

- **Dual-core Xtensa LX7 CPU** @ 240 MHz
- **Wi-Fi 802.11 b/g/n** (2.4GHz)
- **Bluetooth 5 (LE)**
- **16MB SPIRAM** (Octal SPI)
- **8MB Flash** storage
- **Rich peripheral interfaces** (SPI, I2C, UART, etc.)
- **Low power consumption modes**

### LCD Display

- **Size**: 1.47 inches diagonal
- **Controller**: ST7789
- **Interface**: SPI @ 40MHz
- **Resolution**: 172x320 pixels
- **Backlight**: PWM controlled (5kHz, 8-bit resolution)
- **Pin Configuration**:
  - MOSI: 45, SCLK: 40, CS: 42
  - DC: 41, RST: 39, BL: 48

## Common Tasks

### Adding Libraries

Add dependencies to `platformio.ini`:

```ini
[env:esp32s3box]
platform = espressif32
board = esp32s3box
framework = arduino
lib_deps =
    adafruit/Adafruit GFX Library
    adafruit/Adafruit ST7735 and ST7789 Library
```

### Display Programming

The project uses LVGL 9.4.0 for advanced UI rendering:

```cpp
#include <lvgl.h>
#include "lvgl/lvgl_setup.h"

void setup() {
  // Initialize LVGL display system
  lvgl_setup();
  
  // Create weather UI
  WeatherUI weather_ui(&weather_api);
  weather_ui.createWeatherScreen();
  weather_ui.showWeatherScreen();
}

void loop() {
  // LVGL timer handler (must be called regularly)
  lv_timer_handler();
  delay(5);
}
```

**Key Features:**
- PNG image loading via SPIFFS
- Multiple screens and containers
- Professional UI components (cards, labels, icons)
- Anti-aliasing and smooth rendering
- Event handling and animations

## Troubleshooting

### Common Issues

1. **Upload Errors**: Ensure the board is in download mode
2. **Display Not Working**: Check SPI connections and pin assignments
3. **Memory Issues**: Monitor heap usage and optimize code

### Serial Debugging

```cpp
void setup() {
  Serial.begin(115200);
  Serial.println("ESP32-S3 Starting...");
}
```

## Git Workflow & Standards

### Commit Message Standards
**Format**: `<type>(<scope>): <emoji> <message>`

**Types & Emojis:**
- `feat`: ✨ New features and enhancements
- `fix`: 🐛 Bug fixes and error corrections
- `refactor`: ♻️ Code restructuring without functionality changes
- `docs`: 📝 Documentation updates
- `config`: ⚙️ Configuration and build system changes
- `perf`: 🚀 Performance improvements
- `ui`: 💄 User interface and styling changes
- `hardware`: 🔧 Hardware-related modifications

**Best Practices:**
- Keep messages concise and descriptive
- Use present tense ("add feature" not "added feature")
- Include scope when relevant (ui, wifi, serial, etc.)
- No commit body required - title should be self-explanatory
- Reference issues/PRs when applicable

## Contributing

1. Follow the established code formatting rules
2. Test thoroughly on hardware before committing
3. Document any new features or APIs
4. Update this README for significant changes

## License

[Specify your license here]

## Resources

- [ESP32-S3 Technical Reference](https://www.espressif.com/sites/default/files/documentation/esp32-s3_technical_reference_manual_en.pdf)
- [PlatformIO Documentation](https://docs.platformio.org/)
- [Arduino ESP32 Reference](https://docs.espressif.com/projects/arduino-esp32/en/latest/)

## Recent Development History

### October 2025 - Code Refactoring & Cleanup
- ✅ Added debug macro system with conditional compilation
- ✅ Fixed timestamp display bug (now shows actual fetch time)
- ✅ Extracted helper methods from large functions
- ✅ Removed unused features: wind, pressure, cloud coverage, UV index
- ✅ Removed ~350 lines of unused code
- ✅ Simplified WeatherData struct to essential fields only
- ✅ Enhanced NTP sync with timeout validation
- ✅ Added LVGL refresh handling for timestamp updates

### Key Improvements
- **Maintainability**: Helper methods for temperature, humidity, air quality, timestamp display
- **Debug Control**: Single flag to toggle debug output (production vs development)
- **Performance**: Reduced code size and simplified API calls
- **Reliability**: Fixed timestamp freeze issue, added forced LVGL refresh

---

*Generated with assistance from Claude AI*
*Last updated: October 30, 2025*
