#ifndef WEATHER_ICONS_H
#define WEATHER_ICONS_H

#include <lvgl.h>
#include <WString.h>

// Weather icons utility class using SVG files
// Clean WeatherAPI.com integration with LVGL SVG rendering
class WeatherIcons
{
public:
  // Get SVG file path for condition code
  static String getSVGPath(int conditionCode, bool isDaytime = true);

  // Get display name for weather condition code
  static const char *getConditionDisplayName(int conditionCode);

  // Update existing weather icon widget with SVG
  static void updateWeatherIcon(lv_obj_t *iconWidget, int conditionCode, bool isDaytime = true);

  // Create SVG weather icon widget
  static lv_obj_t *createSVGIcon(lv_obj_t *parent, int conditionCode, bool isDaytime = true);

  // Legacy compatibility methods
  static String getIconPath(int conditionCode, bool isDaytime = true);
  static const char *getDisplayName(const String &state);
  static const lv_image_dsc_t *getIconImage(const String &state, bool isDaytime = true);

private:
  // Get emoji for condition code (internal helper)
  static const char *getEmojiForCondition(int conditionCode);
  // Internal mapping structure
  struct WeatherCondition
  {
    int conditionCode;
    const char *description;
    const char *dayIcon;
    const char *nightIcon;
  };

  static const WeatherCondition weatherConditionMap[];
  static const int NUM_CONDITIONS;
};

#endif // WEATHER_ICONS_H
