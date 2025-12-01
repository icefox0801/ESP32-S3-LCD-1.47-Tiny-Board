#ifndef UI_WEATHER_H
#define UI_WEATHER_H

// Third-party libraries
#include <lvgl.h>

// Project headers
#include "../weather/weather_api.h"
#include "weather_icons.h"

class WeatherUI
{
private:
  lv_obj_t *weather_screen;
  lv_obj_t *weather_container;
  lv_obj_t *main_card;
  lv_obj_t *info_card;
  lv_obj_t *title_label;
  lv_obj_t *weather_icon_img;
  lv_obj_t *weather_icon_canvas;
  lv_color_t *icon_canvas_buf;
  lv_obj_t *temperature_label;
  lv_obj_t *humidity_icon_img;
  lv_obj_t *humidity_info_label;
  lv_obj_t *humidity_unit_label;
  lv_obj_t *aqi_icon_img;
  lv_obj_t *aqi_info_label;
  lv_obj_t *aqi_unit_label;
  lv_obj_t *temp_low_label;
  lv_obj_t *refresh_time_label;
  lv_obj_t *card_title_label; // Weather status in upper card (horizontal mode)

  WeatherAPI *weather_api;

  // Private helper methods for UI creation
  void createScreenBase();
  void createTitleLabel();
  void createUpperCard();
  void createLowerCard();

  // Private helper methods for display updates
  void updateTemperatureDisplay(const WeatherData &weather);
  void updateHumidityDisplay(const WeatherData &weather);
  void updateAirQualityDisplay();
  void updateTimestampDisplay();
  void formatTimestamp(char *buffer, size_t buffer_size, time_t timestamp);
  bool isDaytime() const;

public:
  WeatherUI(WeatherAPI *api);

  // Create weather screen
  void createWeatherScreen();

  // Update weather display
  void updateWeatherDisplay();

  // Show weather screen
  void showWeatherScreen();

  // Get weather screen object
  lv_obj_t *getWeatherScreen();
};

#endif // UI_WEATHER_H
