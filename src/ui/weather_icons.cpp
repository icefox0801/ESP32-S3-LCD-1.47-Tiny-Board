#include "weather_icons.h"
#include <Arduino.h>
#include <LittleFS.h>
#include <lvgl.h>

// WeatherAPI.com condition code to SVG file mapping
// Maps weather conditions to day/night SVG icons
const WeatherIcons::WeatherCondition WeatherIcons::weatherConditionMap[] = {
    // WeatherAPI.com condition codes mapped to grid positions
    {1000, "Sunny", "day_1_1.svg", "night_1_1.svg"},                   // Sunny/Clear
    {1003, "Partly cloudy", "day_1_2.svg", "night_1_2.svg"},           // Partly cloudy
    {1006, "Cloudy", "day_1_3.svg", "night_1_3.svg"},                  // Cloudy
    {1009, "Overcast", "day_1_3.svg", "night_1_3.svg"},                // Overcast
    {1030, "Mist", "day_1_4.svg", "night_1_4.svg"},                    // Mist
    {1063, "Patchy rain possible", "day_1_5.svg", "night_1_5.svg"},    // Patchy rain possible
    {1066, "Patchy snow possible", "day_1_5.svg", "night_1_5.svg"},    // Patchy snow possible
    {1069, "Patchy sleet possible", "day_1_6.svg", "night_1_6.svg"},   // Patchy sleet possible
    {1072, "Patchy freezing drizzle", "day_1_7.svg", "night_1_7.svg"}, // Patchy freezing drizzle
    {1087, "Thundery outbreaks", "day_1_8.svg", "night_1_8.svg"},      // Thundery outbreaks
    {1114, "Blowing snow", "day_2_1.svg", "night_2_1.svg"},            // Blowing snow
    {1117, "Blizzard", "day_2_2.svg", "night_2_2.svg"},                // Blizzard
    {1135, "Fog", "day_2_3.svg", "night_2_3.svg"},                     // Fog
    {1147, "Freezing fog", "day_2_4.svg", "night_2_4.svg"},            // Freezing fog
    {1150, "Patchy light drizzle", "day_2_5.svg", "night_2_5.svg"},    // Patchy light drizzle
    {1153, "Light drizzle", "day_2_5.svg", "night_2_5.svg"},           // Light drizzle
    {1168, "Freezing drizzle", "day_2_6.svg", "night_2_6.svg"},        // Freezing drizzle
    {1171, "Heavy freezing drizzle", "day_2_7.svg", "night_2_7.svg"},  // Heavy freezing drizzle
    {1180, "Patchy light rain", "day_2_8.svg", "night_2_8.svg"},       // Patchy light rain
    {1183, "Light rain", "day_3_1.svg", "night_3_1.svg"},              // Light rain
    {1186, "Moderate rain at times", "day_3_2.svg", "night_3_2.svg"},  // Moderate rain at times
    {1189, "Moderate rain", "day_3_2.svg", "night_3_2.svg"},           // Moderate rain
    {1192, "Heavy rain at times", "day_3_3.svg", "night_3_3.svg"},     // Heavy rain at times
    {1195, "Heavy rain", "day_3_3.svg", "night_3_3.svg"},              // Heavy rain
    {1198, "Light freezing rain", "day_3_4.svg", "night_3_4.svg"},     // Light freezing rain
    {1201, "Heavy freezing rain", "day_3_4.svg", "night_3_4.svg"},     // Heavy freezing rain
    {1204, "Light sleet", "day_3_5.svg", "night_3_5.svg"},             // Light sleet
    {1207, "Heavy sleet", "day_3_5.svg", "night_3_5.svg"},             // Heavy sleet
    {1210, "Patchy light snow", "day_2_8.svg", "night_2_8.svg"},       // Patchy light snow
    {1213, "Light snow", "day_3_6.svg", "night_3_6.svg"},              // Light snow
    {1216, "Patchy moderate snow", "day_3_6.svg", "night_3_6.svg"},    // Patchy moderate snow
    {1219, "Moderate snow", "day_3_7.svg", "night_3_7.svg"},           // Moderate snow
    {1222, "Patchy heavy snow", "day_3_8.svg", "night_3_8.svg"},       // Patchy heavy snow
    {1225, "Heavy snow", "day_3_8.svg", "night_3_8.svg"},              // Heavy snow
    {1237, "Ice pellets", "day_4_1.svg", "night_4_1.svg"},             // Ice pellets
    {1240, "Light rain shower", "day_4_2.svg", "night_4_2.svg"},       // Light rain shower
    {1243, "Heavy rain shower", "day_4_3.svg", "night_4_3.svg"},       // Heavy rain shower
    {1246, "Torrential rain shower", "day_4_3.svg", "night_4_3.svg"},  // Torrential rain shower
    {1249, "Light sleet shower", "day_4_4.svg", "night_4_4.svg"},      // Light sleet showers
    {1252, "Heavy sleet shower", "day_4_4.svg", "night_4_4.svg"},      // Heavy sleet showers
    {1255, "Light snow shower", "day_4_5.svg", "night_4_5.svg"},       // Light snow showers
    {1258, "Heavy snow shower", "day_4_6.svg", "night_4_6.svg"},       // Heavy snow showers
    {1261, "Light ice pellet shower", "day_4_7.svg", "night_4_7.svg"}, // Light ice pellet showers
    {1264, "Heavy ice pellet shower", "day_4_7.svg", "night_4_7.svg"}, // Heavy ice pellet showers
    {1273, "Light rain with thunder", "day_4_8.svg", "night_4_8.svg"}, // Light rain with thunder
    {1276, "Heavy rain with thunder", "day_4_8.svg", "night_4_8.svg"}, // Heavy rain with thunder
    {1279, "Light snow with thunder", "day_4_8.svg", "night_4_8.svg"}, // Light snow with thunder
    {1282, "Heavy snow with thunder", "day_4_8.svg", "night_4_8.svg"}, // Heavy snow with thunder
};

const int WeatherIcons::NUM_CONDITIONS = sizeof(weatherConditionMap) / sizeof(weatherConditionMap[0]);

// Get emoji representation for condition code
const char *WeatherIcons::getEmojiForCondition(int conditionCode)
{
  // Map weather conditions to appropriate emojis
  if (conditionCode == 1000)
    return "☀️"; // Sunny
  else if (conditionCode == 1003)
    return "⛅"; // Partly cloudy
  else if (conditionCode == 1006 || conditionCode == 1009)
    return "☁️"; // Cloudy/Overcast
  else if (conditionCode == 1030 || conditionCode == 1135 || conditionCode == 1147)
    return "🌫️"; // Mist/Fog
  else if (conditionCode >= 1063 && conditionCode <= 1072)
    return "🌦️"; // Light rain/drizzle
  else if (conditionCode == 1087 || conditionCode >= 1273)
    return "⛈️"; // Thunder
  else if ((conditionCode >= 1114 && conditionCode <= 1117) ||
           (conditionCode >= 1210 && conditionCode <= 1225) ||
           (conditionCode >= 1255 && conditionCode <= 1282))
    return "🌨️"; // Snow
  else if (conditionCode >= 1180 && conditionCode <= 1201)
    return "🌧️"; // Rain
  else if (conditionCode >= 1204 && conditionCode <= 1207)
    return "🌨️"; // Sleet
  else if (conditionCode == 1237 || conditionCode >= 1261)
    return "🧊"; // Ice pellets
  else if (conditionCode >= 1240 && conditionCode <= 1246)
    return "🌦️"; // Showers

  return "☀️"; // Default sunny
}

// Get PNG file path by condition code (changed from SVG)
String WeatherIcons::getSVGPath(int conditionCode, bool isDaytime)
{
  // Search for the condition code in our mapping
  for (int i = 0; i < NUM_CONDITIONS; i++)
  {
    if (weatherConditionMap[i].conditionCode == conditionCode)
    {
      const char *filename = isDaytime ? weatherConditionMap[i].dayIcon : weatherConditionMap[i].nightIcon;
      // Change extension from .svg to .png
      String path = String("/icons/") + filename;
      path.replace(".svg", ".png");
      return path;
    }
  }

  // Default to sunny icon if condition not found
  return isDaytime ? "/icons/day_1_1.png" : "/icons/night_1_1.png";
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

// Update weather icon widget with PNG (using LODEPNG decoder)
void WeatherIcons::updateWeatherIcon(lv_obj_t *iconWidget, int conditionCode, bool isDaytime)
{
  if (iconWidget == nullptr)
  {
    return;
  }

  // Get the PNG file path for this condition
  String pngPath = getSVGPath(conditionCode, isDaytime);

  // Look for existing image child, or create new one
  lv_obj_t *img = nullptr;
  uint32_t child_count = lv_obj_get_child_count(iconWidget);

  // Try to find existing image object
  for (uint32_t i = 0; i < child_count; i++)
  {
    lv_obj_t *child = lv_obj_get_child(iconWidget, i);
    if (lv_obj_check_type(child, &lv_image_class))
    {
      img = child;
      break;
    }
  }

  // Create new image if none exists
  if (img == nullptr)
  {
    img = lv_image_create(iconWidget);
    if (img == nullptr)
    {
      // Fallback to text label
      lv_obj_clean(iconWidget);
      lv_obj_t *label = lv_label_create(iconWidget);
      lv_label_set_text(label, getConditionDisplayName(conditionCode));
      lv_obj_center(label);
      return;
    }

    // Initial setup for new image
    lv_obj_set_size(img, 64, 64);
    lv_obj_center(img);
    lv_obj_clear_flag(img, LV_OBJ_FLAG_HIDDEN);
  }

  // Use static buffer to ensure path persists in memory
  // LVGL needs the string to remain valid after the function returns
  static char path_buffers[5][64]; // Support up to 5 concurrent images
  static int buffer_index = 0;

  // Cycle through buffers
  buffer_index = (buffer_index + 1) % 5;

  // Build path with filesystem prefix
  snprintf(path_buffers[buffer_index], sizeof(path_buffers[buffer_index]),
           "S:%s", pngPath.c_str());

  // Set the image source (LVGL will decode PNG automatically via LODEPNG)
  lv_image_set_src(img, path_buffers[buffer_index]);
} // Create SVG weather icon widget
lv_obj_t *WeatherIcons::createSVGIcon(lv_obj_t *parent, int conditionCode, bool isDaytime)
{
  if (parent == nullptr)
  {
    Serial.println("Warning: parent is null in createSVGIcon");
    return nullptr;
  }

  // Create container for the icon
  lv_obj_t *container = lv_obj_create(parent);
  lv_obj_set_size(container, 64, 64);
  lv_obj_set_style_bg_opa(container, LV_OPA_TRANSP, 0);
  lv_obj_set_style_border_opa(container, LV_OPA_TRANSP, 0);

  // Update with the weather condition
  updateWeatherIcon(container, conditionCode, isDaytime);

  return container;
}

// Legacy compatibility methods
String WeatherIcons::getIconPath(int conditionCode, bool isDaytime)
{
  return getSVGPath(conditionCode, isDaytime);
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
  // Return null since we're using SVG instead of bitmap images
  return nullptr;
}
