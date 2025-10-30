// System libraries
#include <Arduino.h>
#include <SPIFFS.h>

// Project headers
#include "config.h"
#include "debug.h"
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

  LOG_INFO("\n\n=== ESP32-S3 Weather Display Starting ===");
  LOG_INFOF("Built: %s %s\n", __DATE__, __TIME__);

  // Initialize LVGL (this will also initialize LittleFS via lvgl_fs_spiffs_init)
  DEBUG_LOG("Initializing LVGL...");
  lvgl_setup();

  // Initialize WiFi
  DEBUG_LOG("Initializing WiFi...");
  wifi_setup = new WiFiSetup();
  wifi_setup->init();

  // Connect to WiFi and setup weather
  if (wifi_setup->connect())
  {
    LOG_INFO("WiFi connected!");
    DEBUG_LOG("Initializing weather API...");
    weather_api = new WeatherAPI();
    weather_api->init();

    DEBUG_LOG("Creating weather UI...");
    weather_ui = new WeatherUI(weather_api);
    weather_ui->createWeatherScreen();

    DEBUG_LOG("Fetching initial weather data...");
    if (weather_api->fetchWeatherData())
    {
      DEBUG_LOG("Weather fetch successful, updating display...");
      weather_ui->updateWeatherDisplay();
    }
    else
    {
      LOG_ERROR("Weather fetch failed!");
    }

    weather_ui->showWeatherScreen();
  }
  else
  {
    LOG_ERROR("WiFi connection failed!");
    // Show a basic screen even without WiFi
    weather_api = new WeatherAPI();
    weather_ui = new WeatherUI(weather_api);
    weather_ui->createWeatherScreen();
    weather_ui->showWeatherScreen();
  }

  LOG_INFO("=== Setup Complete ===\n");
}

void loop()
{
  // Periodic weather update check (every 5 minutes)
  static unsigned long last_update_check = 0;
  unsigned long now = millis();

  if (now - last_update_check >= 300000)
  { // 5 minutes = 300000ms
    last_update_check = now;

    if (weather_api && weather_api->needsUpdate())
    {
      DEBUG_LOG("Fetching weather update...");
      if (weather_api->fetchWeatherData())
      {
        DEBUG_LOG("Weather updated successfully");
        if (weather_ui)
        {
          weather_ui->updateWeatherDisplay();
        }
      }
      else
      {
        LOG_ERROR("Weather fetch failed");
      }
    }
  }

  lv_timer_handler();

  if (wifi_setup)
  {
    wifi_setup->handleReconnect();
  }

  delay(5);
}
