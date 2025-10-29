// System libraries
#include <Arduino.h>

// Project headers
#include "config.h"
#include "lvgl/lvgl_setup.h"
#include "ui/ui_weather.h"
#include "weather/weather_api.h"
#include "wifi/wifi_setup.h"

// Global objects
WiFiSetup *wifi_setup;
WeatherAPI *weather_api;
WeatherUI *weather_ui;

void setup()
{
  Serial.begin(115200);
  delay(2000);

  // Initialize LVGL
  lvgl_setup();

  // Initialize WiFi
  wifi_setup = new WiFiSetup();
  wifi_setup->init();

  // Connect to WiFi and setup weather
  if (wifi_setup->connect())
  {
    weather_api = new WeatherAPI();
    weather_api->init();

    weather_ui = new WeatherUI(weather_api);
    weather_ui->createWeatherScreen();

    Serial.println("Fetching weather data...");
    if (weather_api->fetchWeatherData())
    {
      Serial.println("Weather data fetched successfully");
      Serial.println("Current temperature: " + weather_api->getTemperatureString());
      Serial.println("Current humidity: " + weather_api->getHumidityString());
      Serial.println("Current wind: " + weather_api->getWindString());
      weather_ui->updateWeatherDisplay();
    }
    else
    {
      Serial.println("Failed to fetch weather data");
    }

    weather_ui->showWeatherScreen();
    weather_ui->startAutoUpdate();
  }
  else
  {
    // Show a basic screen even without WiFi
    weather_api = new WeatherAPI();
    weather_ui = new WeatherUI(weather_api);
    weather_ui->createWeatherScreen();
    weather_ui->showWeatherScreen();
  }
}

void loop()
{
  lv_timer_handler();

  if (wifi_setup)
  {
    wifi_setup->handleReconnect();
  }

  delay(5);
}
