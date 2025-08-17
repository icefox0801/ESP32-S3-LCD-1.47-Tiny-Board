#include "weather_ui.h"

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

  // Create container for weather elements
  weather_container = lv_obj_create(weather_screen);
  lv_obj_set_size(weather_container, LV_HOR_RES - 20, LV_VER_RES - 20);
  lv_obj_center(weather_container);
  lv_obj_set_style_bg_color(weather_container, lv_color_hex(0x1a1a1a), LV_PART_MAIN);
  lv_obj_set_style_border_color(weather_container, lv_color_hex(0x333333), LV_PART_MAIN);
  lv_obj_set_style_border_width(weather_container, 2, LV_PART_MAIN);
  lv_obj_set_style_radius(weather_container, 10, LV_PART_MAIN);

  // Weather icon (large, centered at top)
  weather_icon_label = lv_label_create(weather_container);
  lv_label_set_text(weather_icon_label, LV_SYMBOL_DUMMY);
  lv_obj_set_style_text_font(weather_icon_label, &lv_font_montserrat_14, LV_PART_MAIN);
  lv_obj_set_style_text_color(weather_icon_label, lv_color_hex(0x00aaff), LV_PART_MAIN);
  lv_obj_align(weather_icon_label, LV_ALIGN_TOP_MID, 0, 10);

  // Temperature (large, below icon)
  temperature_label = lv_label_create(weather_container);
  lv_label_set_text(temperature_label, "--°C");
  lv_obj_set_style_text_font(temperature_label, &lv_font_montserrat_14, LV_PART_MAIN);
  lv_obj_set_style_text_color(temperature_label, lv_color_hex(0xffffff), LV_PART_MAIN);
  lv_obj_align_to(temperature_label, weather_icon_label, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);

  // Humidity row
  humidity_label = lv_label_create(weather_container);
  lv_label_set_text(humidity_label, "Humidity: --%");
  lv_obj_set_style_text_font(humidity_label, &lv_font_montserrat_14, LV_PART_MAIN);
  lv_obj_set_style_text_color(humidity_label, lv_color_hex(0xaaaaaa), LV_PART_MAIN);
  lv_obj_align_to(humidity_label, temperature_label, LV_ALIGN_OUT_BOTTOM_LEFT, -30, 15);

  // Pressure row
  pressure_label = lv_label_create(weather_container);
  lv_label_set_text(pressure_label, "Pressure: -- hPa");
  lv_obj_set_style_text_font(pressure_label, &lv_font_montserrat_14, LV_PART_MAIN);
  lv_obj_set_style_text_color(pressure_label, lv_color_hex(0xaaaaaa), LV_PART_MAIN);
  lv_obj_align_to(pressure_label, humidity_label, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 8);

  // Wind row
  wind_label = lv_label_create(weather_container);
  lv_label_set_text(wind_label, "Wind: -- km/h");
  lv_obj_set_style_text_font(wind_label, &lv_font_montserrat_14, LV_PART_MAIN);
  lv_obj_set_style_text_color(wind_label, lv_color_hex(0xaaaaaa), LV_PART_MAIN);
  lv_obj_align_to(wind_label, pressure_label, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 8);

  // Status label
  status_label = lv_label_create(weather_container);
  lv_label_set_text(status_label, "Initializing...");
  lv_obj_set_style_text_font(status_label, &lv_font_montserrat_14, LV_PART_MAIN);
  lv_obj_set_style_text_color(status_label, lv_color_hex(0x888888), LV_PART_MAIN);
  lv_obj_align_to(status_label, wind_label, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 15);

  // Last update label
  last_update_label = lv_label_create(weather_container);
  lv_label_set_text(last_update_label, "Last update: Never");
  lv_obj_set_style_text_font(last_update_label, &lv_font_montserrat_14, LV_PART_MAIN);
  lv_obj_set_style_text_color(last_update_label, lv_color_hex(0x666666), LV_PART_MAIN);
  lv_obj_align(last_update_label, LV_ALIGN_BOTTOM_MID, 0, -10);

  Serial.println("[WeatherUI] Weather screen created successfully");
}

void WeatherUI::updateWeatherDisplay()
{
  if (!weather_api || !weather_container)
    return;

  WeatherData weather = weather_api->getCurrentWeather();

  if (weather.valid)
  {
    // Update weather icon
    const char *icon = weather_api->getWeatherIcon(weather.state);
    lv_label_set_text(weather_icon_label, icon);

    // Update temperature
    String temp_str = weather_api->getTemperatureString();
    lv_label_set_text(temperature_label, temp_str.c_str());

    // Update humidity
    String humidity_str = "Humidity: " + weather_api->getHumidityString();
    lv_label_set_text(humidity_label, humidity_str.c_str());

    // Update pressure
    String pressure_str = "Pressure: " + weather_api->getPressureString();
    lv_label_set_text(pressure_label, pressure_str.c_str());

    // Update wind
    String wind_str = "Wind: " + weather_api->getWindString();
    lv_label_set_text(wind_label, wind_str.c_str());

    // Update status
    String status_str = "Weather: " + weather.state;
    lv_label_set_text(status_label, status_str.c_str());

    // Update last update time
    String update_str = "Updated: " + String(millis() / 1000) + "s ago";
    lv_label_set_text(last_update_label, update_str.c_str());

    Serial.println("[WeatherUI] Weather display updated");
  }
  else
  {
    // Show error state
    lv_label_set_text(weather_icon_label, LV_SYMBOL_WARNING);
    lv_label_set_text(temperature_label, "--°C");
    lv_label_set_text(humidity_label, "Humidity: --%");
    lv_label_set_text(pressure_label, "Pressure: -- hPa");
    lv_label_set_text(wind_label, "Wind: -- km/h");
    lv_label_set_text(status_label, "No weather data");
    lv_label_set_text(last_update_label, "Error fetching data");
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
