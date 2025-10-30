// System libraries
#include <Arduino.h>
#include <SPIFFS.h>

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

  Serial.println("\n\n=== ESP32-S3 Weather Display Starting ===");
  Serial.println("Built: " __DATE__ " " __TIME__);

  // Initialize LVGL (this will also initialize LittleFS via lvgl_fs_spiffs_init)
  Serial.println("Initializing LVGL...");
  lvgl_setup();

  // Initialize WiFi
  Serial.println("Initializing WiFi...");
  wifi_setup = new WiFiSetup();
  wifi_setup->init();

  // Connect to WiFi and setup weather
  if (wifi_setup->connect())
  {
    Serial.println("WiFi connected! Initializing weather API...");
    weather_api = new WeatherAPI();
    weather_api->init();

    Serial.println("Creating weather UI...");
    weather_ui = new WeatherUI(weather_api);
    weather_ui->createWeatherScreen();

    Serial.println("Fetching initial weather data...");
    if (weather_api->fetchWeatherData())
    {
      Serial.println("Weather fetch successful, updating display...");
      weather_ui->updateWeatherDisplay();
    }
    else
    {
      Serial.println("Weather fetch FAILED!");
    }

    weather_ui->showWeatherScreen();
    Serial.println("Starting auto-update timer (5min check, 10min update)...");
    weather_ui->startAutoUpdate();
  }
  else
  {
    Serial.println("WiFi connection FAILED! Showing error screen...");
    // Show a basic screen even without WiFi
    weather_api = new WeatherAPI();
    weather_ui = new WeatherUI(weather_api);
    weather_ui->createWeatherScreen();
    weather_ui->showWeatherScreen();
  }

  Serial.println("=== Setup Complete ===\n");
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
      Serial.println(">>> Fetching weather update from loop()...");
      if (weather_api->fetchWeatherData())
      {
        Serial.println(">>> Fetch successful, updating display...");
        if (weather_ui)
        {
          weather_ui->updateWeatherDisplay();
        }
      }
      else
      {
        Serial.println(">>> Fetch FAILED!");
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
