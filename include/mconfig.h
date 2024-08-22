#ifndef MCONFIG_H
#define MCONFIG_H

#ifdef TFTILI9341
  #define SCREEN_WIDTH 320
  #define SCREEN_HEIGHT 240
#else 
  #define SCREEN_WIDTH 480
  #define SCREEN_HEIGHT 320
#endif
#define TOUCH_CS 33     // Chip select pin (T_CS) of touch screen
#endif