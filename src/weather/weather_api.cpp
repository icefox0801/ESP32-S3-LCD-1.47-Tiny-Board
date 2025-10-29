// Own header
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

bool WeatherAPI::fetchWeatherData()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    return false;
  }

  if (!fetchCurrentWeather())
  {
    return false;
  }

  if (!fetchForecastData())
  {
    setFallbackForecast();
  }

  current_weather.valid = true;
  last_update = millis();
  return true;
}

bool WeatherAPI::fetchCurrentWeather()
{
  HTTPClient http;
  String url = "http://api.openweathermap.org/data/2.5/weather?q=" + 
               config.city + "," + config.country_code + 
               "&appid=" + config.api_key + 
               "&units=" + config.units;

  http.begin(url);
  int httpResponseCode = http.GET();

  if (httpResponseCode != 200)
  {
    http.end();
    return false;
  }

  String payload = http.getString();

  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, payload);

  if (error)
  {
    http.end();
    return false;
  }

  // Map OpenWeatherMap data to our structure
  current_weather.temperature = doc["main"]["temp"].as<float>();
  current_weather.temperature_unit = (config.units == "metric") ? "°C" : 
                                    (config.units == "imperial") ? "°F" : "K";
  current_weather.humidity = doc["main"]["humidity"].as<int>();
  current_weather.pressure = doc["main"]["pressure"].as<float>();
  current_weather.pressure_unit = "hPa";
  current_weather.wind_speed = doc["wind"]["speed"].as<float>();
  current_weather.wind_bearing = doc["wind"]["deg"].as<float>();
  current_weather.wind_speed_unit = (config.units == "metric") ? "m/s" : 
                                   (config.units == "imperial") ? "mph" : "m/s";
  current_weather.cloud_coverage = doc["clouds"]["all"].as<float>();
  current_weather.uv_index = 0; // UV data requires separate API call
  
  // Map weather condition to our state format
  String condition = doc["weather"][0]["main"].as<String>();
  int weather_id = doc["weather"][0]["id"].as<int>();
  current_weather.state = mapOWMConditionToState(condition, weather_id);
  
  current_weather.last_updated = String(millis());

  http.end();
  return true;
}

bool WeatherAPI::fetchForecastData()
{
  HTTPClient http;
  String url = "http://api.openweathermap.org/data/2.5/forecast?q=" + 
               config.city + "," + config.country_code + 
               "&appid=" + config.api_key + 
               "&units=" + config.units + 
               "&cnt=8"; // Get 8 forecasts (24 hours, 3-hour intervals)

  http.begin(url);
  int httpResponseCode = http.GET();

  if (httpResponseCode != 200)
  {
    http.end();
    return false;
  }

  String payload = http.getString();

  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, payload);

  if (error)
  {
    http.end();
    return false;
  }

  // Extract today's min/max from forecast data
  float min_temp = current_weather.temperature;
  float max_temp = current_weather.temperature;
  
  JsonArray forecasts = doc["list"];
  for (JsonVariant forecast : forecasts)
  {
    float temp = forecast["main"]["temp"].as<float>();
    if (temp < min_temp) min_temp = temp;
    if (temp > max_temp) max_temp = temp;
  }
  
  current_weather.temp_low = min_temp;
  current_weather.temp_high = max_temp;

  http.end();
  return true;
}

void WeatherAPI::setFallbackForecast()
{
  current_weather.temp_high = current_weather.temperature;
  current_weather.temp_low = current_weather.temperature;
}

WeatherData WeatherAPI::getCurrentWeather()
{
  return current_weather;
}

bool WeatherAPI::needsUpdate()
{
  return (millis() - last_update) > update_interval;
}

const char *WeatherAPI::getWeatherIcon(const String &weather_state)
{
  // Map weather states to text descriptions
  if (weather_state == "sunny" || weather_state == "clear")
  {
    return "SUNNY";
  }
  else if (weather_state == "partlycloudy")
  {
    return "PARTLY CLOUDY";
  }
  else if (weather_state == "cloudy")
  {
    return "CLOUDY";
  }
  else if (weather_state == "rainy")
  {
    return "RAINY";
  }
  else if (weather_state == "pouring")
  {
    return "HEAVY RAIN";
  }
  else if (weather_state == "snowy" || weather_state == "snowy-rainy")
  {
    return "SNOWY";
  }
  else if (weather_state == "fog" || weather_state == "hail")
  {
    return "FOGGY";
  }
  else if (weather_state == "windy")
  {
    return "WINDY";
  }
  else if (weather_state == "lightning" || weather_state == "lightning-rainy")
  {
    return "STORM";
  }
  else if (weather_state == "exceptional")
  {
    return "ALERT";
  }
  else
  {
    return "LOADING";
  }
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
  else
    return "?";
}

String WeatherAPI::getTemperatureString()
{
  if (!current_weather.valid)
    return "--°C";
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

String WeatherAPI::mapOWMConditionToState(const String& condition, int weather_id)
{
  // Map OpenWeatherMap weather conditions to our icon states
  // Based on OpenWeatherMap weather condition codes:
  // https://openweathermap.org/weather-conditions
  
  if (weather_id >= 200 && weather_id < 300) {
    // Thunderstorm group
    if (weather_id >= 230 && weather_id <= 232) return "lightning-rainy";
    return "lightning";
  }
  else if (weather_id >= 300 && weather_id < 400) {
    // Drizzle group
    return "rainy";
  }
  else if (weather_id >= 500 && weather_id < 600) {
    // Rain group
    if (weather_id >= 520 && weather_id <= 531) return "pouring"; // Heavy rain
    return "rainy";
  }
  else if (weather_id >= 600 && weather_id < 700) {
    // Snow group
    if (weather_id == 615 || weather_id == 616 || weather_id == 620) return "snowy-rainy";
    return "snowy";
  }
  else if (weather_id >= 700 && weather_id < 800) {
    // Atmosphere group (mist, fog, haze, etc.)
    if (weather_id == 701 || weather_id == 741) return "fog"; // Mist/Fog
    return "haze";
  }
  else if (weather_id == 800) {
    // Clear sky
    return "sunny";
  }
  else if (weather_id >= 801 && weather_id <= 804) {
    // Clouds group
    if (weather_id == 801) return "partly_cloudy"; // Few clouds
    if (weather_id == 802) return "partlycloudy"; // Scattered clouds
    return "cloudy"; // Broken/overcast clouds
  }
  
  // Fallback
  return "sunny";
}
