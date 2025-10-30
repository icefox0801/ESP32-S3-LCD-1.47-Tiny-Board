#include "weather_api.h"
#include "../debug.h"

WeatherAPI::WeatherAPI()
{
  current_weather.valid = false;
}

bool WeatherAPI::init()
{
  current_weather.valid = false;
  last_update = 0;
  return true;
}

WeatherData WeatherAPI::getCurrentWeather()
{
  return current_weather;
}

bool WeatherAPI::fetchWeatherData()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    return false;
  }

  // Always use WeatherAPI.com - simplified approach
  if (!fetchCurrentAndTodayWeatherAPI())
  {
    return false;
  }

  current_weather.valid = true;
  last_update = millis();
  time(&last_update_time); // Capture the system time when data was fetched
  DEBUG_LOGF("Weather fetched at: %lu\n", (unsigned long)last_update_time);
  return true;
}

bool WeatherAPI::fetchCurrentAndTodayWeatherAPI()
{
  HTTPClient http;
  String url = "http://api.weatherapi.com/v1/forecast.json?key=" +
               weatherapi_config.api_key +
               "&q=" + weatherapi_config.location +
               "&days=1&aqi=yes&alerts=no";

  http.begin(url);
  int httpResponseCode = http.GET();

  if (httpResponseCode != 200)
  {
    http.end();
    return false;
  }

  String payload = http.getString();
  http.end();

  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, payload);

  if (error)
  {
    return false;
  }

  // Parse current weather
  current_weather.temperature = doc["current"]["temp_c"].as<float>();
  current_weather.temperature_unit = "°C";
  current_weather.humidity = doc["current"]["humidity"].as<int>();

  // Parse air quality data
  if (doc["current"]["air_quality"])
  {
    current_weather.air_quality_pm25 = doc["current"]["air_quality"]["pm2_5"].as<int>();
    current_weather.air_quality_us_epa = doc["current"]["air_quality"]["us-epa-index"].as<int>();
  }
  else
  {
    current_weather.air_quality_pm25 = 0;
    current_weather.air_quality_us_epa = 0;
  }

  // Get condition code directly from WeatherAPI.com
  current_weather.condition_code = doc["current"]["condition"]["code"].as<int>();

  // Get today's min/max from forecast data
  JsonObject today_forecast = doc["forecast"]["forecastday"][0]["day"];
  current_weather.temp_high = today_forecast["maxtemp_c"].as<float>();
  current_weather.temp_low = today_forecast["mintemp_c"].as<float>();

  current_weather.last_updated = String(millis());

  DEBUG_LOGF("Temp: %.1f°C, Range: %.1f-%.1f°C, Condition: %d\n",
             current_weather.temperature, current_weather.temp_low,
             current_weather.temp_high, current_weather.condition_code);

  return true;
}

String WeatherAPI::getTemperatureString()
{
  if (!current_weather.valid)
    return "--°";
  return String(current_weather.temperature, 1) + current_weather.temperature_unit;
}

String WeatherAPI::getHumidityString()
{
  if (!current_weather.valid)
    return "--%";
  return String(current_weather.humidity) + "%";
}

bool WeatherAPI::needsUpdate()
{
  return (millis() - last_update) > update_interval;
}

time_t WeatherAPI::getLastUpdateTime()
{
  return last_update_time;
}

String WeatherAPI::getAirQualityString()
{
  if (!current_weather.valid || current_weather.air_quality_us_epa == 0)
    return "--";
  return String(current_weather.air_quality_pm25);
}

String WeatherAPI::getAirQualityCategory()
{
  if (!current_weather.valid || current_weather.air_quality_us_epa == 0)
    return "N/A";

  // US EPA Air Quality Index (1-6)
  switch (current_weather.air_quality_us_epa)
  {
  case 1:
    return "Good";
  case 2:
    return "Moderate";
  case 3:
    return "Unhealthy for Sensitive";
  case 4:
    return "Unhealthy";
  case 5:
    return "Very Unhealthy";
  case 6:
    return "Hazardous";
  default:
    return "Unknown";
  }
}
