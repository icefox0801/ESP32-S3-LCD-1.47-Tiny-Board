// Own header
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
  pinMode(TFT_BL, OUTPUT);
  analogWrite(TFT_BL, TFT_BACKLIGHT_PWM);
}

void lvgl_setup_spi()
{
  SPI.begin(TFT_SCLK, -1, TFT_MOSI, TFT_CS);
  SPI.setFrequency(SPI_FREQUENCY);
}

void lvgl_setup_display()
{
  tft.init(SCREEN_WIDTH, SCREEN_HEIGHT);
  tft.setRotation(0);
}

void lvgl_init_display()
{
  lv_init();
  disp = lv_display_create(SCREEN_WIDTH, SCREEN_HEIGHT);
  lv_display_set_flush_cb(disp, my_disp_flush);
  lv_display_set_buffers(disp, buf, NULL, sizeof(buf), LV_DISPLAY_RENDER_MODE_PARTIAL);
}

void lvgl_setup()
{
  lvgl_setup_backlight();
  lvgl_setup_spi();
  lvgl_setup_display();
  lvgl_init_display();
}
