#ifndef WEATHER_ICONS_H
#define WEATHER_ICONS_H

#include <lvgl.h>
#include <WString.h>

// Weather icons utility class using Unicode emojis
// Clean WeatherAPI.com integration with emoji weather symbols
class WeatherIcons
{
public:
  // Get weather emoji for condition code
  static const char *getWeatherEmoji(int conditionCode);

  // Get display name for weather condition code
  static const char *getConditionDisplayName(int conditionCode);

  // Update existing weather icon widget with new condition
  static void updateWeatherIcon(lv_obj_t *iconWidget, int conditionCode, bool isDaytime = true);

  // Legacy compatibility methods (simplified)
  static String getIconPath(int conditionCode, bool isDaytime = true);
  static const char *getDisplayName(const String &state);
  static const lv_image_dsc_t *getIconImage(const String &state, bool isDaytime = true);
};

#endif // WEATHER_ICONS_H
