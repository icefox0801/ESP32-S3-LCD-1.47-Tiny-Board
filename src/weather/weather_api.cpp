#include "weather_api.h"

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

  DynamicJsonDocument doc(4096);
  DeserializationError error = deserializeJson(doc, payload);

  if (error)
  {
    return false;
  }

  // Parse current weather
  current_weather.temperature = doc["current"]["temp_c"].as<float>();
  current_weather.temperature_unit = "°C";
  current_weather.humidity = doc["current"]["humidity"].as<int>();
  current_weather.pressure = doc["current"]["pressure_mb"].as<float>();
  current_weather.pressure_unit = "hPa";
  current_weather.wind_speed = doc["current"]["wind_kph"].as<float>() / 3.6; // Convert to m/s
  current_weather.wind_bearing = doc["current"]["wind_degree"].as<float>();
  current_weather.wind_speed_unit = "m/s";
  current_weather.cloud_coverage = doc["current"]["cloud"].as<float>();
  current_weather.uv_index = doc["current"]["uv"].as<float>();

  // Get condition code directly from WeatherAPI.com
  current_weather.condition_code = doc["current"]["condition"]["code"].as<int>();

  // Get today's min/max from forecast data
  JsonObject today_forecast = doc["forecast"]["forecastday"][0]["day"];
  current_weather.temp_high = today_forecast["maxtemp_c"].as<float>();
  current_weather.temp_low = today_forecast["mintemp_c"].as<float>();

  current_weather.last_updated = String(millis());

  Serial.println("WeatherAPI.com: Current temp: " + String(current_weather.temperature) + "°C");
  Serial.println("WeatherAPI.com: Today's range: " + String(current_weather.temp_low) + "°C to " + String(current_weather.temp_high) + "°C");
  Serial.println("WeatherAPI.com: Condition code: " + String(current_weather.condition_code));

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

String WeatherAPI::getPressureString()
{
  if (!current_weather.valid)
    return "-- hPa";
  return String(current_weather.pressure, 1) + " " + current_weather.pressure_unit;
}

String WeatherAPI::getWindString()
{
  if (!current_weather.valid)
    return "-- m/s";
  return String(current_weather.wind_speed, 1) + " " + current_weather.wind_speed_unit +
         " " + getWindDirection(current_weather.wind_bearing);
}

String WeatherAPI::getWindDirection(float bearing)
{
  if (bearing >= 337.5 || bearing < 22.5)
    return "N";
  else if (bearing >= 22.5 && bearing < 67.5)
    return "NE";
  else if (bearing >= 67.5 && bearing < 112.5)
    return "E";
  else if (bearing >= 112.5 && bearing < 157.5)
    return "SE";
  else if (bearing >= 157.5 && bearing < 202.5)
    return "S";
  else if (bearing >= 202.5 && bearing < 247.5)
    return "SW";
  else if (bearing >= 247.5 && bearing < 292.5)
    return "W";
  else if (bearing >= 292.5 && bearing < 337.5)
    return "NW";
  return "N";
}

// Private helper methods
bool WeatherAPI::fetchCurrentWeather()
{
  return fetchCurrentWeatherAPI();
}

bool WeatherAPI::fetchForecastData()
{
  return fetchForecastDataAPI();
}

bool WeatherAPI::fetchExtendedForecast()
{
  // Extended forecast not needed for now
  return true;
}

void WeatherAPI::setFallbackForecast()
{
  if (!current_weather.valid)
    return;

  // Set fallback values using current weather
  current_weather.temp_low = current_weather.temperature - 3;
  current_weather.temp_high = current_weather.temperature + 3;
}

bool WeatherAPI::fetchCurrentWeatherAPI()
{
  HTTPClient http;
  String url = "http://api.weatherapi.com/v1/current.json?key=" +
               weatherapi_config.api_key +
               "&q=" + weatherapi_config.location +
               "&aqi=yes";

  http.begin(url);
  int httpResponseCode = http.GET();

  if (httpResponseCode != 200)
  {
    http.end();
    return false;
  }

  String payload = http.getString();
  http.end();

  DynamicJsonDocument doc(2048);
  DeserializationError error = deserializeJson(doc, payload);

  if (error)
  {
    return false;
  }

  current_weather.temperature = doc["current"]["temp_c"].as<float>();
  current_weather.temperature_unit = "°C";
  current_weather.humidity = doc["current"]["humidity"].as<int>();
  current_weather.pressure = doc["current"]["pressure_mb"].as<float>();
  current_weather.pressure_unit = "hPa";
  current_weather.wind_speed = doc["current"]["wind_kph"].as<float>() / 3.6; // Convert to m/s
  current_weather.wind_bearing = doc["current"]["wind_degree"].as<float>();
  current_weather.wind_speed_unit = "m/s";
  current_weather.cloud_coverage = doc["current"]["cloud"].as<float>();
  current_weather.uv_index = doc["current"]["uv"].as<float>();

  // Set initial min/max to current temp (will be updated by forecast if available)
  current_weather.temp_low = current_weather.temperature;
  current_weather.temp_high = current_weather.temperature;

  // Get condition code directly from WeatherAPI.com
  current_weather.condition_code = doc["current"]["condition"]["code"].as<int>();

  current_weather.last_updated = String(millis());
  return true;
}

bool WeatherAPI::fetchForecastDataAPI()
{
  HTTPClient http;
  String url = "http://api.weatherapi.com/v1/forecast.json?key=" +
               weatherapi_config.api_key +
               "&q=" + weatherapi_config.location +
               "&days=1&aqi=no&alerts=no";

  http.begin(url);
  int httpResponseCode = http.GET();

  if (httpResponseCode != 200)
  {
    http.end();
    return false;
  }

  String payload = http.getString();
  http.end();

  DynamicJsonDocument doc(4096);
  DeserializationError error = deserializeJson(doc, payload);

  if (error)
  {
    return false;
  }

  // Get today's min/max from day forecast
  JsonObject day_forecast = doc["forecast"]["forecastday"][0]["day"];
  current_weather.temp_high = day_forecast["maxtemp_c"].as<float>();
  current_weather.temp_low = day_forecast["mintemp_c"].as<float>();

  return true;
}

// Public forecast accessor methods
int WeatherAPI::getHourlyForecastCount()
{
  return hourly_count;
}

ForecastEntry WeatherAPI::getHourlyForecast(int index)
{
  if (index >= 0 && index < hourly_count)
  {
    return hourly_forecasts[index];
  }
  return ForecastEntry(); // Return empty entry
}

int WeatherAPI::getDailyForecastCount()
{
  return daily_count;
}

DailyForecast WeatherAPI::getDailyForecast(int index)
{
  if (index >= 0 && index < daily_count)
  {
    return daily_forecasts[index];
  }
  return DailyForecast(); // Return empty entry
}

String WeatherAPI::getTodayForecastSummary()
{
  if (!current_weather.valid)
    return "No data available";

  return "Today: " + String(current_weather.temp_low, 0) + "°C to " + String(current_weather.temp_high, 0) + "°C";
}

float WeatherAPI::getTodayTempRange(bool getMax)
{
  if (!current_weather.valid)
    return 0.0;

  return getMax ? current_weather.temp_high : current_weather.temp_low;
}

const char *WeatherAPI::getWeatherIcon(const String &weather_state)
{
  // Return simple fallback - not needed since we're using emojis
  return "";
}

bool WeatherAPI::needsUpdate()
{
  return (millis() - last_update) > update_interval;
}
