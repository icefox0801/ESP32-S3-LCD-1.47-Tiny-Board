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

    if (weather_api->fetchWeatherData())
    {
      weather_ui->updateWeatherDisplay();
    }

    weather_ui->showWeatherScreen();
    weather_ui->startAutoUpdate();
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
