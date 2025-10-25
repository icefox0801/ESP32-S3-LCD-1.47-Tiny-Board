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
  String wind_speed_unit;  // "km/h"
  String last_updated;     // Last update timestamp
  bool valid;              // Data validity flag
};

// Home Assistant API configuration
struct HAConfig
{
  String server_ip = HA_SERVER_IP;
  int server_port = HA_SERVER_PORT;
  String bearer_token = HA_BEARER_TOKEN;
  String weather_entity = HA_WEATHER_ENTITY;
};

// Weather API class
class WeatherAPI
{
private:
  HAConfig config;
  WeatherData current_weather;
  unsigned long last_update = 0;
  const unsigned long update_interval = 60000; // Update every 60 seconds

  // Helper methods for breaking down fetchWeatherData
  bool fetchCurrentWeather();
  bool fetchForecastData();
  void setFallbackForecast();

public:
  WeatherAPI();

  // Initialize weather API
  bool init();

  // Fetch weather data from Home Assistant
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
