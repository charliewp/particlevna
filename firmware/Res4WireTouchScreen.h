// Touch screen library with X Y and Z (pressure) readings as well
// as oversampling to avoid 'bouncing'
// (c) ladyada / adafruit
// Code under MIT License

#ifndef _ADAFRUIT_RES4WIRETOUCHSCREEN_H_
#define _ADAFRUIT_RES4WIRETOUCHSCREEN_H_

#include <stdint.h>

class TSPoint {
 public:
  TSPoint(void);
  TSPoint(int16_t x, int16_t y, int16_t z);  
  bool operator==(TSPoint);
  bool operator!=(TSPoint);
  int16_t x, y, z;
};

#define TOUCH_TYPE_SIMPLE 0
#define TOUCH_TYPE_SWIPE  1
class TouchScreen {
 public:
  TouchScreen(uint8_t xp, uint8_t yp, uint8_t xm, uint8_t ym);
  TouchScreen(uint8_t xp, uint8_t yp, uint8_t xm, uint8_t ym, uint16_t rx);

  bool isTouching(void);
  uint16_t pressure(void);
  int readTouchY();
  int readTouchX();
  TSPoint getPoint();
  TSPoint getPoint(int pThresh);
  int getTouchType();
  TSPoint getSwipeBox();
  int16_t pressureThreshhold;

protected:
  uint16_t _swipeX;
  uint16_t _swipeY;
  uint8_t _lastTouchType;
  uint8_t _yp, _ym, _xm, _xp;
  uint16_t _rxplate;
  int16_t _x1, _y1, _x2, _y2;
};
#endif
