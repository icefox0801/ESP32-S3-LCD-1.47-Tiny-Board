// Own header
#include "ui_weather.h"

WeatherUI::WeatherUI(WeatherAPI *api) : weather_api(api)
{
  weather_screen = nullptr;
  weather_container = nullptr;
  weather_icon_label = nullptr;
  temperature_label = nullptr;
  humidity_label = nullptr;
  pressure_label = nullptr;
  wind_label = nullptr;
  status_label = nullptr;
  last_update_label = nullptr;
  update_timer = nullptr;
}

void WeatherUI::createWeatherScreen()
{
  Serial.println("[WeatherUI] Creating weather screen...");

  // Create main weather screen
  weather_screen = lv_obj_create(NULL);
  lv_obj_set_size(weather_screen, LV_HOR_RES, LV_VER_RES);
  lv_obj_set_style_bg_color(weather_screen, lv_color_hex(0x0a0a0a), LV_PART_MAIN);

  // Create container for weather elements - minimal design
  weather_container = lv_obj_create(weather_screen);
  lv_obj_set_size(weather_container, LV_HOR_RES - 10, LV_VER_RES - 10);
  lv_obj_center(weather_container);
  lv_obj_set_style_bg_color(weather_container, lv_color_hex(0x000000), LV_PART_MAIN);
  lv_obj_set_style_border_width(weather_container, 0, LV_PART_MAIN);
  lv_obj_set_style_radius(weather_container, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(weather_container, 0, LV_PART_MAIN);

  // Weather status - positioned at top third of screen with wrapping
  weather_icon_label = lv_label_create(weather_container);
  lv_label_set_text(weather_icon_label, "LOADING");
  lv_label_set_long_mode(weather_icon_label, LV_LABEL_LONG_WRAP);
  lv_obj_set_width(weather_icon_label, LV_HOR_RES - 30);
  lv_obj_set_style_text_align(weather_icon_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
  lv_obj_set_style_text_font(weather_icon_label, &lv_font_montserrat_24, LV_PART_MAIN);
  lv_obj_set_style_text_color(weather_icon_label, lv_color_hex(0x00d4ff), LV_PART_MAIN);
  lv_obj_align(weather_icon_label, LV_ALIGN_CENTER, 0, -60);

  // Temperature - extra large and centered
  temperature_label = lv_label_create(weather_container);
  lv_label_set_text(temperature_label, "--°C");
  lv_obj_set_style_text_font(temperature_label, &lv_font_montserrat_48, LV_PART_MAIN);
  lv_obj_set_style_text_color(temperature_label, lv_color_hex(0xffffff), LV_PART_MAIN);
  lv_obj_align(temperature_label, LV_ALIGN_CENTER, 0, 15);

  // Hide unused elements (set to NULL initially)
  humidity_label = nullptr;
  pressure_label = nullptr;
  wind_label = nullptr;
  status_label = nullptr;
  last_update_label = nullptr;

  Serial.println("[WeatherUI] Weather screen created successfully");
}

void WeatherUI::updateWeatherDisplay()
{
  if (!weather_api || !weather_container)
    return;

  WeatherData weather = weather_api->getCurrentWeather();

  if (weather.valid)
  {
    // Update weather status text
    const char *icon = weather_api->getWeatherIcon(weather.state);
    lv_label_set_text(weather_icon_label, icon);

    // Update temperature
    String temp_str = weather_api->getTemperatureString();
    lv_label_set_text(temperature_label, temp_str.c_str());

    Serial.println("[WeatherUI] Weather display updated");
  }
  else
  {
    // Show error state
    lv_label_set_text(weather_icon_label, "ERROR");
    lv_label_set_text(temperature_label, "--°C");
    Serial.println("[WeatherUI] Error: No valid weather data");
  }
}

void WeatherUI::showWeatherScreen()
{
  if (weather_screen)
  {
    lv_scr_load(weather_screen);
    Serial.println("[WeatherUI] Weather screen shown");
  }
}

void WeatherUI::hideWeatherScreen()
{
  // Implementation for hiding if needed
}

lv_obj_t *WeatherUI::getWeatherScreen()
{
  return weather_screen;
}

void WeatherUI::startAutoUpdate()
{
  if (!update_timer)
  {
    update_timer = lv_timer_create(update_timer_cb, 30000, this); // Update every 30 seconds
    Serial.println("[WeatherUI] Auto-update timer started");
  }
}

void WeatherUI::stopAutoUpdate()
{
  if (update_timer)
  {
    lv_timer_del(update_timer);
    update_timer = nullptr;
    Serial.println("[WeatherUI] Auto-update timer stopped");
  }
}

void WeatherUI::update_timer_cb(lv_timer_t *timer)
{
  WeatherUI *ui = (WeatherUI *)lv_timer_get_user_data(timer);
  ui->handleUpdateTimer();
}

void WeatherUI::handleUpdateTimer()
{
  Serial.println("[WeatherUI] Auto-update triggered");

  // Fetch new weather data
  if (weather_api && weather_api->needsUpdate())
  {
    Serial.println("[WeatherUI] Fetching fresh weather data...");
    if (weather_api->fetchWeatherData())
    {
      updateWeatherDisplay();
    }
    else
    {
      Serial.println("[WeatherUI] Failed to fetch weather data");
    }
  }
}
