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
  String url = "http://" + config.server_ip + ":" + String(config.server_port) +
               "/api/states/" + config.weather_entity;

  http.begin(url);
  http.addHeader("Authorization", "Bearer " + config.bearer_token);
  http.addHeader("Content-Type", "application/json");

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

  current_weather.state = doc["state"].as<String>();
  current_weather.temperature = doc["attributes"]["temperature"].as<float>();
  current_weather.temperature_unit = doc["attributes"]["temperature_unit"].as<String>();
  current_weather.humidity = doc["attributes"]["humidity"].as<int>();
  current_weather.cloud_coverage = doc["attributes"]["cloud_coverage"].as<float>();
  current_weather.uv_index = doc["attributes"]["uv_index"].as<float>();
  current_weather.pressure = doc["attributes"]["pressure"].as<float>();
  current_weather.pressure_unit = doc["attributes"]["pressure_unit"].as<String>();
  current_weather.wind_bearing = doc["attributes"]["wind_bearing"].as<float>();
  current_weather.wind_speed = doc["attributes"]["wind_speed"].as<float>();
  current_weather.wind_speed_unit = doc["attributes"]["wind_speed_unit"].as<String>();
  current_weather.last_updated = doc["last_updated"].as<String>();

  http.end();
  return true;
}

bool WeatherAPI::fetchForecastData()
{
  HTTPClient http;
  String forecastUrl = "http://" + config.server_ip + ":" + String(config.server_port) +
                       "/api/services/weather/get_forecasts?return_response=true";

  http.begin(forecastUrl);
  http.addHeader("Authorization", "Bearer " + config.bearer_token);
  http.addHeader("Content-Type", "application/json");

  String postData = "{\"entity_id\": \"" + config.weather_entity + "\", \"type\": \"daily\"}";
  int httpResponseCode = http.POST(postData);

  if (httpResponseCode != 200)
  {
    http.end();
    return false;
  }

  String forecastPayload = http.getString();

  JsonDocument forecastDoc;
  DeserializationError forecastError = deserializeJson(forecastDoc, forecastPayload);

  if (forecastError)
  {
    http.end();
    return false;
  }

  JsonObject serviceResponse = forecastDoc["service_response"];
  JsonVariant entityVariant = serviceResponse[config.weather_entity];

  if (entityVariant.isNull())
  {
    http.end();
    return false;
  }

  JsonArray forecast = entityVariant["forecast"];
  if (forecast.size() == 0)
  {
    http.end();
    return false;
  }

  JsonObject today = forecast[0];
  current_weather.temp_high = today["temperature"].as<float>();
  current_weather.temp_low = today["templow"].as<float>();

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
    return "-- km/h";
  return String(current_weather.wind_speed, 1) + " " + current_weather.wind_speed_unit +
         " " + getWindDirection(current_weather.wind_bearing);
}
