// Own header
#include "ui_weather.h"
#include "label_icons.h"
#include "../debug.h"
#include "../config.h"
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
  temp_low_label = nullptr;
  refresh_time_label = nullptr;
  card_title_label = nullptr;
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
#if DISPLAY_HORIZONTAL
  lv_obj_set_style_text_font(refresh_time_label, &lv_font_montserrat_14, LV_PART_MAIN);
  lv_obj_set_style_text_color(refresh_time_label, lv_color_hex(0x888888), LV_PART_MAIN);
  lv_obj_align(refresh_time_label, LV_ALIGN_BOTTOM_MID, 0, -3);
#else
  lv_obj_set_style_text_font(refresh_time_label, &lv_font_montserrat_16, LV_PART_MAIN);
  lv_obj_set_style_text_color(refresh_time_label, lv_color_hex(0x888888), LV_PART_MAIN);
  lv_obj_align(refresh_time_label, LV_ALIGN_BOTTOM_MID, 0, -5);
#endif
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
#if DISPLAY_HORIZONTAL
  // In horizontal mode, title is inside the upper card - hide this one
  lv_obj_add_flag(title_label, LV_OBJ_FLAG_HIDDEN);
#else
  lv_obj_set_style_text_font(title_label, &lv_font_montserrat_16, LV_PART_MAIN);
  lv_obj_set_style_text_color(title_label, lv_color_hex(0x1976d2), LV_PART_MAIN);
  lv_obj_align(title_label, LV_ALIGN_TOP_MID, 0, 10);
#endif
}

void WeatherUI::createUpperCard()
{
  // === UPPER CARD: Weather & Temperature ===
  main_card = lv_obj_create(weather_container);
#if DISPLAY_HORIZONTAL
  // Horizontal: Full width upper card starting from top
  lv_obj_set_size(main_card, LV_HOR_RES - 10, 95);
  lv_obj_align(main_card, LV_ALIGN_TOP_MID, 0, 5);
#else
  // Vertical: Full width upper card
  lv_obj_set_size(main_card, LV_HOR_RES - 20, 165);
  lv_obj_align(main_card, LV_ALIGN_TOP_MID, 0, 35);
#endif
  lv_obj_set_style_bg_color(main_card, lv_color_hex(0x29006b), LV_PART_MAIN); // Match icon background
  lv_obj_set_style_border_width(main_card, 0, LV_PART_MAIN);
  lv_obj_set_style_radius(main_card, 15, LV_PART_MAIN);
  lv_obj_set_style_pad_all(main_card, 8, LV_PART_MAIN);
  lv_obj_set_style_shadow_width(main_card, 10, LV_PART_MAIN);
  lv_obj_set_style_shadow_color(main_card, lv_color_hex(0x000000), LV_PART_MAIN);
  lv_obj_set_style_shadow_opa(main_card, LV_OPA_30, LV_PART_MAIN);
  lv_obj_clear_flag(main_card, LV_OBJ_FLAG_SCROLLABLE);

#if DISPLAY_HORIZONTAL
  // Weather status label inside upper card (horizontal only)
  card_title_label = lv_label_create(main_card);
  lv_label_set_text(card_title_label, "Weather");
  lv_obj_set_style_text_font(card_title_label, &lv_font_montserrat_16, LV_PART_MAIN);
  lv_obj_set_style_text_color(card_title_label, lv_color_hex(0xe3f2fd), LV_PART_MAIN);
  lv_obj_align(card_title_label, LV_ALIGN_TOP_LEFT, 5, 2);
#endif

  // Weather icon - top of upper card (vertical) or left side (horizontal)
  weather_icon_img = lv_obj_create(main_card);
#if DISPLAY_HORIZONTAL
  lv_obj_set_size(weather_icon_img, 64, 64);
  lv_obj_set_style_bg_opa(weather_icon_img, LV_OPA_TRANSP, 0);
  lv_obj_set_style_border_width(weather_icon_img, 0, 0);
  lv_obj_set_style_pad_all(weather_icon_img, 0, 0);
  lv_obj_clear_flag(weather_icon_img, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_align(weather_icon_img, LV_ALIGN_LEFT_MID, 5, 8);
#else
  lv_obj_set_size(weather_icon_img, 64, 64);
  lv_obj_set_style_bg_opa(weather_icon_img, LV_OPA_TRANSP, 0);
  lv_obj_set_style_border_width(weather_icon_img, 0, 0);
  lv_obj_set_style_pad_all(weather_icon_img, 0, 0);
  lv_obj_align(weather_icon_img, LV_ALIGN_TOP_MID, 0, 0);
#endif

  // Large temperature - center of upper card (vertical) or right side (horizontal)
  temperature_label = lv_label_create(main_card);
  lv_label_set_text(temperature_label, "--°");
#if DISPLAY_HORIZONTAL
  lv_obj_set_style_text_font(temperature_label, &lv_font_montserrat_48, LV_PART_MAIN);
  lv_obj_set_style_text_color(temperature_label, lv_color_hex(0xffffff), LV_PART_MAIN);
  lv_obj_align(temperature_label, LV_ALIGN_CENTER, 10, 5);
#else
  lv_obj_set_style_text_font(temperature_label, &lv_font_montserrat_48, LV_PART_MAIN);
  lv_obj_set_style_text_color(temperature_label, lv_color_hex(0xffffff), LV_PART_MAIN);
  lv_obj_align(temperature_label, LV_ALIGN_CENTER, 0, 15);
#endif

  // Low/High temperature range - bottom center of upper card
  temp_low_label = lv_label_create(main_card);
  lv_label_set_text(temp_low_label, "-- - --°");
#if DISPLAY_HORIZONTAL
  lv_obj_set_style_text_font(temp_low_label, &lv_font_montserrat_24, LV_PART_MAIN);
  lv_obj_set_style_text_color(temp_low_label, lv_color_hex(0xb39ddb), LV_PART_MAIN); // Dimmed purple
  lv_obj_align(temp_low_label, LV_ALIGN_BOTTOM_RIGHT, -10, -5);
#else
  lv_obj_set_style_text_font(temp_low_label, &lv_font_montserrat_24, LV_PART_MAIN);
  lv_obj_set_style_text_color(temp_low_label, lv_color_hex(0xe3f2fd), LV_PART_MAIN);
  lv_obj_align(temp_low_label, LV_ALIGN_BOTTOM_MID, 0, -8);
#endif
}

void WeatherUI::createLowerCard()
{
  // === LOWER CARD: Air Quality & Humidity Info ===
  info_card = lv_obj_create(weather_container);
#if DISPLAY_HORIZONTAL
  // Horizontal: Full width lower card below upper card
  lv_obj_set_size(info_card, LV_HOR_RES - 10, 38);
  lv_obj_align(info_card, LV_ALIGN_TOP_MID, 0, 104);
#else
  // Vertical: Full width lower card
  lv_obj_set_size(info_card, LV_HOR_RES - 20, 80);
  lv_obj_align(info_card, LV_ALIGN_TOP_MID, 0, 210);
#endif
  lv_obj_set_style_bg_color(info_card, lv_color_hex(0x004d40), LV_PART_MAIN); // Dark teal-green
  lv_obj_set_style_border_width(info_card, 0, LV_PART_MAIN);
  lv_obj_set_style_radius(info_card, 15, LV_PART_MAIN);
#if DISPLAY_HORIZONTAL
  lv_obj_set_style_pad_top(info_card, 6, LV_PART_MAIN);
  lv_obj_set_style_pad_left(info_card, 15, LV_PART_MAIN);
  lv_obj_set_style_pad_right(info_card, 15, LV_PART_MAIN);
  lv_obj_set_style_pad_bottom(info_card, 6, LV_PART_MAIN);
#else
  lv_obj_set_style_pad_top(info_card, 8, LV_PART_MAIN);
  lv_obj_set_style_pad_left(info_card, 15, LV_PART_MAIN);
  lv_obj_set_style_pad_right(info_card, 15, LV_PART_MAIN);
  lv_obj_set_style_pad_bottom(info_card, 8, LV_PART_MAIN);
#endif
  lv_obj_set_style_shadow_width(info_card, 10, LV_PART_MAIN);
  lv_obj_set_style_shadow_color(info_card, lv_color_hex(0x000000), LV_PART_MAIN);
  lv_obj_set_style_shadow_opa(info_card, LV_OPA_30, LV_PART_MAIN);
  lv_obj_clear_flag(info_card, LV_OBJ_FLAG_SCROLLABLE);

#if DISPLAY_HORIZONTAL
  // === HORIZONTAL MODE: Simple text labels ===
  // PM2.5 label on the left: "PM2.5: xxx"
  aqi_icon_img = lv_label_create(info_card); // Reuse as combined label
  lv_label_set_text(aqi_icon_img, "PM2.5:");
  lv_obj_set_style_text_font(aqi_icon_img, &lv_font_montserrat_16, LV_PART_MAIN);
  lv_obj_set_style_text_color(aqi_icon_img, lv_color_hex(0xe8f5e9), LV_PART_MAIN);
  lv_obj_align(aqi_icon_img, LV_ALIGN_LEFT_MID, 0, 0);

  // AQI value
  aqi_info_label = lv_label_create(info_card);
  lv_label_set_text(aqi_info_label, "--");
  lv_obj_set_style_text_font(aqi_info_label, &lv_font_montserrat_24, LV_PART_MAIN);
  lv_obj_set_style_text_color(aqi_info_label, lv_color_hex(0xffffff), LV_PART_MAIN);
  lv_obj_align(aqi_info_label, LV_ALIGN_LEFT_MID, 65, 0);

  // Hide unused labels in horizontal mode
  aqi_unit_label = lv_label_create(info_card);
  lv_obj_add_flag(aqi_unit_label, LV_OBJ_FLAG_HIDDEN);

  // Humidity label on the right: "Humidity: xx%"
  humidity_icon_img = lv_label_create(info_card); // Reuse as combined label
  lv_label_set_text(humidity_icon_img, "Humidity:");
  lv_obj_set_style_text_font(humidity_icon_img, &lv_font_montserrat_16, LV_PART_MAIN);
  lv_obj_set_style_text_color(humidity_icon_img, lv_color_hex(0xe8f5e9), LV_PART_MAIN);
  lv_obj_align(humidity_icon_img, LV_ALIGN_CENTER, 20, 0);

  // Humidity value with %
  humidity_info_label = lv_label_create(info_card);
  lv_label_set_text(humidity_info_label, "--%");
  lv_obj_set_style_text_font(humidity_info_label, &lv_font_montserrat_24, LV_PART_MAIN);
  lv_obj_set_style_text_color(humidity_info_label, lv_color_hex(0xffffff), LV_PART_MAIN);
  lv_obj_align(humidity_info_label, LV_ALIGN_RIGHT_MID, 0, 0);

  // Hide unused labels in horizontal mode
  humidity_unit_label = lv_label_create(info_card);
  lv_obj_add_flag(humidity_unit_label, LV_OBJ_FLAG_HIDDEN);

#else
  // === VERTICAL MODE: Original layout with icons ===
  // AQI icon (using LVGL symbol)
  aqi_icon_img = lv_label_create(info_card);
  lv_label_set_text(aqi_icon_img, AIR_QUALITY_ICON_SYMBOL);
  lv_obj_set_style_text_color(aqi_icon_img, lv_color_hex(0xffffff), LV_PART_MAIN);
  lv_obj_set_style_text_font(aqi_icon_img, &lv_font_montserrat_16, LV_PART_MAIN);
  lv_obj_align(aqi_icon_img, LV_ALIGN_TOP_LEFT, 5, 10);

  // AQI unit label (smaller)
  aqi_unit_label = lv_label_create(info_card);
  lv_label_set_text(aqi_unit_label, "PM2.5");
  lv_obj_set_style_text_font(aqi_unit_label, &lv_font_montserrat_14, LV_PART_MAIN);
  lv_obj_set_style_text_color(aqi_unit_label, lv_color_hex(0xe8f5e9), LV_PART_MAIN);
  lv_obj_align_to(aqi_unit_label, aqi_icon_img, LV_ALIGN_BOTTOM_LEFT, 80, 4);

  // AQI value in lower card
  aqi_info_label = lv_label_create(info_card);
  lv_label_set_text(aqi_info_label, "--");
  lv_obj_set_style_text_font(aqi_info_label, &lv_font_montserrat_26, LV_PART_MAIN);
  lv_obj_set_style_text_color(aqi_info_label, lv_color_hex(0xffffff), LV_PART_MAIN);
  lv_obj_set_style_text_align(aqi_info_label, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN);
  lv_obj_align(aqi_info_label, LV_ALIGN_TOP_RIGHT, -56, 4);

  // Humidity icon (using LVGL symbol)
  humidity_icon_img = lv_label_create(info_card);
  lv_label_set_text(humidity_icon_img, HUMIDITY_ICON_SYMBOL);
  lv_obj_set_style_text_color(humidity_icon_img, lv_color_hex(0xffffff), LV_PART_MAIN);
  lv_obj_set_style_text_font(humidity_icon_img, &lv_font_montserrat_16, LV_PART_MAIN);
  lv_obj_align(humidity_icon_img, LV_ALIGN_TOP_LEFT, 5, 44);

  // Humidity unit label (smaller, right-aligned)
  humidity_unit_label = lv_label_create(info_card);
  lv_label_set_text(humidity_unit_label, "%");
  lv_obj_set_style_text_font(humidity_unit_label, &lv_font_montserrat_16, LV_PART_MAIN);
  lv_obj_set_style_text_color(humidity_unit_label, lv_color_hex(0xe8f5e9), LV_PART_MAIN);
  lv_obj_align_to(humidity_unit_label, humidity_icon_img, LV_ALIGN_BOTTOM_LEFT, 80, 4);

  // Humidity value in lower card
  humidity_info_label = lv_label_create(info_card);
  lv_label_set_text(humidity_info_label, "--");
  lv_obj_set_style_text_font(humidity_info_label, &lv_font_montserrat_26, LV_PART_MAIN);
  lv_obj_set_style_text_color(humidity_info_label, lv_color_hex(0xffffff), LV_PART_MAIN);
  lv_obj_set_style_text_align(humidity_info_label, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN);
  lv_obj_align(humidity_info_label, LV_ALIGN_TOP_RIGHT, -56, 36);
#endif
}

void WeatherUI::updateWeatherDisplay()
{
  DEBUG_LOG("Updating weather display...");

  if (!weather_api || !weather_container)
  {
    LOG_ERROR("weather_api or weather_container is NULL!");
    return;
  }

  WeatherData weather = weather_api->getCurrentWeather();

  if (weather.valid)
  {
    // Update weather state title
    const char *stateName = WeatherIcons::getConditionDisplayName(weather.condition_code);
    lv_label_set_text(title_label, stateName);
#if DISPLAY_HORIZONTAL
    // Also update card title in horizontal mode
    if (card_title_label)
    {
      lv_label_set_text(card_title_label, stateName);
    }
#endif

    // Update weather icon with day/night variant
    if (weather_icon_img)
    {
      WeatherIcons::updateWeatherIcon(weather_icon_img, weather.condition_code, isDaytime());
    }

    // Update all display sections
    updateTemperatureDisplay(weather);
    updateHumidityDisplay(weather);
    updateAirQualityDisplay();
    updateTimestampDisplay();
  }
  else
  {
    // Show error state
    lv_label_set_text(title_label, "Weather");
#if DISPLAY_HORIZONTAL
    if (card_title_label)
    {
      lv_label_set_text(card_title_label, "Weather");
    }
    lv_label_set_text(humidity_info_label, "--%");
#else
    lv_label_set_text(humidity_info_label, "--");
#endif
    lv_label_set_text(temperature_label, "--°");
    lv_label_set_text(aqi_info_label, "--");
    lv_label_set_text(refresh_time_label, LV_SYMBOL_LOOP "  --");
  }
}

// Helper method: Check if current time is daytime (6 AM - 6 PM)
bool WeatherUI::isDaytime() const
{
  time_t now;
  time(&now);
  struct tm timeinfo;
  localtime_r(&now, &timeinfo);
  return (timeinfo.tm_hour >= 6 && timeinfo.tm_hour < 18);
}

// Helper method: Update temperature display
void WeatherUI::updateTemperatureDisplay(const WeatherData &weather)
{
  // Update current temperature (rounded)
  String temp_str = String((int)round(weather.temperature)) + "°";
  lv_label_set_text(temperature_label, temp_str.c_str());

  // Update low/high temperature range in "X - Y°" format (rounded)
  String temp_range = String((int)round(weather.temp_low)) + " - " +
                      String((int)round(weather.temp_high)) + "°";
  lv_label_set_text(temp_low_label, temp_range.c_str());
}

// Helper method: Update humidity display
void WeatherUI::updateHumidityDisplay(const WeatherData &weather)
{
#if DISPLAY_HORIZONTAL
  // Horizontal mode: include % in the value
  String humidity_str = String(weather.humidity) + "%";
#else
  String humidity_str = String(weather.humidity);
#endif
  lv_label_set_text(humidity_info_label, humidity_str.c_str());
}

// Helper method: Update air quality display
void WeatherUI::updateAirQualityDisplay()
{
  String aqi_str = weather_api->getAirQualityString();
  lv_label_set_text(aqi_info_label, aqi_str.c_str());
}

// Helper method: Update timestamp display
void WeatherUI::updateTimestampDisplay()
{
  time_t fetch_time = weather_api->getLastUpdateTime();
  DEBUG_LOGF("Fetch time: %lu\n", (unsigned long)fetch_time);

  static char time_buf[32];
  formatTimestamp(time_buf, sizeof(time_buf), fetch_time);

  lv_label_set_text(refresh_time_label, time_buf);

  // Force LVGL to refresh the label
  lv_obj_invalidate(refresh_time_label);
  delay(10);
  lv_refr_now(NULL);
}

// Helper method: Format timestamp as "HH:MM Mon DD"
void WeatherUI::formatTimestamp(char *buffer, size_t buffer_size, time_t timestamp)
{
  struct tm timeinfo;
  localtime_r(&timestamp, &timeinfo);

  static const char *months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
                                 "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

  snprintf(buffer, buffer_size, LV_SYMBOL_LOOP "  %02d:%02d %s %d",
           timeinfo.tm_hour, timeinfo.tm_min,
           months[timeinfo.tm_mon], timeinfo.tm_mday);
}

void WeatherUI::showWeatherScreen()
{
  if (weather_screen)
  {
    lv_scr_load(weather_screen);
  }
}

lv_obj_t *WeatherUI::getWeatherScreen()
{
  return weather_screen;
}
