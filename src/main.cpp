#include <Arduino.h>
#include "config.h"
#include "lvgl/lvgl_setup.h"
#include "lvgl/lvgl_ui.h"
#include "wifi/wifi_setup.h"
#include "weather/weather_api.h"
#include "weather/weather_ui.h"

// Global objects
WiFiSetup *wifi_setup;
WeatherAPI *weather_api;
WeatherUI *weather_ui;

void setup()
{
  // Initialize USB-Serial communication
  Serial.begin(115200);

  // Wait for USB-Serial to be ready
  delay(2000); // Give time for USB serial to initialize

  Serial.println("=== ESP32-S3 Weather Display Starting ===");

  // Initialize LVGL with modular setup
  lvgl_setup();

  // Initialize WiFi
  wifi_setup = new WiFiSetup(WIFI_SSID, WIFI_PASSWORD);
  wifi_setup->init();

  // Connect to WiFi
  if (wifi_setup->connect())
  {
    Serial.println("[Main] WiFi connected successfully");

    // Initialize weather API
    weather_api = new WeatherAPI();
    weather_api->init();

    // Initialize weather UI
    weather_ui = new WeatherUI(weather_api);
    weather_ui->createWeatherScreen();

    // Fetch initial weather data
    Serial.println("[Main] Fetching initial weather data...");
    if (weather_api->fetchWeatherData())
    {
      Serial.println("[Main] Initial weather data fetched successfully");
      weather_ui->updateWeatherDisplay();
    }
    else
    {
      Serial.println("[Main] Failed to fetch initial weather data");
    }

    // Show weather screen
    weather_ui->showWeatherScreen();

    // Start automatic weather updates
    weather_ui->startAutoUpdate();

    Serial.println("[Main] Weather display system initialized");
  }
  else
  {
    Serial.println("[Main] WiFi connection failed - running without weather");

    // Optionally create basic UI without weather
    // ui_create_main_screen();
  }

  Serial.println("Main setup complete. Starting main loop...");
}

void loop()
{
  // Handle LVGL tasks
  lv_timer_handler();

  // Handle WiFi reconnection if needed
  if (wifi_setup)
  {
    wifi_setup->handleReconnect();
  }

  // Add a small delay to prevent watchdog timeout
  delay(5);
}
