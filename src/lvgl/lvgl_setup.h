#ifndef LVGL_SETUP_H
#define LVGL_SETUP_H

// System libraries
#include <Arduino.h>
#include <SPI.h>

// Third-party libraries
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <lvgl.h>

// ESP32-S3-LCD-1.47-Tiny-Board display pins
#define TFT_MOSI 45
#define TFT_SCLK 40
#define TFT_CS 42             // Chip select control pin
#define TFT_DC 41             // Data Command control pin
#define TFT_RST 39            // Reset pin
#define TFT_BL 48             // LED back-light control pin
#define TFT_BACKLIGHT_ON HIGH // Level to turn ON back-light (HIGH or LOW)
#define TFT_BACKLIGHT_PWM 180 // PWM value for backlight (0-255, 180 = ~70% brightness)

// Display dimensions for ESP32-S3-LCD-1.47-Tiny-Board
#define SCREEN_WIDTH 172
#define SCREEN_HEIGHT 320

// SPI settings
#define SPI_FREQUENCY 80000000 // 80MHz SPI speed (high performance)

// LVGL buffer settings
#define LVGL_BUFFER_LINES 15 // Buffer for 15 lines (optimized for 172px width)
#define LVGL_BUFFER_SIZE (SCREEN_WIDTH * LVGL_BUFFER_LINES)

// External references
extern Adafruit_ST7789 tft;
extern lv_display_t *disp;

// Function declarations
void lvgl_setup_display();
void lvgl_setup_spi();
void lvgl_setup_backlight();
void lvgl_init_display();
void lvgl_create_demo_ui();
void my_disp_flush(lv_display_t *disp_drv, const lv_area_t *area, uint8_t *px_map);

// Main setup function
void lvgl_setup();

#endif // LVGL_SETUP_H
