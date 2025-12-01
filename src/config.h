#ifndef CONFIG_H
#define CONFIG_H

// Project headers
#include "weather/secrets.h"

// Weather Update Settings
#define WEATHER_UPDATE_INTERVAL_MS 60000    // Update every 60 seconds
#define WEATHER_UI_UPDATE_INTERVAL_MS 30000 // UI refresh every 30 seconds

// Display Settings
#define BACKLIGHT_BRIGHTNESS 70 // 0-100%

// Display Orientation
// Set to 0 for VERTICAL (portrait, 172x320) - default
// Set to 1 for HORIZONTAL (landscape, 320x172)
#define DISPLAY_HORIZONTAL 1

// Debug Settings
// Set to 1 to enable verbose debug logging, 0 to disable
#define DEBUG_ENABLED 0

#endif // CONFIG_H
