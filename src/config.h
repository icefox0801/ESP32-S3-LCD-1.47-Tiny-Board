#ifndef CONFIG_H
#define CONFIG_H

// Project headers
#include "weather/secrets.h"

// Weather Update Settings
#define WEATHER_UPDATE_INTERVAL_MS 60000    // Update every 60 seconds
#define WEATHER_UI_UPDATE_INTERVAL_MS 30000 // UI refresh every 30 seconds

// Display Settings
#define BACKLIGHT_BRIGHTNESS 70 // 0-100%

// Debug Settings
// Set to 1 to enable verbose debug logging, 0 to disable
#define DEBUG_ENABLED 0

#endif // CONFIG_H
