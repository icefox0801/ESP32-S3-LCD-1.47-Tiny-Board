#include <Arduino.h>
#include "lvgl_ui.h"

// Global UI objects
lv_obj_t *main_screen = nullptr;
lv_obj_t *settings_screen = nullptr;
lv_obj_t *status_bar = nullptr;

// Event handlers
void button_event_handler(lv_event_t *e)
{
  lv_event_code_t code = lv_event_get_code(e);
  if (code == LV_EVENT_CLICKED)
  {
    Serial.println("Button clicked!");

    // Create a temporary message box
    lv_obj_t *mbox = lv_msgbox_create(lv_screen_active());
    lv_msgbox_add_title(mbox, "Button Pressed");
    lv_msgbox_add_text(mbox, "Hello from ESP32-S3!");
    lv_msgbox_add_footer_button(mbox, "OK");
    lv_obj_center(mbox);
  }
}

void slider_event_handler(lv_event_t *e)
{
  lv_obj_t *slider = (lv_obj_t *)lv_event_get_target(e);
  int32_t value = lv_slider_get_value(slider);
  Serial.println("Slider value: " + String(value));
}

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

void ui_create_button_panel()
{
  lv_obj_t *panel = lv_obj_create(lv_screen_active());
  lv_obj_set_size(panel, LV_PCT(90), 100);
  lv_obj_align(panel, LV_ALIGN_CENTER, 0, -50);

  // Create multiple buttons
  const char *btn_texts[] = {"Start", "Stop", "Reset", "Settings"};
  lv_color_t btn_colors[] = {
      lv_color_make(0x00, 0x80, 0x00), // Green
      lv_color_make(0x80, 0x00, 0x00), // Red
      lv_color_make(0x80, 0x80, 0x00), // Yellow
      lv_color_make(0x00, 0x00, 0x80)  // Blue
  };

  for (int i = 0; i < 4; i++)
  {
    lv_obj_t *btn = lv_button_create(panel);
    lv_obj_set_size(btn, 80, 40);
    lv_obj_align(btn, LV_ALIGN_TOP_LEFT, 10 + (i % 2) * 90, 10 + (i / 2) * 50);
    lv_obj_set_style_bg_color(btn, btn_colors[i], 0);
    lv_obj_add_event_cb(btn, button_event_handler, LV_EVENT_CLICKED, NULL);

    lv_obj_t *label = lv_label_create(btn);
    lv_label_set_text(label, btn_texts[i]);
    lv_obj_center(label);
    lv_obj_set_style_text_color(label, lv_color_white(), 0);
  }
}

void ui_create_slider_demo()
{
  lv_obj_t *slider = lv_slider_create(lv_screen_active());
  lv_obj_set_size(slider, 200, 20);
  lv_obj_align(slider, LV_ALIGN_CENTER, 0, 50);
  lv_slider_set_range(slider, 0, 100);
  lv_slider_set_value(slider, 50, LV_ANIM_OFF);
  lv_obj_add_event_cb(slider, slider_event_handler, LV_EVENT_VALUE_CHANGED, NULL);

  lv_obj_t *slider_label = lv_label_create(lv_screen_active());
  lv_label_set_text(slider_label, "Brightness: 50%");
  lv_obj_align_to(slider_label, slider, LV_ALIGN_OUT_TOP_MID, 0, -10);
}

void ui_create_chart_demo()
{
  lv_obj_t *chart = lv_chart_create(lv_screen_active());
  lv_obj_set_size(chart, 180, 100);
  lv_obj_align(chart, LV_ALIGN_BOTTOM_MID, 0, -10);
  lv_chart_set_type(chart, LV_CHART_TYPE_LINE);

  lv_chart_series_t *series = lv_chart_add_series(chart, lv_color_make(0xFF, 0x00, 0x00), LV_CHART_AXIS_PRIMARY_Y);

  // Add some sample data
  lv_chart_set_point_count(chart, 10);
  lv_chart_set_next_value(chart, series, 10);
  lv_chart_set_next_value(chart, series, 20);
  lv_chart_set_next_value(chart, series, 35);
  lv_chart_set_next_value(chart, series, 50);
  lv_chart_set_next_value(chart, series, 75);
  lv_chart_set_next_value(chart, series, 60);
  lv_chart_set_next_value(chart, series, 45);
  lv_chart_set_next_value(chart, series, 30);
  lv_chart_set_next_value(chart, series, 15);
  lv_chart_set_next_value(chart, series, 25);
}

void ui_create_main_screen()
{
  main_screen = lv_obj_create(NULL);
  lv_obj_t *old_screen = lv_screen_active();
  lv_screen_load(main_screen);

  ui_create_status_bar();
  ui_create_button_panel();
  ui_create_slider_demo();

  Serial.println("✓ Main screen created");
}

void ui_create_settings_screen()
{
  settings_screen = lv_obj_create(NULL);

  // This would contain settings widgets
  // Implementation can be added later

  Serial.println("✓ Settings screen created");
}
