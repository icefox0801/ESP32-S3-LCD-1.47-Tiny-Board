#ifndef WEATHER_ICONS_H
#define WEATHER_ICONS_H

#include <lvgl.h>
#include <WString.h>

// Weather icon types
enum WeatherIcon
{
  ICON_SUNNY,
  ICON_PARTLY_CLOUDY,
  ICON_CLOUDY,
  ICON_RAINY,
  ICON_POURING,
  ICON_SNOWY,
  ICON_FOG,
  ICON_WINDY,
  ICON_LIGHTNING,
  ICON_CLEAR_NIGHT,
  ICON_UNKNOWN
};

// Weather icons utility class
class WeatherIcons
{
public:
  // Get text representation of weather icon
  static const char *getIconText(WeatherIcon icon);

  // Map weather state string to icon type
  static WeatherIcon getIconFromState(const String &state);

  // Get display name for weather state
  static const char *getStateDisplayName(const String &state);
};

#endif // WEATHER_ICONS_H
