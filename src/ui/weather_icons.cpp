#include "weather_icons.h"
#include <Arduino.h>
#include <LittleFS.h>
#include <lvgl.h>

// WeatherAPI.com condition code to PNG file mapping
// Maps weather conditions to day/night PNG icons (converted from SVG)
// clang-format off
const WeatherIcons::WeatherCondition WeatherIcons::weatherConditionMap[] = {
    // Code  Description                Day Icon         Night Icon
    {1000,  "Sunny",                   "day_1_1.png",   "night_1_1.png"},   // Sunny/Clear
    {1003,  "Partly cloudy",           "day_1_2.png",   "night_1_2.png"},   // Partly cloudy
    {1006,  "Cloudy",                  "day_1_3.png",   "night_1_3.png"},   // Cloudy
    {1009,  "Overcast",                "day_1_3.png",   "night_1_3.png"},   // Overcast
    {1030,  "Mist",                    "day_1_4.png",   "night_1_4.png"},   // Mist
    {1063,  "Patchy rain possible",    "day_1_5.png",   "night_1_5.png"},   // Patchy rain possible
    {1066,  "Patchy snow possible",    "day_1_5.png",   "night_1_5.png"},   // Patchy snow possible
    {1069,  "Patchy sleet possible",   "day_1_6.png",   "night_1_6.png"},   // Patchy sleet possible
    {1072,  "Patchy freezing drizzle", "day_1_7.png",   "night_1_7.png"},   // Patchy freezing drizzle
    {1087,  "Thundery outbreaks",      "day_1_8.png",   "night_1_8.png"},   // Thundery outbreaks
    {1114,  "Blowing snow",            "day_2_1.png",   "night_2_1.png"},   // Blowing snow
    {1117,  "Blizzard",                "day_2_2.png",   "night_2_2.png"},   // Blizzard
    {1135,  "Fog",                     "day_2_3.png",   "night_2_3.png"},   // Fog
    {1147,  "Freezing fog",            "day_2_4.png",   "night_2_4.png"},   // Freezing fog
    {1150,  "Patchy light drizzle",    "day_2_5.png",   "night_2_5.png"},   // Patchy light drizzle
    {1153,  "Light drizzle",           "day_2_5.png",   "night_2_5.png"},   // Light drizzle
    {1168,  "Freezing drizzle",        "day_2_6.png",   "night_2_6.png"},   // Freezing drizzle
    {1171,  "Heavy freezing drizzle",  "day_2_7.png",   "night_2_7.png"},   // Heavy freezing drizzle
    {1180,  "Patchy light rain",       "day_2_8.png",   "night_2_8.png"},   // Patchy light rain
    {1183,  "Light rain",              "day_3_1.png",   "night_3_1.png"},   // Light rain
    {1186,  "Moderate rain at times",  "day_3_2.png",   "night_3_2.png"},   // Moderate rain at times
    {1189,  "Moderate rain",           "day_3_2.png",   "night_3_2.png"},   // Moderate rain
    {1192,  "Heavy rain at times",     "day_3_3.png",   "night_3_3.png"},   // Heavy rain at times
    {1195,  "Heavy rain",              "day_3_3.png",   "night_3_3.png"},   // Heavy rain
    {1198,  "Light freezing rain",     "day_3_4.png",   "night_3_4.png"},   // Light freezing rain
    {1201,  "Heavy freezing rain",     "day_3_4.png",   "night_3_4.png"},   // Heavy freezing rain
    {1204,  "Light sleet",             "day_3_5.png",   "night_3_5.png"},   // Light sleet
    {1207,  "Heavy sleet",             "day_3_5.png",   "night_3_5.png"},   // Heavy sleet
    {1210,  "Patchy light snow",       "day_2_8.png",   "night_2_8.png"},   // Patchy light snow
    {1213,  "Light snow",              "day_3_6.png",   "night_3_6.png"},   // Light snow
    {1216,  "Patchy moderate snow",    "day_3_6.png",   "night_3_6.png"},   // Patchy moderate snow
    {1219,  "Moderate snow",           "day_3_7.png",   "night_3_7.png"},   // Moderate snow
    {1222,  "Patchy heavy snow",       "day_3_8.png",   "night_3_8.png"},   // Patchy heavy snow
    {1225,  "Heavy snow",              "day_3_8.png",   "night_3_8.png"},   // Heavy snow
    {1237,  "Ice pellets",             "day_4_1.png",   "night_4_1.png"},   // Ice pellets
    {1240,  "Light rain shower",       "day_4_2.png",   "night_4_2.png"},   // Light rain shower
    {1243,  "Heavy rain shower",       "day_4_3.png",   "night_4_3.png"},   // Heavy rain shower
    {1246,  "Torrential rain shower",  "day_4_3.png",   "night_4_3.png"},   // Torrential rain shower
    {1249,  "Light sleet shower",      "day_4_4.png",   "night_4_4.png"},   // Light sleet showers
    {1252,  "Heavy sleet shower",      "day_4_4.png",   "night_4_4.png"},   // Heavy sleet showers
    {1255,  "Light snow shower",       "day_4_5.png",   "night_4_5.png"},   // Light snow showers
    {1258,  "Heavy snow shower",       "day_4_6.png",   "night_4_6.png"},   // Heavy snow showers
    {1261,  "Light ice pellet shower", "day_4_7.png",   "night_4_7.png"},   // Light ice pellet showers
    {1264,  "Heavy ice pellet shower", "day_4_7.png",   "night_4_7.png"},   // Heavy ice pellet showers
    {1273,  "Light rain with thunder", "day_4_8.png",   "night_4_8.png"},   // Light rain with thunder
    {1276,  "Heavy rain with thunder", "day_4_8.png",   "night_4_8.png"},   // Heavy rain with thunder
    {1279,  "Light snow with thunder", "day_4_8.png",   "night_4_8.png"},   // Light snow with thunder
    {1282,  "Heavy snow with thunder", "day_4_8.png",   "night_4_8.png"},   // Heavy snow with thunder
};
// clang-format on

const int WeatherIcons::NUM_CONDITIONS = sizeof(weatherConditionMap) / sizeof(weatherConditionMap[0]);

// Get PNG file path by condition code
String WeatherIcons::getPNGPath(int conditionCode, bool isDaytime)
{
  // Search for the condition code in our mapping
  for (int i = 0; i < NUM_CONDITIONS; i++)
  {
    if (weatherConditionMap[i].conditionCode == conditionCode)
    {
      const char *filename = isDaytime ? weatherConditionMap[i].dayIcon : weatherConditionMap[i].nightIcon;
      return String("/icons/") + filename;
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
  String pngPath = getPNGPath(conditionCode, isDaytime);

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
}
