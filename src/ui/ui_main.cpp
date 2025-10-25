// System libraries
#include <Arduino.h>

// Own header
#include "ui_main.h"

// Global UI objects
lv_obj_t *main_screen = nullptr;
lv_obj_t *settings_screen = nullptr;
lv_obj_t *status_bar = nullptr;

void ui_switch_to_screen(lv_obj_t *screen)
{
  if (screen != nullptr)
  {
    lv_screen_load(screen);
  }
}

void ui_create_status_bar()
{
  status_bar = lv_obj_create(lv_screen_active());
  lv_obj_set_size(status_bar, LV_PCT(100), 30);
  lv_obj_align(status_bar, LV_ALIGN_TOP_MID, 0, 0);
  lv_obj_set_style_bg_color(status_bar, lv_color_make(0x10, 0x10, 0x20), 0);

  lv_obj_t *status_label = lv_label_create(status_bar);
  lv_label_set_text(status_label, "ESP32-S3-Box | Ready");
  lv_obj_center(status_label);
  lv_obj_set_style_text_color(status_label, lv_color_white(), 0);
}

void ui_create_main_screen()
{
  main_screen = lv_obj_create(NULL);
  lv_obj_t *old_screen = lv_screen_active();
  lv_screen_load(main_screen);

  ui_create_status_bar();

  Serial.println("✓ Main screen created");
}

void ui_create_settings_screen()
{
  settings_screen = lv_obj_create(NULL);

  // This would contain settings widgets
  // Implementation can be added later

  Serial.println("✓ Settings screen created");
}
