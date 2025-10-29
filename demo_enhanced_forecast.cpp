#include <iostream>
#include <string>
#include <cstdio>

// Simulate the enhanced forecast functionality for testing
struct ForecastEntry
{
  std::string datetime;
  float temperature;
  std::string state;
  std::string description;
  int humidity;
  float pressure;
  float wind_speed;
  float wind_bearing;
  float cloud_coverage;
};

struct DailyForecast
{
  std::string date;
  float temp_min;
  float temp_max;
  std::string main_condition;
  std::string description;
};

void demonstrateEnhancedForecast()
{
  std::cout << "=== Enhanced Weather Forecast System ===" << std::endl;
  std::cout << std::endl;

  // Sample data based on our Beijing test
  ForecastEntry hourly[8] = {
      {"2025-10-29 12:00:00", 11.5, "partlycloudy", "few clouds", 45, 1020, 1.71, 10, 16},
      {"2025-10-29 15:00:00", 12.3, "partlycloudy", "scattered clouds", 42, 1019, 2.1, 15, 25},
      {"2025-10-29 18:00:00", 12.6, "cloudy", "broken clouds", 48, 1018, 1.8, 20, 65},
      {"2025-10-29 21:00:00", 12.3, "cloudy", "overcast clouds", 52, 1017, 1.5, 25, 85},
      {"2025-10-30 00:00:00", 13.0, "cloudy", "overcast clouds", 55, 1016, 1.2, 30, 90},
      {"2025-10-30 03:00:00", 16.4, "partlycloudy", "scattered clouds", 45, 1015, 2.5, 35, 45},
      {"2025-10-30 06:00:00", 13.9, "partlycloudy", "scattered clouds", 48, 1016, 2.0, 40, 35},
      {"2025-10-30 09:00:00", 11.8, "cloudy", "broken clouds", 50, 1018, 1.6, 45, 70}};

  DailyForecast daily[5] = {
      {"2025-10-29", 9.9, 16.4, "partlycloudy", "partly cloudy"},
      {"2025-10-30", 8.5, 18.2, "sunny", "clear sky"},
      {"2025-10-31", 10.1, 15.8, "cloudy", "overcast"},
      {"2025-11-01", 12.3, 17.5, "rainy", "light rain"},
      {"2025-11-02", 11.0, 14.2, "partlycloudy", "scattered clouds"}};

  // Display current weather
  std::cout << "Current Weather in Beijing:" << std::endl;
  std::cout << "Temperature: 9.94°C" << std::endl;
  std::cout << "Condition: Few clouds" << std::endl;
  std::cout << "Today's Range: 11.5°C - 16.4°C" << std::endl;
  std::cout << std::endl;

  // Display hourly forecast
  std::cout << "=== Next 24 Hours (3-hour intervals) ===" << std::endl;
  for (int i = 0; i < 8; i++)
  {
    printf("%-19s | %5.1f°C | %-15s | %s\n",
           hourly[i].datetime.c_str(),
           hourly[i].temperature,
           hourly[i].state.c_str(),
           hourly[i].description.c_str());
  }
  std::cout << std::endl;

  // Display daily forecast
  std::cout << "=== 5-Day Forecast ===" << std::endl;
  for (int i = 0; i < 5; i++)
  {
    printf("%-10s | %5.1f - %5.1f°C | %-15s | %s\n",
           daily[i].date.c_str(),
           daily[i].temp_min,
           daily[i].temp_max,
           daily[i].main_condition.c_str(),
           daily[i].description.c_str());
  }
  std::cout << std::endl;

  // Usage examples for ESP32
  std::cout << "=== ESP32 Usage Examples ===" << std::endl;
  std::cout << "// Get hourly forecasts" << std::endl;
  std::cout << "int count = weather_api.getHourlyForecastCount();" << std::endl;
  std::cout << "for (int i = 0; i < count; i++) {" << std::endl;
  std::cout << "  ForecastEntry forecast = weather_api.getHourlyForecast(i);" << std::endl;
  std::cout << "  Serial.println(forecast.datetime + \": \" + String(forecast.temperature) + \"°C\");" << std::endl;
  std::cout << "}" << std::endl;
  std::cout << std::endl;

  std::cout << "// Get daily forecasts" << std::endl;
  std::cout << "int days = weather_api.getDailyForecastCount();" << std::endl;
  std::cout << "for (int i = 0; i < days; i++) {" << std::endl;
  std::cout << "  DailyForecast day = weather_api.getDailyForecast(i);" << std::endl;
  std::cout << "  Serial.println(day.date + \": \" + String(day.temp_min) + \"-\" + String(day.temp_max) + \"°C\");" << std::endl;
  std::cout << "}" << std::endl;
  std::cout << std::endl;

  std::cout << "// Quick access methods" << std::endl;
  std::cout << "String summary = weather_api.getTodayForecastSummary();" << std::endl;
  std::cout << "float maxTemp = weather_api.getTodayTempRange(true);  // Get max" << std::endl;
  std::cout << "float minTemp = weather_api.getTodayTempRange(false); // Get min" << std::endl;
  std::cout << std::endl;

  std::cout << "=== New Weather Icons Supported ===" << std::endl;
  std::cout << "✓ sunny, partlycloudy, cloudy, rainy, pouring" << std::endl;
  std::cout << "✓ snowy, snowy-rainy, fog, haze, windy" << std::endl;
  std::cout << "✓ lightning, lightning-rainy, clear-night" << std::endl;
  std::cout << "✓ All mapped from OpenWeatherMap condition codes" << std::endl;
}

int main()
{
  demonstrateEnhancedForecast();
  return 0;
}
