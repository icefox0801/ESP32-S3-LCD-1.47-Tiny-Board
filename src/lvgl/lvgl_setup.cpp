#include "lvgl_setup.h"

// Global objects
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);
lv_display_t *disp = nullptr;
static lv_color_t buf[LVGL_BUFFER_SIZE];

// Display flush function for LVGL v9
void my_disp_flush(lv_display_t *disp_drv, const lv_area_t *area, uint8_t *px_map)
{
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  tft.writePixels((uint16_t *)px_map, w * h);
  tft.endWrite();

  lv_display_flush_ready(disp_drv);
}

void lvgl_setup_backlight()
{
  Serial.println("Initializing backlight...");
  pinMode(TFT_BL, OUTPUT);
  analogWrite(TFT_BL, TFT_BACKLIGHT_PWM);
  Serial.println("✓ Backlight set to " + String((TFT_BACKLIGHT_PWM * 100) / 255) + "% brightness");
}

void lvgl_setup_spi()
{
  Serial.println("Initializing SPI...");
  SPI.begin(TFT_SCLK, -1, TFT_MOSI, TFT_CS);
  SPI.setFrequency(SPI_FREQUENCY);
  Serial.println("✓ SPI initialized at " + String(SPI_FREQUENCY / 1000000) + "MHz");
}

void lvgl_setup_display()
{
  Serial.println("Initializing ST7789 display...");
  tft.init(SCREEN_WIDTH, SCREEN_HEIGHT);
  tft.setRotation(0); // Portrait mode
  Serial.println("✓ ST7789 display initialized (" + String(SCREEN_WIDTH) + "x" + String(SCREEN_HEIGHT) + ")");
}

void lvgl_init_display()
{
  Serial.println("Initializing LVGL...");

  // Initialize LVGL
  lv_init();

  // Create display with LVGL v9 API
  disp = lv_display_create(SCREEN_WIDTH, SCREEN_HEIGHT);
  lv_display_set_flush_cb(disp, my_disp_flush);
  lv_display_set_buffers(disp, buf, NULL, sizeof(buf), LV_DISPLAY_RENDER_MODE_PARTIAL);

  Serial.println("✓ LVGL display driver initialized");
}

void lvgl_create_demo_ui()
{
  Serial.println("Creating demo UI...");

  // Create a simple label
  lv_obj_t *label = lv_label_create(lv_screen_active());
  lv_label_set_text(label, "ESP32-S3-Box\nLVGL v9.3.0\nDisplay: 240x320\n\nSystem Ready!");
  lv_obj_center(label);

  // Set label style for better visibility
  lv_obj_set_style_text_color(label, lv_color_white(), 0);
  lv_obj_set_style_bg_color(label, lv_color_make(0x20, 0x20, 0x40), 0);
  lv_obj_set_style_bg_opa(label, LV_OPA_COVER, 0);
  lv_obj_set_style_pad_all(label, 15, 0);
  lv_obj_set_style_radius(label, 10, 0);
  lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0);

  // Create a button
  lv_obj_t *btn = lv_button_create(lv_screen_active());
  lv_obj_set_size(btn, 120, 50);
  lv_obj_align(btn, LV_ALIGN_BOTTOM_MID, 0, -30);

  lv_obj_t *btn_label = lv_label_create(btn);
  lv_label_set_text(btn_label, "Click Me!");
  lv_obj_center(btn_label);

  // Force a display refresh
  lv_refr_now(disp);

  Serial.println("✓ Demo UI created");
}

void lvgl_setup()
{
  Serial.println("==========================");
  Serial.println("ESP32-S3 LVGL Setup v1.0");
  Serial.println("==========================");

  // Setup sequence
  lvgl_setup_backlight();
  lvgl_setup_spi();
  lvgl_setup_display();
  lvgl_init_display();
  lvgl_create_demo_ui();

  // Print configuration summary
  Serial.println("\n==========================");
  Serial.println("LVGL Setup Complete!");
  Serial.println("==========================");
  Serial.println("Display: " + String(SCREEN_WIDTH) + "x" + String(SCREEN_HEIGHT));
  Serial.println("SPI Pins:");
  Serial.println("  MOSI: " + String(TFT_MOSI));
  Serial.println("  SCLK: " + String(TFT_SCLK));
  Serial.println("  CS:   " + String(TFT_CS));
  Serial.println("Control Pins:");
  Serial.println("  DC:   " + String(TFT_DC));
  Serial.println("  RST:  " + String(TFT_RST));
  Serial.println("  BL:   " + String(TFT_BL));
  Serial.println("Buffer: " + String(LVGL_BUFFER_SIZE * 2) + " bytes (" + String(LVGL_BUFFER_LINES) + " lines)");
  Serial.println("==========================\n");
}
