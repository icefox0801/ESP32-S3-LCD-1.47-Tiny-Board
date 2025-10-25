#include "weather_icons.h"
#include <Arduino.h>

// Include all weather icon headers
#include "icons/sunny_icon_64.h"
#include "icons/clear_night_icon_64.h"
#include "icons/partly_cloudy_icon_64.h"
#include "icons/cloudy_icon_64.h"
#include "icons/rainy_icon_64.h"
#include "icons/pouring_icon_64.h"
#include "icons/snowy_icon_64.h"
#include "icons/snowy_rainy_icon_64.h"
#include "icons/fog_icon_64.h"
#include "icons/hail_icon_64.h"
#include "icons/windy_icon_64.h"
#include "icons/lightning_icon_64.h"
#include "icons/lightning_rainy_icon_64.h"
#include "icons/exceptional_icon_64.h"
#include "icons/overcast_icon_64.h"

const char *WeatherIcons::getIconText(WeatherIcon icon)
{
  switch (icon)
  {
  case ICON_SUNNY:
    return "☀️";
  case ICON_PARTLY_CLOUDY:
    return "⛅";
  case ICON_CLOUDY:
    return "☁️";
  case ICON_RAINY:
    return "🌧️";
  case ICON_POURING:
    return "⛈️";
  case ICON_SNOWY:
    return "❄️";
  case ICON_FOG:
    return "🌫️";
  case ICON_WINDY:
    return "💨";
  case ICON_LIGHTNING:
    return "⚡";
  case ICON_CLEAR_NIGHT:
    return "🌙";
  default:
    return "❓";
  }
}

WeatherIcon WeatherIcons::getIconFromState(const String &state)
{
  if (state == "sunny" || state == "clear")
  {
    return ICON_SUNNY;
  }
  else if (state == "partlycloudy")
  {
    return ICON_PARTLY_CLOUDY;
  }
  else if (state == "cloudy")
  {
    return ICON_CLOUDY;
  }
  else if (state == "rainy")
  {
    return ICON_RAINY;
  }
  else if (state == "pouring")
  {
    return ICON_POURING;
  }
  else if (state == "snowy" || state == "snowy-rainy")
  {
    return ICON_SNOWY;
  }
  else if (state == "fog" || state == "hail")
  {
    return ICON_FOG;
  }
  else if (state == "windy")
  {
    return ICON_WINDY;
  }
  else if (state == "lightning" || state == "lightning-rainy")
  {
    return ICON_LIGHTNING;
  }
  else if (state == "clear-night")
  {
    return ICON_CLEAR_NIGHT;
  }
  else
  {
    return ICON_UNKNOWN;
  }
}

const char *WeatherIcons::getStateDisplayName(const String &state)
{
  if (state == "sunny" || state == "clear")
  {
    return "Sunny";
  }
  else if (state == "partlycloudy")
  {
    return "Partly Cloudy";
  }
  else if (state == "cloudy")
  {
    return "Cloudy";
  }
  else if (state == "rainy")
  {
    return "Rainy";
  }
  else if (state == "pouring")
  {
    return "Pouring";
  }
  else if (state == "snowy")
  {
    return "Snowy";
  }
  else if (state == "snowy-rainy")
  {
    return "Snowy Rainy";
  }
  else if (state == "fog")
  {
    return "Foggy";
  }
  else if (state == "hail")
  {
    return "Hail";
  }
  else if (state == "windy")
  {
    return "Windy";
  }
  else if (state == "lightning")
  {
    return "Lightning";
  }
  else if (state == "lightning-rainy")
  {
    return "Thunderstorm";
  }
  else if (state == "clear-night")
  {
    return "Clear Night";
  }
  else if (state == "exceptional")
  {
    return "Exceptional";
  }
  else
  {
    return "Unknown";
  }
}

const lv_image_dsc_t *WeatherIcons::getIconImage(const String &state)
{
  if (state == "sunny" || state == "clear")
  {
    return &sunny_icon_64;
  }
  else if (state == "partlycloudy")
  {
    return &partly_cloudy_icon_64;
  }
  else if (state == "cloudy")
  {
    return &cloudy_icon_64;
  }
  else if (state == "rainy")
  {
    return &rainy_icon_64;
  }
  else if (state == "pouring")
  {
    return &pouring_icon_64;
  }
  else if (state == "snowy")
  {
    return &snowy_icon_64;
  }
  else if (state == "snowy-rainy")
  {
    return &snowy_rainy_icon_64;
  }
  else if (state == "fog")
  {
    return &fog_icon_64;
  }
  else if (state == "hail")
  {
    return &hail_icon_64;
  }
  else if (state == "windy")
  {
    return &windy_icon_64;
  }
  else if (state == "lightning")
  {
    return &lightning_icon_64;
  }
  else if (state == "lightning-rainy")
  {
    return &lightning_rainy_icon_64;
  }
  else if (state == "clear-night")
  {
    return &clear_night_icon_64;
  }
  else if (state == "exceptional")
  {
    return &exceptional_icon_64;
  }
  else
  {
    // Default to sunny if unknown
    return &sunny_icon_64;
  }
}
