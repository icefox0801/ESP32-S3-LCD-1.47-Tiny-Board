#ifndef CONFIG_H
#define CONFIG_H

// Include sensitive configuration
#include "secrets.h"

// Weather Update Settings
#define WEATHER_UPDATE_INTERVAL_MS 60000    // Update every 60 seconds
#define WEATHER_UI_UPDATE_INTERVAL_MS 30000 // UI refresh every 30 seconds

// Display Settings
#define BACKLIGHT_BRIGHTNESS 70 // 0-100%

// Debug Settings
#define DEBUG_WEATHER true
#define DEBUG_WIFI true
#define DEBUG_LVGL true

#endif // CONFIG_H
