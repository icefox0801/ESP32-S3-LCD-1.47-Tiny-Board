#ifndef UI_MAIN_H
#define UI_MAIN_H

#include <lvgl.h>

// UI Function declarations
void ui_create_main_screen();
void ui_create_settings_screen();
void ui_create_status_bar();
void ui_create_button_panel();
void ui_create_slider_demo();
void ui_create_chart_demo();

// Event handlers
void button_event_handler(lv_event_t *e);
void slider_event_handler(lv_event_t *e);

// Screen management
void ui_switch_to_screen(lv_obj_t *screen);

// Global UI objects (extern declarations)
extern lv_obj_t *main_screen;
extern lv_obj_t *settings_screen;
extern lv_obj_t *status_bar;

#endif // UI_MAIN_H
