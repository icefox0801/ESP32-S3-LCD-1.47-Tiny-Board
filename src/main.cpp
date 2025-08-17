#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>
#include <lvgl.h>

#define USER_SETUP_ID 71

#define TFT_BL 48             // LED back-light control pin
#define TFT_BACKLIGHT_ON HIGH // Level to turn ON back-light (HIGH or LOW)

#define TFT_MOSI 45
#define TFT_SCLK 40
#define TFT_CS 42  // Chip select control pin
#define TFT_DC 41  // Data Command control pin
#define TFT_RST 39 // Reset pin (could connect to RST pin)

#define SPI_FREQUENCY 8000000

// Display dimensions for ESP32-S3-Box
#define SCREEN_WIDTH 172
#define SCREEN_HEIGHT 320

#define USE_HSPI_PORT 1

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

// LVGL display buffer
static lv_display_t *disp;
static lv_color_t buf[SCREEN_WIDTH * 10]; // Buffer for 10 lines

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

void setup()
{
  // Initialize USB-Serial communication
  Serial.begin(115200);

  // Wait for USB-Serial to be ready
  delay(2000); // Give time for USB serial to initialize

  Serial.println("==========================");
  Serial.println("ESP32-S3 LVGL Initialization");
  Serial.println("==========================");

  // Initialize backlight control pin
  Serial.println("Initializing backlight...");
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, TFT_BACKLIGHT_ON);
  Serial.println("Backlight turned ON");

  Serial.println("==========================");
  Serial.println("ESP32-S3 LVGL Initialization");
  Serial.println("==========================");

  // Initialize display
  Serial.println("Initializing ST7789 display...");
  tft.init(SCREEN_WIDTH, SCREEN_HEIGHT); // Init ST7789 172x320
  tft.setRotation(0);                    // Portrait mode
  tft.fillScreen(ST77XX_WHITE);

  Serial.println("Initializing LVGL...");

  // Initialize LVGL
  lv_init();

  // Create display with LVGL v9 API
  disp = lv_display_create(SCREEN_WIDTH, SCREEN_HEIGHT);
  lv_display_set_flush_cb(disp, my_disp_flush);
  lv_display_set_buffers(disp, buf, NULL, sizeof(buf), LV_DISPLAY_RENDER_MODE_PARTIAL);

  // Create a simple label
  lv_obj_t *label = lv_label_create(lv_screen_active());
  lv_label_set_text(label, "Hello ESP32-S3!\nLVGL v9 is running!");
  lv_obj_center(label);

  Serial.println("LVGL initialized successfully!");
  Serial.println("Display size: " + String(SCREEN_WIDTH) + "x" + String(SCREEN_HEIGHT));
  Serial.println();

  // put your setup code here, to run once:
  Serial.println();
  Serial.println("Setup complete. Starting main loop...");
  Serial.println();
}

void loop()
{
  // Handle LVGL tasks
  lv_timer_handler();

  // Add a small delay to prevent watchdog timeout
  delay(5);
}
