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

// Forecast entry structure
struct ForecastEntry
{
  String datetime;      // Forecast date/time
  float temperature;    // Temperature
  String state;         // Weather condition
  String description;   // Weather description
  int condition_code;   // WeatherAPI.com condition code
  int humidity;         // Humidity percentage
  float pressure;       // Atmospheric pressure
  float wind_speed;     // Wind speed
  float wind_bearing;   // Wind direction
  float cloud_coverage; // Cloud coverage percentage
};

// Daily forecast summary
struct DailyForecast
{
  String date;           // Date (YYYY-MM-DD)
  float temp_min;        // Minimum temperature
  float temp_max;        // Maximum temperature
  String main_condition; // Primary weather condition
  String description;    // Weather description
};

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
  float cloud_coverage;    // Cloud coverage percentage
  float uv_index;          // UV index
  float pressure;          // Atmospheric pressure
  String pressure_unit;    // "hPa"
  float wind_bearing;      // Wind direction in degrees
  float wind_speed;        // Wind speed
  String wind_speed_unit;  // "km/h" or "m/s"
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
  time_t last_update_time = 0; // System time when data was last fetched
  const float latitude = 40.0;
  const float longitude = 116.5;
  const unsigned long update_interval = 600000; // Update every 10 minutes

  // Forecast data storage
  static const int MAX_HOURLY_FORECASTS = 3; // Next 9 hours (3-hour intervals)
  static const int MAX_DAILY_FORECASTS = 3;  // Next 3 days
  ForecastEntry hourly_forecasts[MAX_HOURLY_FORECASTS];
  DailyForecast daily_forecasts[MAX_DAILY_FORECASTS];
  int hourly_count = 0;
  int daily_count = 0;

  // Helper methods for WeatherAPI.com
  bool fetchCurrentWeather();
  bool fetchForecastData();
  bool fetchExtendedForecast();
  void setFallbackForecast();

  // WeatherAPI.com specific methods - clean and simple
  bool fetchCurrentAndTodayWeatherAPI(); // Optimized single call for current + today's min/max
  bool fetchCurrentWeatherAPI();
  bool fetchForecastDataAPI();

  void processDailyForecasts();

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

  // Forecast data access methods
  int getHourlyForecastCount();
  ForecastEntry getHourlyForecast(int index);
  int getDailyForecastCount();
  DailyForecast getDailyForecast(int index);

  // Get today's detailed forecast
  String getTodayForecastSummary();
  float getTodayTempRange(bool getMax); // true for max, false for min

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

  // Format air quality string
  String getAirQualityString();

  // Get air quality category name
  String getAirQualityCategory();
};

#endif // WEATHER_API_H
