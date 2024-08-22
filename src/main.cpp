#include <TFT_eSPI.h>
#include <lvgl.h>
#include "mconfig.h"
#include <XPT2046_Touchscreen.h>

#define XPT2046_IRQ 36   // T_IRQ
#define XPT2046_MOSI 32  // T_DIN
#define XPT2046_MISO 39  // T_OUT
#define XPT2046_CLK 25   // T_CLK
#define XPT2046_CS 33 
#define MY_DISP_HOR_RES SCREEN_WIDTH
#define MY_DISP_VER_RES SCREEN_HEIGHT

uint32_t lastTouchTime;


SPIClass touchscreenSPI = SPIClass(VSPI);
XPT2046_Touchscreen touchscreen(XPT2046_CS, XPT2046_IRQ);
lv_display_t *display;
lv_obj_t *scr1;
lv_obj_t *scr2;
#define DRAW_BUF_SIZE (SCREEN_WIDTH * SCREEN_HEIGHT / 10 * (LV_COLOR_DEPTH / 8))
uint32_t buf1[DRAW_BUF_SIZE / 4];
// Get the Touchscreen data
void touchscreen_read(lv_indev_t * indev, lv_indev_data_t * data) {
  if (touchscreen.tirqTouched() && touchscreen.touched()) {
    TS_Point p = touchscreen.getPoint();
    x = map(p.x, 200, 3700, 1, SCREEN_WIDTH);
    y = map(p.y, 240, 3800, 1, SCREEN_HEIGHT);
    z = p.z;

    data->state = LV_INDEV_STATE_PRESSED;
    data->point.x = x;
    data->point.y = y;

    Serial.print("X = ");
    Serial.print(x);
    Serial.print(" | Y = ");
    Serial.print(y);
    Serial.print(" | Pressure = ");
    Serial.print(z);
    Serial.println();

    lastTouchTime = millis();  // Update last touch time
  } else {
    data->state = LV_INDEV_STATE_RELEASED;
  }
}


void setup(){
 String LVGL_Arduino = String("LVGL Library Version: ") + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();
 
  Serial.println(LVGL_Arduino);
  Serial.begin(9600);
  lv_init();
    
  lv_tick_set_cb(xTaskGetTickCount);
  touchscreenSPI.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS);
  touchscreen.begin(touchscreenSPI);
  touchscreen.setRotation(1);

  display = lv_tft_espi_create(SCREEN_WIDTH, SCREEN_HEIGHT, buf1, sizeof(buf1));
}
void loop(){
   lv_task_handler();
  lv_tick_inc(5);
  delay(5);
}