#ifndef WEATHER_API_H
#define WEATHER_API_H

// System libraries
#include <HTTPClient.h>
#include <WiFi.h>

// Third-party libraries
#include <ArduinoJson.h>
#include <lvgl.h>

// Project headers
#include "secrets.h"

// Weather data structure
struct WeatherData
{
  String state;            // "cloudy", "sunny", "rainy", etc.
  int condition_code;      // WeatherAPI.com condition code
  float temperature;       // Current temperature
  String temperature_unit; // "°C"
  float temp_low;          // Low temperature (from forecast)
  float temp_high;         // High temperature (from forecast)
  int humidity;            // Humidity percentage
  int air_quality_pm25;    // Air Quality PM2.5 (μg/m³)
  int air_quality_us_epa;  // US EPA Air Quality Index (1-6)
  String last_updated;     // Last update timestamp
  bool valid;              // Data validity flag
};

// WeatherAPI.com configuration
struct WeatherAPIConfig
{
  String api_key = WEATHER_API_KEY;
  String location = WEATHER_LOCATION;
  String units = WEATHER_UNITS;
};

// Weather API class
class WeatherAPI
{
private:
  WeatherAPIConfig weatherapi_config;
  WeatherData current_weather;
  unsigned long last_update = 0;
  time_t last_update_time = 0;                  // System time when data was last fetched
  const unsigned long update_interval = 600000; // Update every 10 minutes

  // WeatherAPI.com method - fetches current weather and today's min/max in one call
  bool fetchCurrentAndTodayWeatherAPI();

public:
  WeatherAPI();

  // Initialize weather API
  bool init();

  // Fetch weather data from WeatherAPI.com
  bool fetchWeatherData();

  // Get current weather data
  WeatherData getCurrentWeather();

  // Get the time when weather data was last fetched
  time_t getLastUpdateTime();

  // Check if data needs updating
  bool needsUpdate();

  // Format temperature string
  String getTemperatureString();

  // Format humidity string
  String getHumidityString();

  // Format air quality string
  String getAirQualityString();
};

#endif // WEATHER_API_H
