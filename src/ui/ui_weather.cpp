// Own header
#include "ui_weather.h"
#include <time.h>

WeatherUI::WeatherUI(WeatherAPI *api) : weather_api(api)
{
  weather_screen = nullptr;
  weather_container = nullptr;
  main_card = nullptr;
  info_card = nullptr;
  title_label = nullptr;
  weather_icon_img = nullptr;
  weather_icon_canvas = nullptr;
  icon_canvas_buf = nullptr;
  temperature_label = nullptr;
  humidity_icon_img = nullptr;
  humidity_info_label = nullptr;
  humidity_unit_label = nullptr;
  aqi_icon_img = nullptr;
  aqi_info_label = nullptr;
  aqi_unit_label = nullptr;
  outdoor_label = nullptr;
  temp_low_label = nullptr;
  temp_high_label = nullptr;
  refresh_time_label = nullptr;
  update_timer = nullptr;
}

void WeatherUI::createWeatherScreen()
{
  createScreenBase();
  createTitleLabel();
  createUpperCard();
  createLowerCard();

  // Create refresh timestamp label at bottom (below lower card)
  refresh_time_label = lv_label_create(weather_container);
  lv_label_set_text(refresh_time_label, "Refreshed: --");
  lv_obj_set_style_text_font(refresh_time_label, &lv_font_montserrat_16, LV_PART_MAIN); // Increased from 14 to 16
  lv_obj_set_style_text_color(refresh_time_label, lv_color_hex(0x888888), LV_PART_MAIN);
  lv_obj_align(refresh_time_label, LV_ALIGN_BOTTOM_MID, 0, -6); // Lowered by 2pt more for optimal spacing
}

void WeatherUI::createScreenBase()
{
  // Create main weather screen with white background
  weather_screen = lv_obj_create(NULL);
  lv_obj_set_size(weather_screen, LV_HOR_RES, LV_VER_RES);
  lv_obj_set_style_bg_color(weather_screen, lv_color_hex(0xffffff), LV_PART_MAIN);

  // Create main container
  weather_container = lv_obj_create(weather_screen);
  lv_obj_set_size(weather_container, LV_HOR_RES, LV_VER_RES);
  lv_obj_center(weather_container);
  lv_obj_set_style_bg_color(weather_container, lv_color_hex(0xffffff), LV_PART_MAIN);
  lv_obj_set_style_border_width(weather_container, 0, LV_PART_MAIN);
  lv_obj_set_style_radius(weather_container, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(weather_container, 0, LV_PART_MAIN);
  lv_obj_clear_flag(weather_container, LV_OBJ_FLAG_SCROLLABLE);
}

void WeatherUI::createTitleLabel()
{
  // Weather state label at top (will be updated with actual state)
  title_label = lv_label_create(weather_container);
  lv_label_set_text(title_label, "Weather");
  lv_obj_set_style_text_font(title_label, &lv_font_montserrat_16, LV_PART_MAIN);  // Increased from 14 to 16
  lv_obj_set_style_text_color(title_label, lv_color_hex(0x1976d2), LV_PART_MAIN); // Darker blue for clarity
  lv_obj_align(title_label, LV_ALIGN_TOP_MID, 0, 10);                             // Reduced from 15 to 10
}

void WeatherUI::createUpperCard()
{
  // === UPPER CARD: Weather & Temperature ===
  main_card = lv_obj_create(weather_container);
  lv_obj_set_size(main_card, LV_HOR_RES - 20, 165);
  lv_obj_align(main_card, LV_ALIGN_TOP_MID, 0, 35);                           // Reduced from 40 to 35
  lv_obj_set_style_bg_color(main_card, lv_color_hex(0x29006b), LV_PART_MAIN); // Match icon background (RGB565 0x280d)
  lv_obj_set_style_border_width(main_card, 0, LV_PART_MAIN);
  lv_obj_set_style_radius(main_card, 15, LV_PART_MAIN);
  lv_obj_set_style_pad_all(main_card, 8, LV_PART_MAIN);
  lv_obj_set_style_shadow_width(main_card, 10, LV_PART_MAIN);
  lv_obj_set_style_shadow_color(main_card, lv_color_hex(0x000000), LV_PART_MAIN);
  lv_obj_set_style_shadow_opa(main_card, LV_OPA_30, LV_PART_MAIN);
  lv_obj_clear_flag(main_card, LV_OBJ_FLAG_SCROLLABLE);

  // Weather icon - top of upper card (using emoji)
  // Create container for PNG weather icon
  weather_icon_img = lv_obj_create(main_card);
  lv_obj_set_size(weather_icon_img, 64, 64);
  lv_obj_set_style_bg_opa(weather_icon_img, LV_OPA_TRANSP, 0);
  lv_obj_set_style_border_width(weather_icon_img, 0, 0);
  lv_obj_set_style_pad_all(weather_icon_img, 0, 0);
  lv_obj_align(weather_icon_img, LV_ALIGN_TOP_MID, 0, 0);

  // Large temperature - center of upper card
  temperature_label = lv_label_create(main_card);
  lv_label_set_text(temperature_label, "--°");
  lv_obj_set_style_text_font(temperature_label, &lv_font_montserrat_48, LV_PART_MAIN);
  lv_obj_set_style_text_color(temperature_label, lv_color_hex(0xffffff), LV_PART_MAIN);
  lv_obj_align(temperature_label, LV_ALIGN_CENTER, 0, 15);

  // Low/High temperature range - bottom center of upper card
  temp_low_label = lv_label_create(main_card);
  lv_label_set_text(temp_low_label, "-- - --°");
  lv_obj_set_style_text_font(temp_low_label, &lv_font_montserrat_24, LV_PART_MAIN);
  lv_obj_set_style_text_color(temp_low_label, lv_color_hex(0xe3f2fd), LV_PART_MAIN);
  lv_obj_align(temp_low_label, LV_ALIGN_BOTTOM_MID, 0, -8);

  // High temperature label no longer needed (using temp_low_label for combined display)
  temp_high_label = nullptr;
}

void WeatherUI::createLowerCard()
{
  // === LOWER CARD: Air Quality & Humidity Info ===
  info_card = lv_obj_create(weather_container);
  lv_obj_set_size(info_card, LV_HOR_RES - 20, 78); // Reduced back to 78 to avoid overlap
  lv_obj_align(info_card, LV_ALIGN_TOP_MID, 0, 210);
  lv_obj_set_style_bg_color(info_card, lv_color_hex(0x004d40), LV_PART_MAIN); // Dark teal-green
  lv_obj_set_style_border_width(info_card, 0, LV_PART_MAIN);
  lv_obj_set_style_radius(info_card, 15, LV_PART_MAIN);
  lv_obj_set_style_pad_top(info_card, 8, LV_PART_MAIN);
  lv_obj_set_style_pad_left(info_card, 8, LV_PART_MAIN);
  lv_obj_set_style_pad_right(info_card, 8, LV_PART_MAIN);
  lv_obj_set_style_pad_bottom(info_card, 20, LV_PART_MAIN); // Increased bottom padding from 16 to 20
  lv_obj_set_style_shadow_width(info_card, 10, LV_PART_MAIN);
  lv_obj_set_style_shadow_color(info_card, lv_color_hex(0x000000), LV_PART_MAIN);
  lv_obj_set_style_shadow_opa(info_card, LV_OPA_30, LV_PART_MAIN);
  lv_obj_clear_flag(info_card, LV_OBJ_FLAG_SCROLLABLE);

  // === AIR QUALITY ROW (First line) ===
  // AQI icon (warning symbol for air quality) in lower card
  aqi_icon_img = lv_label_create(info_card);
  lv_label_set_text(aqi_icon_img, LV_SYMBOL_WARNING); // Warning/alert symbol for air quality
  lv_obj_set_style_text_font(aqi_icon_img, &lv_font_montserrat_16, LV_PART_MAIN);
  lv_obj_set_style_text_color(aqi_icon_img, lv_color_hex(0xe8f5e9), LV_PART_MAIN);
  lv_obj_align(aqi_icon_img, LV_ALIGN_TOP_LEFT, 5, 8);

  // AQI unit label (smaller)
  aqi_unit_label = lv_label_create(info_card);
  lv_label_set_text(aqi_unit_label, "PM2.5");
  lv_obj_set_style_text_font(aqi_unit_label, &lv_font_montserrat_14, LV_PART_MAIN);
  lv_obj_set_style_text_color(aqi_unit_label, lv_color_hex(0xe8f5e9), LV_PART_MAIN);
  lv_obj_align_to(aqi_unit_label, aqi_icon_img, LV_ALIGN_BOTTOM_LEFT, 80, 4);

  // AQI value in lower card (anchored to left of unit label)
  aqi_info_label = lv_label_create(info_card);
  lv_label_set_text(aqi_info_label, "--");
  lv_obj_set_style_text_font(aqi_info_label, &lv_font_montserrat_26, LV_PART_MAIN);
  lv_obj_set_style_text_color(aqi_info_label, lv_color_hex(0xffffff), LV_PART_MAIN);
  lv_obj_set_style_text_align(aqi_info_label, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN);
  lv_obj_align(aqi_info_label, LV_ALIGN_TOP_RIGHT, -56, 4);

  // === HUMIDITY ROW (Second line) ===
  // Humidity icon (droplet symbol) in lower card
  humidity_icon_img = lv_label_create(info_card);
  lv_label_set_text(humidity_icon_img, LV_SYMBOL_TINT); // Water droplet
  lv_obj_set_style_text_font(humidity_icon_img, &lv_font_montserrat_16, LV_PART_MAIN);
  lv_obj_set_style_text_color(humidity_icon_img, lv_color_hex(0xe8f5e9), LV_PART_MAIN);
  lv_obj_align(humidity_icon_img, LV_ALIGN_TOP_LEFT, 5, 42);

  // Humidity unit label (smaller, right-aligned) - position first as anchor
  humidity_unit_label = lv_label_create(info_card);
  lv_label_set_text(humidity_unit_label, "%");
  lv_obj_set_style_text_font(humidity_unit_label, &lv_font_montserrat_16, LV_PART_MAIN);
  lv_obj_set_style_text_color(humidity_unit_label, lv_color_hex(0xe8f5e9), LV_PART_MAIN);
  lv_obj_align_to(humidity_unit_label, humidity_icon_img, LV_ALIGN_BOTTOM_LEFT, 80, 4);

  // Humidity value in lower card (anchored to left of unit label)
  humidity_info_label = lv_label_create(info_card);
  lv_label_set_text(humidity_info_label, "--");
  lv_obj_set_style_text_font(humidity_info_label, &lv_font_montserrat_26, LV_PART_MAIN);
  lv_obj_set_style_text_color(humidity_info_label, lv_color_hex(0xffffff), LV_PART_MAIN);
  lv_obj_set_style_text_align(humidity_info_label, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN);
  lv_obj_align(humidity_info_label, LV_ALIGN_TOP_RIGHT, -56, 36);
}

void WeatherUI::updateWeatherDisplay()
{
  if (!weather_api || !weather_container)
    return;

  WeatherData weather = weather_api->getCurrentWeather();

  if (weather.valid)
  {
    // Update weather state title using condition code
    const char *stateName = WeatherIcons::getConditionDisplayName(weather.condition_code);
    lv_label_set_text(title_label, stateName);

    // Determine if it's daytime (simple approach: 6 AM to 6 PM)
    time_t now;
    time(&now);
    struct tm timeinfo;
    localtime_r(&now, &timeinfo);
    bool isDaytime = (timeinfo.tm_hour >= 6 && timeinfo.tm_hour < 18);

    // Update weather icon based on current condition code with day/night variant
    if (weather_icon_img)
    {
      // Update the SVG icon using the condition code
      WeatherIcons::updateWeatherIcon(weather_icon_img, weather.condition_code, isDaytime);
    }

    // Update temperature (rounded)
    String temp_str = String((int)round(weather.temperature)) + "°";
    lv_label_set_text(temperature_label, temp_str.c_str());

    // Update humidity (value only, unit is separate)
    String humidity_str = String(weather.humidity);
    lv_label_set_text(humidity_info_label, humidity_str.c_str());

    // Update air quality PM2.5 value
    String aqi_str = weather_api->getAirQualityString();
    lv_label_set_text(aqi_info_label, aqi_str.c_str());

    // Update low/high temperature range in "X - Y°" format (rounded)
    String temp_range = String((int)round(weather.temp_low)) + " - " + String((int)round(weather.temp_high)) + "°";
    lv_label_set_text(temp_low_label, temp_range.c_str());

    // Update refresh timestamp with loop icon and spacing
    char time_buf[32];
    const char *months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    snprintf(time_buf, sizeof(time_buf), LV_SYMBOL_LOOP "  %02d:%02d %s %d",
             timeinfo.tm_hour, timeinfo.tm_min, months[timeinfo.tm_mon], timeinfo.tm_mday);
    lv_label_set_text(refresh_time_label, time_buf);
  }
  else
  {
    // Show error state
    lv_label_set_text(title_label, "Weather");
    lv_label_set_text(temperature_label, "--°");
    lv_label_set_text(humidity_info_label, "--");
    lv_label_set_text(aqi_info_label, "--");
    lv_label_set_text(refresh_time_label, LV_SYMBOL_LOOP "  --");
  }
}

void WeatherUI::showWeatherScreen()
{
  if (weather_screen)
  {
    lv_scr_load(weather_screen);
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
    update_timer = lv_timer_create(update_timer_cb, 300000, this);
  }
}

void WeatherUI::stopAutoUpdate()
{
  if (update_timer)
  {
    lv_timer_del(update_timer);
    update_timer = nullptr;
  }
}

void WeatherUI::update_timer_cb(lv_timer_t *timer)
{
  WeatherUI *ui = (WeatherUI *)lv_timer_get_user_data(timer);
  ui->handleUpdateTimer();
}

void WeatherUI::handleUpdateTimer()
{
  if (weather_api && weather_api->needsUpdate())
  {
    if (weather_api->fetchWeatherData())
    {
      updateWeatherDisplay();
    }
  }
}
