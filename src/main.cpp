#include <TFT_eSPI.h>
#include <lvgl.h>
#include "mconfig.h"
#include <XPT2046_Touchscreen.h>
#include "screen.c"
#include "fan_back_white.c"
#include "fan_out_back_white.c"
#include "light1.c"
#include "light2.c"
#include "logo.c"
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

// Animation callback function
void anim_exec_cb(void * obj, int32_t value) {
    lv_obj_set_style_img_opa((lv_obj_t *)obj, value, LV_PART_MAIN);
}

// Animation finish callback function
void anim_end_cb(lv_anim_t * a) {
    lv_scr_load(scr2);  // Switch to screen 2
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

  LV_IMAGE_DECLARE(fan_back_white);
  LV_IMAGE_DECLARE(fan_out_back_white);
   LV_IMAGE_DECLARE(light1);
   LV_IMAGE_DECLARE(light2);
  LV_IMAGE_DECLARE(screen);
   LV_IMAGE_DECLARE(logo);
   // Initialize an LVGL input device object (Touchscreen)
  lv_indev_t * indev = lv_indev_create();
  lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
  lv_indev_set_read_cb(indev, touchscreen_read);
  
  static lv_style_t scrstyle;
  lv_style_init(&scrstyle);
  lv_color_t color = lv_color_hex(0xFFFFFF);
  lv_style_set_bg_color(&scrstyle, color);

  // Screen 1
  scr1 = lv_scr_act();
  lv_obj_add_style(scr1, &scrstyle, LV_PART_MAIN);

  lv_obj_t *img = lv_img_create(lv_scr_act());
  lv_img_set_src(img, &logo);
  lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);

  lv_obj_set_style_img_opa(img, LV_OPA_TRANSP, LV_PART_MAIN);

  lv_anim_t a;
  lv_anim_init(&a);
  lv_anim_set_var(&a, img);
  lv_anim_set_exec_cb(&a, anim_exec_cb);
  lv_anim_set_time(&a, 2100);
  lv_anim_set_values(&a, LV_OPA_TRANSP, LV_OPA_COVER);
  lv_anim_set_ready_cb(&a, anim_end_cb);
  lv_anim_start(&a);

}
void loop(){
   lv_task_handler();
  lv_tick_inc(5);
  delay(5);
}