// Touch screen library with X Y and Z (pressure) readings as well
// as oversampling to avoid 'bouncing'
// (c) ladyada / adafruit
// (c) charliep 09/2016 some changes to work with Particle Photon
// Code under MIT License

#include "application.h"
//#include "wiring_private.h"
//#if !defined(__SAM3X8E__)
//	#include <include/pio.h>
//	typedef Pio* Port_t;
//#else
//	#include <avr/pgmspace.h>
//	typedef uint8_t Port_t;
//#endif
#include "Res4WireTouchScreen.h"

// increase or decrease the touchscreen oversampling. This is a little different than you make think:
// 1 is no oversampling, whatever data we get is immediately returned
// 2 is double-sampling and we only return valid data if both points are the same
// 3+ uses insert sort to get the median value.
// We found 2 is precise yet not too slow so we suggest sticking with it!

#define NUMSAMPLES 4

// Particle Photon is 12bit, Arduino 10bit
#define A2DRESOLUTION 4096			

TSPoint::TSPoint(void) {
  x = y = z = 0;
}

TSPoint::TSPoint(int16_t x0, int16_t y0, int16_t z0) {
  x = x0;
  y = y0;
  z = z0;
}

bool TSPoint::operator==(TSPoint p1) {
  return  ((p1.x == x) && (p1.y == y) && (p1.z == z));
}

bool TSPoint::operator!=(TSPoint p1) {
  return  ((p1.x != x) || (p1.y != y) || (p1.z != z));
}

#if (NUMSAMPLES > 2)
static void insert_sort(int array[], uint8_t size) {
  uint8_t j;
  int save;
  
  for (int i = 1; i < size; i++) {
    save = array[i];
    for (j = i; j >= 1 && save < array[j - 1]; j--)
      array[j] = array[j - 1];
    array[j] = save; 
  }
}
#endif

int TouchScreen::getTouchType() {
	return _lastTouchType;
}
TSPoint TouchScreen::getSwipeBox() {
	return TSPoint(_x2-_x1, _y2-_y1, 0);
}

TSPoint TouchScreen::getPoint(int pThresh) {
   int x, y, z;
   int samples[NUMSAMPLES];
   uint8_t i, valid;
   uint16_t loopcount=0;
   _x1=-1;
   _y1=-1;
   _x2=-1;
   _y2=-1;
  do {
   valid = 1;
   //pinMode(_yp, INPUT);
   pinMode(_ym, INPUT);
   //digitalWrite(_yp, LOW);
   digitalWrite(_ym, LOW);
   
   pinMode(_xp, OUTPUT);
   digitalWrite(_xp, HIGH);
   pinMode(_xm, OUTPUT);  
   digitalWrite(_xm, LOW);
  
   for (i=0; i<NUMSAMPLES; i++) {
     samples[i] = analogRead(_yp);
	 //Serial.print(samples[i]);Serial.print(",");
   }
   //Serial.print(" ");
   
   #if NUMSAMPLES > 2
     insert_sort(samples, NUMSAMPLES);
   #endif
   #if NUMSAMPLES == 2
     if (samples[0]>>4 != samples[1]>>4) { valid = 0; }
   #endif

   y = ((A2DRESOLUTION-1)-samples[NUMSAMPLES/2]);

   pinMode(_xp, INPUT);
   //pinMode(_xm, INPUT);
   digitalWrite(_xp, LOW);
   //digitalWrite(_xm, LOW);
        
   pinMode(_yp, OUTPUT);
   digitalWrite(_yp, HIGH);
   pinMode(_ym, OUTPUT);   
   digitalWrite(_ym, LOW);
  
   for (i=0; i<NUMSAMPLES; i++) {
     samples[i] = analogRead(_xm);
	 //Serial.print(samples[i]);Serial.print(",");
   }
   //Serial.print("@");

   #if NUMSAMPLES > 2
     insert_sort(samples, NUMSAMPLES);
   #endif
   #if NUMSAMPLES == 2
     if (samples[0]>>4 != samples[1]>>4) { valid = 0; }
   #endif

   x = ((A2DRESOLUTION-1)-samples[NUMSAMPLES/2]);

   // Set X+ to ground
   pinMode(_xp, OUTPUT);
   digitalWrite(_xp, LOW);
  
   // Set Y- to VCC
   digitalWrite(_ym, HIGH); 
  
   // Hi-Z X- and Y+
   digitalWrite(_yp, LOW);
   //pinMode(_yp, INPUT);
   //pinMode(_xm, INPUT);

   int z1 = analogRead(_xm); 
   int z2 = analogRead(_yp);
   
   if (_rxplate != 0) {
     // now read the x 
     float rtouch;
     rtouch = z2;
     rtouch /= z1;
     rtouch -= 1;
     rtouch *= x;
     rtouch *= _rxplate;
     rtouch /= A2DRESOLUTION;
     
     z = rtouch;
   } else {
     z = ((A2DRESOLUTION-1)-(z2-z1));
   }

   if (! valid) {
     z = 0;
   }
   //Serial.println(z);
   delay(1);
   loopcount++;
   if((valid) && (z>pThresh) && (_x1==-1) && (_y1==-1)) {
	   _x1=x;
	   _y1=y;
   }
  } while(z > pThresh);
  
  if(loopcount>100)
	  _lastTouchType = TOUCH_TYPE_SWIPE;
  else
	  _lastTouchType = TOUCH_TYPE_SIMPLE;
  
   _x2=x;
   _y2=y;
   
   return TSPoint(x, y, z);
}

TSPoint TouchScreen::getPoint(void) {
  int x, y, z;
  int samples[NUMSAMPLES];
  uint8_t i, valid;
  valid = 1;

  //pinMode(_yp, INPUT);
  pinMode(_ym, INPUT);
  //digitalWrite(_yp, LOW);
  digitalWrite(_ym, LOW);
   
  pinMode(_xp, OUTPUT);
  digitalWrite(_xp, HIGH);
  pinMode(_xm, OUTPUT);  
  digitalWrite(_xm, LOW);
  
  for (i=0; i<NUMSAMPLES; i++) {
     samples[i] = analogRead(_yp);
	 //Serial.print(samples[i]);Serial.print(",");
  }
  //Serial.print(" ");
   
#if NUMSAMPLES > 2
   insert_sort(samples, NUMSAMPLES);
#endif
#if NUMSAMPLES == 2
   if (samples[0]>>4 != samples[1]>>4) { valid = 0; }
#endif

   y = ((A2DRESOLUTION-1)-samples[NUMSAMPLES/2]);

   pinMode(_xp, INPUT);
   //pinMode(_xm, INPUT);
   digitalWrite(_xp, LOW);
   //digitalWrite(_xm, LOW);
        
   pinMode(_yp, OUTPUT);
   digitalWrite(_yp, HIGH);
   pinMode(_ym, OUTPUT);   
   digitalWrite(_ym, LOW);
  
   for (i=0; i<NUMSAMPLES; i++) {
     samples[i] = analogRead(_xm);
	 //Serial.print(samples[i]);Serial.print(",");
   }
   //Serial.print("@");

#if NUMSAMPLES > 2
   insert_sort(samples, NUMSAMPLES);
#endif
#if NUMSAMPLES == 2
   if (samples[0]>>4 != samples[1]>>4) { valid = 0; }
#endif

   x = ((A2DRESOLUTION-1)-samples[NUMSAMPLES/2]);

   // Set X+ to ground
   pinMode(_xp, OUTPUT);
   digitalWrite(_xp, LOW);
  
   // Set Y- to VCC
   digitalWrite(_ym, HIGH); 
  
   // Hi-Z X- and Y+
   digitalWrite(_yp, LOW);
   //pinMode(_yp, INPUT);
   //pinMode(_xm, INPUT);

   int z1 = analogRead(_xm); 
   int z2 = analogRead(_yp);
   
   if (_rxplate != 0) {
     // now read the x 
     float rtouch;
     rtouch = z2;
     rtouch /= z1;
     rtouch -= 1;
     rtouch *= x;
     rtouch *= _rxplate;
     rtouch /= A2DRESOLUTION;
     
     z = rtouch;
   } else {
     z = ((A2DRESOLUTION-1)-(z2-z1));
   }

   if (! valid) {
     z = 0;
   }
   //Serial.println(z);

   return TSPoint(x, y, z);
}

TouchScreen::TouchScreen(uint8_t xp, uint8_t yp, uint8_t xm, uint8_t ym) {
  _yp = yp;
  _xm = xm;
  _ym = ym;
  _xp = xp;
  _rxplate = 0;
  pressureThreshhold = 10;
}


TouchScreen::TouchScreen(uint8_t xp, uint8_t yp, uint8_t xm, uint8_t ym,
			 uint16_t rxplate) {
  _yp = yp;
  _xm = xm;
  _ym = ym;
  _xp = xp;
  _rxplate = rxplate;

  pressureThreshhold = 10;
}

int TouchScreen::readTouchX(void) {
   pinMode(_yp, INPUT);
   pinMode(_ym, INPUT);
   digitalWrite(_yp, LOW);
   digitalWrite(_ym, LOW);
   
   pinMode(_xp, OUTPUT);
   digitalWrite(_xp, HIGH);
   pinMode(_xm, OUTPUT);
   digitalWrite(_xm, LOW);
   
   return ((A2DRESOLUTION-1)-analogRead(_yp));   
}


int TouchScreen::readTouchY(void) {
   pinMode(_xp, INPUT);
   pinMode(_xm, INPUT);
   digitalWrite(_xp, LOW);
   digitalWrite(_xm, LOW);
   
   pinMode(_yp, OUTPUT);
   digitalWrite(_yp, HIGH);
   pinMode(_ym, OUTPUT);
   digitalWrite(_ym, LOW);
   
   return ((A2DRESOLUTION-1)-analogRead(_xm));
}


uint16_t TouchScreen::pressure(void) {
  // Set X+ to ground
  pinMode(_xp, OUTPUT);
  digitalWrite(_xp, LOW);
  
  // Set Y- to VCC
  pinMode(_ym, OUTPUT);
  digitalWrite(_ym, HIGH); 
  
  // Hi-Z X- and Y+
  digitalWrite(_xm, LOW);
  pinMode(_xm, INPUT);
  digitalWrite(_yp, LOW);
  pinMode(_yp, INPUT);
  
  int z1 = analogRead(_xm); 
  int z2 = analogRead(_yp);

  if (_rxplate != 0) {
    // now read the x 
    float rtouch;
    rtouch = z2;
    rtouch /= z1;
    rtouch -= 1;
    rtouch *= readTouchX();
    rtouch *= _rxplate;
    rtouch /= A2DRESOLUTION;
    
    return rtouch;
  } else {
    return ((A2DRESOLUTION-1)-(z2-z1));
  }
}
