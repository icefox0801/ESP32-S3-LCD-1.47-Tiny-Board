#include "weather_icons.h"
#include <Arduino.h>

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
