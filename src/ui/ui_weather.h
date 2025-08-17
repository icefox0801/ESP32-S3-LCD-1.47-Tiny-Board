#ifndef UI_WEATHER_H
#define UI_WEATHER_H

#include "lvgl.h"
#include "misc/lv_timer.h"
#include "../weather/weather_api.h"

class WeatherUI
{
private:
  lv_obj_t *weather_screen;
  lv_obj_t *weather_container;
  lv_obj_t *weather_icon_label;
  lv_obj_t *temperature_label;
  lv_obj_t *humidity_label;
  lv_obj_t *pressure_label;
  lv_obj_t *wind_label;
  lv_obj_t *status_label;
  lv_obj_t *last_update_label;

  WeatherAPI *weather_api;

  // Update task
  lv_timer_t *update_timer;

  // Static callback for timer
  static void update_timer_cb(lv_timer_t *timer);

public:
  WeatherUI(WeatherAPI *api);

  // Create weather screen
  void createWeatherScreen();

  // Update weather display
  void updateWeatherDisplay();

  // Show weather screen
  void showWeatherScreen();

  // Hide weather screen
  void hideWeatherScreen();

  // Get weather screen object
  lv_obj_t *getWeatherScreen();

  // Start automatic updates
  void startAutoUpdate();

  // Stop automatic updates
  void stopAutoUpdate();

  // Handle update timer
  void handleUpdateTimer();
};

#endif // UI_WEATHER_H
