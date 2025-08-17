# ESP32-S3-LCD-1.47-Tiny-Board Project

## Overview

This project is designed for the ESP32-S3-LCD-1.47-Tiny-Board, a compact development board featuring an ESP32-S3 microcontroller with an integrated 1.47-inch LCD display.

## Project Structure

```
├── .vscode/                 # VS Code configuration
│   ├── c_cpp_properties.json
│   ├── extensions.json
│   ├── launch.json
│   └── settings.json
├── include/                 # Header files
├── lib/                     # Project libraries
├── src/                     # Source code
│   └── main.cpp            # Main application file
├── test/                    # Unit tests
├── .editorconfig           # Editor configuration
├── .gitignore              # Git ignore rules
├── platformio.ini          # PlatformIO configuration
└── CLAUDE.md              # This documentation file
```

## Hardware Specifications

- **Microcontroller**: ESP32-S3
- **Display**: 1.47-inch LCD
- **Framework**: Arduino
- **Platform**: Espressif32

## Development Environment

### PlatformIO Configuration

The project uses PlatformIO with the following configuration:
- **Platform**: espressif32
- **Board**: esp32s3box
- **Framework**: arduino

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

# Upload to device
pio run --target upload

# Monitor serial output
pio device monitor
```

### Debugging

The project includes debug configurations for:
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
- Use the `include/` directory for custom headers
- Keep `main.cpp` focused on setup and loop functions

## Hardware Features

### ESP32-S3 Capabilities

- **Dual-core Xtensa LX7 CPU** up to 240 MHz
- **Wi-Fi 802.11 b/g/n**
- **Bluetooth 5 (LE)**
- **Rich peripheral interfaces**
- **Low power consumption modes**

### LCD Display

- **Size**: 1.47 inches
- **Interface**: SPI
- **Resolution**: [To be specified based on actual hardware]

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

Example basic display setup:

```cpp
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

// Initialize display (pins to be configured)
Adafruit_ST7789 tft = Adafruit_ST7789(CS_PIN, DC_PIN, RST_PIN);

void setup() {
  tft.init(240, 240);  // Initialize with display resolution
  tft.setRotation(2);
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_WHITE);
  tft.println("Hello, ESP32-S3!");
}
```

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

---

*Generated with assistance from Claude AI*
*Last updated: August 21, 2025*
