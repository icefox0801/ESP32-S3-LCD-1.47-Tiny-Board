#ifndef WEATHER_ICONS_H
#define WEATHER_ICONS_H

#include <lvgl.h>
#include <WString.h>

// Weather icons utility class for PNG weather icons
// WeatherAPI.com integration with LVGL PNG rendering via LODEPNG
class WeatherIcons
{
public:
  // Get PNG file path for a weather condition code
  static String getPNGPath(int conditionCode, bool isDaytime = true);

  // Get display name for weather condition code
  static const char *getConditionDisplayName(int conditionCode);

  // Update existing weather icon widget with PNG image
  static void updateWeatherIcon(lv_obj_t *iconWidget, int conditionCode, bool isDaytime = true);

private:
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
