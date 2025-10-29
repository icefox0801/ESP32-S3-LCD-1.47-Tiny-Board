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
  float temperature;       // Current temperature
  String temperature_unit; // "°C"
  float temp_low;          // Low temperature (from forecast)
  float temp_high;         // High temperature (from forecast)
  int humidity;            // Humidity percentage
  float cloud_coverage;    // Cloud coverage percentage
  float uv_index;          // UV index
  float pressure;          // Atmospheric pressure
  String pressure_unit;    // "hPa"
  float wind_bearing;      // Wind direction in degrees
  float wind_speed;        // Wind speed
  String wind_speed_unit;  // "km/h" or "m/s"
  String last_updated;     // Last update timestamp
  bool valid;              // Data validity flag
};

// OpenWeatherMap API configuration
struct OWMConfig
{
  String api_key = OWM_API_KEY;
  String city = OWM_CITY;
  String country_code = OWM_COUNTRY_CODE;
  String units = OWM_UNITS;
};

// Weather API class
class WeatherAPI
{
private:
  OWMConfig config;
  WeatherData current_weather;
  unsigned long last_update = 0;
  const unsigned long update_interval = 600000; // Update every 10 minutes (OpenWeatherMap rate limit)

  // Helper methods for breaking down fetchWeatherData
  bool fetchCurrentWeather();
  bool fetchForecastData();
  void setFallbackForecast();
  String mapOWMConditionToState(const String &condition, int weather_id);

public:
  WeatherAPI();

  // Initialize weather API
  bool init();

  // Fetch weather data from OpenWeatherMap
  bool fetchWeatherData();

  // Get current weather data
  WeatherData getCurrentWeather();

  // Check if data needs updating
  bool needsUpdate();

  // Get weather icon name based on state
  const char *getWeatherIcon(const String &weather_state);

  // Get wind direction text
  String getWindDirection(float bearing);

  // Format temperature string
  String getTemperatureString();

  // Format humidity string
  String getHumidityString();

  // Format pressure string
  String getPressureString();

  // Format wind string
  String getWindString();
};

#endif // WEATHER_API_H
