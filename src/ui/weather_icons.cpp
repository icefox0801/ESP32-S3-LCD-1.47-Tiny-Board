#include "weather_icons.h"
#include <Arduino.h>

// WeatherAPI.com condition code to emoji mapping
// Simple and direct weather icon system
static const struct
{
  int conditionCode;
  const char *description;
  const char *emoji;
} weatherConditionMap[] = {
    // WeatherAPI.com condition codes
    {1000, "Sunny", "☀️"},                     // Sunny/Clear
    {1003, "Partly cloudy", "⛅"},            // Partly cloudy
    {1006, "Cloudy", "☁️"},                    // Cloudy
    {1009, "Overcast", "☁️"},                  // Overcast
    {1030, "Mist", "🌫️"},                     // Mist
    {1063, "Patchy rain possible", "🌦️"},     // Patchy rain possible
    {1066, "Patchy snow possible", "🌨️"},     // Patchy snow possible
    {1069, "Patchy sleet possible", "🌨️"},    // Patchy sleet possible
    {1072, "Patchy freezing drizzle", "🌦️"},  // Patchy freezing drizzle possible
    {1087, "Thundery outbreaks", "⛈️"},        // Thundery outbreaks possible
    {1114, "Blowing snow", "🌨️"},             // Blowing snow
    {1117, "Blizzard", "❄️"},                  // Blizzard
    {1135, "Fog", "🌫️"},                      // Fog
    {1147, "Freezing fog", "🌫️"},             // Freezing fog
    {1150, "Patchy light drizzle", "🌦️"},     // Patchy light drizzle
    {1153, "Light drizzle", "🌦️"},            // Light drizzle
    {1168, "Freezing drizzle", "🌧️"},         // Freezing drizzle
    {1171, "Heavy freezing drizzle", "🌧️"},   // Heavy freezing drizzle
    {1180, "Patchy light rain", "🌦️"},        // Patchy light rain
    {1183, "Light rain", "🌧️"},               // Light rain
    {1186, "Moderate rain at times", "🌧️"},   // Moderate rain at times
    {1189, "Moderate rain", "🌧️"},            // Moderate rain
    {1192, "Heavy rain at times", "🌧️"},      // Heavy rain at times
    {1195, "Heavy rain", "🌧️"},               // Heavy rain
    {1198, "Light freezing rain", "🌧️"},      // Light freezing rain
    {1201, "Heavy freezing rain", "🌧️"},      // Moderate or heavy freezing rain
    {1204, "Light sleet", "🌨️"},              // Light sleet
    {1207, "Heavy sleet", "🌨️"},              // Moderate or heavy sleet
    {1210, "Patchy light snow", "🌨️"},        // Patchy light snow
    {1213, "Light snow", "🌨️"},               // Light snow
    {1216, "Patchy moderate snow", "🌨️"},     // Patchy moderate snow
    {1219, "Moderate snow", "🌨️"},            // Moderate snow
    {1222, "Patchy heavy snow", "❄️"},         // Patchy heavy snow
    {1225, "Heavy snow", "❄️"},                // Heavy snow
    {1237, "Ice pellets", "🧊"},              // Ice pellets
    {1240, "Light rain shower", "🌦️"},        // Light rain shower
    {1243, "Heavy rain shower", "🌧️"},        // Moderate or heavy rain shower
    {1246, "Torrential rain shower", "🌧️"},   // Torrential rain shower
    {1249, "Light sleet showers", "🌨️"},      // Light sleet showers
    {1252, "Heavy sleet showers", "🌨️"},      // Moderate or heavy sleet showers
    {1255, "Light snow showers", "🌨️"},       // Light snow showers
    {1258, "Heavy snow showers", "❄️"},        // Moderate or heavy snow showers
    {1261, "Light ice pellet showers", "🧊"}, // Light showers of ice pellets
    {1264, "Heavy ice pellet showers", "🧊"}, // Moderate or heavy ice pellet showers
    {1273, "Light rain with thunder", "⛈️"},   // Patchy light rain with thunder
    {1276, "Heavy rain with thunder", "⛈️"},   // Moderate or heavy rain with thunder
    {1279, "Light snow with thunder", "⛈️"},   // Patchy light snow with thunder
    {1282, "Heavy snow with thunder", "⛈️"}    // Moderate or heavy snow with thunder
};

static const int NUM_CONDITIONS = sizeof(weatherConditionMap) / sizeof(weatherConditionMap[0]);

// Get weather emoji by condition code
const char *WeatherIcons::getWeatherEmoji(int conditionCode)
{
  // Search for the condition code in our mapping
  for (int i = 0; i < NUM_CONDITIONS; i++)
  {
    if (weatherConditionMap[i].conditionCode == conditionCode)
    {
      return weatherConditionMap[i].emoji;
    }
  }

  // Default to sunny emoji if condition not found
  return "☀️";
}

// Get condition description by code
const char *WeatherIcons::getConditionDisplayName(int conditionCode)
{
  // Search for the condition code in our mapping
  for (int i = 0; i < NUM_CONDITIONS; i++)
  {
    if (weatherConditionMap[i].conditionCode == conditionCode)
    {
      return weatherConditionMap[i].description;
    }
  }

  return "Unknown";
}

// Update weather icon widget with emoji symbol
void WeatherIcons::updateWeatherIcon(lv_obj_t *iconWidget, int conditionCode, bool isDaytime)
{
  if (iconWidget == nullptr)
  {
    Serial.println("Warning: iconWidget is null in updateWeatherIcon");
    return;
  }

  // Get the emoji for this condition
  const char *emoji = getWeatherEmoji(conditionCode);

  Serial.printf("Setting weather icon to: %s (condition: %d)\n", emoji, conditionCode);

  // Set the emoji as text on the label widget
  lv_label_set_text(iconWidget, emoji);

  // Center align the emoji
  lv_obj_set_style_text_align(iconWidget, LV_TEXT_ALIGN_CENTER, 0);
}

// Legacy compatibility methods (simplified)
String WeatherIcons::getIconPath(int conditionCode, bool isDaytime)
{
  // Return emoji instead of file path for compatibility
  return String(getWeatherEmoji(conditionCode));
}

const char *WeatherIcons::getDisplayName(const String &state)
{
  // Legacy method - map string states to display names
  if (state == "clear-day" || state == "clear-night" || state == "sunny")
    return "Clear";
  else if (state == "partly-cloudy-day" || state == "partly-cloudy-night")
    return "Partly Cloudy";
  else if (state == "cloudy")
    return "Cloudy";
  else if (state == "overcast")
    return "Overcast";
  else if (state == "rain" || state == "light-rain" || state == "heavy-rain")
    return "Rain";
  else if (state == "snow" || state == "light-snow" || state == "heavy-snow")
    return "Snow";
  else if (state == "sleet")
    return "Sleet";
  else if (state == "wind" || state == "windy")
    return "Windy";
  else if (state == "fog" || state == "foggy")
    return "Fog";
  else if (state == "thunderstorm")
    return "Thunderstorm";
  else if (state == "mist")
    return "Mist";
  else
    return "Unknown";
}

const lv_image_dsc_t *WeatherIcons::getIconImage(const String &state, bool isDaytime)
{
  // Return null since we're using emoji text instead of images
  return nullptr;
}
