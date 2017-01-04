/***************************************************
  VnaUi - Vector Network Analyzer User Interface

  Written by Charlie Price/ICCD for Fun and Enjoyment.
  MIT license, all text above must be included in any redistribution
 ****************************************************/

#include "VnaUi.h"
#include <math.h>

uint8_t StaticVars::targetPage = 0;

void StaticVars::EEPROM_write(uint16_t addr,uint8_t data) {
  #ifndef EMULATED_EEPROM
  Wire.beginTransmission(EEPROM_ADDR);
  Wire._I2C_WRITE((uint8_t)(addr >> 8));       // MSB
  Wire._I2C_WRITE((uint8_t)(addr & 0xFF));     // LSB
  Wire._I2C_WRITE((uint8_t) data);
  Wire.endTransmission();
  char hex[6];
  if((addr%16)==0) {
       sprintf(hex,"0x%04x",addr);
       Serial.println(); Serial.print(hex);Serial.print(": ");
  }
  sprintf(hex,"0x%02x ",data);
  Serial.print(hex);
  delay(5);
  #else
  //Serial.print(emulated_EEPROM[addr],HEX); Serial.print(" ");
  //if((addr%16)==0)
	//Serial.print("\n");
  emulated_EEPROM[addr] = data;
  #endif

}

byte StaticVars::EEPROM_read(uint16_t addr) {
  #ifndef EMULATED_EEPROM
  byte data = 0xFF;
  Wire.beginTransmission(EEPROM_ADDR);
  Wire._I2C_WRITE((uint8_t)(addr >> 8));       // MSB
  Wire._I2C_WRITE((uint8_t)(addr & 0xFF));     // LSB
  Wire.endTransmission();
  Wire.requestFrom(EEPROM_ADDR,1);
  if (Wire.available())
    data = Wire._I2C_READ();
  delay(2);
  return data;
  #else
  //Serial.print(emulated_EEPROM[addr],HEX); Serial.print(" ");
  //if((addr%16)==0)
	//Serial.print("\n");
  return emulated_EEPROM[addr];
  #endif
}

Page::Page(uint8_t pageId) {
 _pageId = pageId;
}
uint8_t Page::getPageId() {
  return _pageId;
}


KeyPad::KeyPad(uint8_t pageId) : Page(pageId) {
  _numberOfButtons = 0;
}
void KeyPad::addButton(Vna_GFX_Button button) {
	_buttons[_numberOfButtons++] = button;
}
void KeyPad::attach(Adafruit_HX8357 *gfx, ButtonDefinitionSet bset) {
  _gfx = gfx;
  char c;
  int m=0;
  do {
	  c = bset._banner[m];
	  _keypadLabel[m] = c;
	  m++;
  } while(c!=0);
  for(int n=0; n<bset.getButtonDefinitionCount(); n++) {
	  Serial.print("adding button definition "); Serial.println(n);
	  ButtonDefinition bdef = bset.getButtonDefinition(n);
	  Vna_GFX_Button newButton;
	  newButton.initButton(_gfx, bdef);
	  addButton(newButton);
  }
}
void KeyPad::clearRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
	for(int n=y; n<y+h; n++)
			  _gfx->drawFastHLine(x, n, w, color);
}
void KeyPad::show() {
  clearRect(C_7, R_1, 480-C_7, 320-R_1, ILI9341_BLACK);
  _gfx->setFont(SOURCECODEPRO_22);
  _gfx->setTextSize(1);
  _gfx->setTextColor(ILI9341_WHITE);
  _gfx->setCursor(C_7, R_1);
  _gfx->println(_keypadLabel);
  for(uint8_t i=0; i<_numberOfButtons; i++) {
     _buttons[i].drawButton(false);
  }
}
PageAction KeyPad::contains(int16_t x, int16_t y) {
  //StaticVars staticVars;
  PageAction pageAction(BUTTON_TYPE_NULL,0);
  for(int i=0; i<_numberOfButtons; i++) {
    if (_buttons[i].contains(x, y)) {
        //Serial.print("xPressing: "); Serial.println(i);
        _buttons[i].press(true);  // tell the button it is pressed
		pageAction.setTypeAndAction(_buttons[i].getType(),_buttons[i].getPosition());
    } else {
        _buttons[i].press(false);  // tell the button it is NOT pressed
    } // end - if (_buttons[i].contains(x, y))

	if (_buttons[i].justReleased()) {
        //Serial.print("Released: "); Serial.println(i);
        //_buttons[i].drawButton(false);  // draw normal

    }
    if (_buttons[i].justPressed()) {
	   if(_buttons[i].getType() != BUTTON_TYPE_NULL) {
	      _buttons[i].drawButton(true);  // draw invert!
          delay(200);
          _buttons[i].drawButton(false);
	   }
    }
  }
  return pageAction;
}

void SwipeBar::attach(Adafruit_HX8357 *gfx, uint16_t x_org, uint16_t y_org, uint16_t width, uint16_t height, uint8_t type, long deviation, long markerSpacing, uint8_t text_division, char *fmt, uint16_t fmtdiv, uint16_t ratio, uint16_t background_color, uint16_t indicator_color, uint16_t cfmarker_color) {
		_gfx = gfx;
		_x_org = x_org;
		_y_org = y_org;
		_width = width;
		_height = height;
		_ratio = ratio;
		_background_color = background_color;
		_indicator_color = indicator_color;
		_cfmarker_color = cfmarker_color;
		_markerSpacing = markerSpacing;
		_deviation = deviation;
		_text_division = text_division;
		_position = 0;
		_fmtdiv = fmtdiv;
		int n=0;
        char s;
		do {
          s = fmt[n];
	      _fmt[n++] = s;
         } while (s != 0);
		 _type = type;
		 clearRect(_x_org, _y_org, _width, _height, _background_color);
		_gfx->drawRect(_x_org, _y_org, _width, _height, _indicator_color);
		_gfx->drawFastHLine(_x_org, _y_org+_height/2, _width, _indicator_color);
		_gfx->drawFastVLine(_x_org+_width/2, _y_org, _height, _cfmarker_color);
}
PageAction SwipeBar::contains(int16_t x, int16_t y) {
  PageAction pageAction(SWIPEBAR_TYPE_NULL,0);
  if((x>=_x_org) && (x<_x_org+_width) && (y>=_y_org) && (y<_y_org+_height))
    pageAction.setTypeAndAction(SWIPEBAR_TYPE_FOCUS, SWIPEBAR_TYPE_FOCUS);
  return pageAction;
}

void SwipeBar::clearRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
	for(int n=y; n<y+h; n++)
			  _gfx->drawFastHLine(x, n, w, color);
}
void SwipeBar::show() {
	    //erase the oldMarkers and text
		_gfx->setFont(ARIAL_12);
        _gfx->setTextSize(1);
        _gfx->setTextColor(_background_color);
		for(int n=0; n<MAX_MARKERS; n++) {
			_gfx->drawFastVLine(_lastMarker[n], _y_org+_height/2+1, 4, _background_color);
			_gfx->setCursor(_lastMarker[n]-20, _y_org+_height/2+10);
	        _gfx->println(_lastDisplayFrequency[n]);
		}
		//_gfx->drawRect(_x_org, _y_org, _width, _height, _indicator_color);
		_gfx->drawFastHLine(_x_org, _y_org+_height/2, _width, _indicator_color);
		_gfx->drawFastVLine(_x_org+_width/2, _y_org, _height, _cfmarker_color);
		//now place the markers on the horizontal line
		int idx=0;
		_gfx->setTextColor(_indicator_color);
		for(long n=_position-(_markerSpacing*_deviation); n<_position+(_markerSpacing*_deviation); n+=1) {
			uint16_t markerX = map(n, _position-(_markerSpacing*_deviation), _position+(_markerSpacing*_deviation), 0, _width);
			if((n%_markerSpacing)==0) {
		      _gfx->drawFastVLine(_x_org+markerX, _y_org+_height/2, 5, _indicator_color);
			  _lastMarker[idx]=_x_org+markerX;
			  if(((idx%_text_division)==0) && (_x_org+markerX<(_x_org+_width-40))) {
			     _gfx->setCursor(_x_org+markerX-20, _y_org+_height/2+10);
				 switch(_type) {
				   case SWIPEBAR_FREQ:  sprintf(_lastDisplayFrequency[idx],_fmt,((float)(float)n/(float)_fmtdiv)); break;
				   case SWIPEBAR_WAVELENGTH: sprintf(_lastDisplayFrequency[idx],_fmt,(float)((float)300000/(float)(n))); break;
				 }
				 _gfx->println(_lastDisplayFrequency[idx]);
			  } else {
				 _lastDisplayFrequency[idx][0] = 0;
			  }
              idx++;
			}
		}
		while(idx<MAX_MARKERS) {
			_lastDisplayFrequency[idx++][0]=0;
		}
}
void SwipeBar::setPosition(long position) {
		_position = position;
}
void SwipeBar::changePosition(long changePosition) {
		_position += (changePosition/_ratio);
}
long SwipeBar::getPosition() {
		return _position;
}
PageAction::PageAction(uint8_t type, uint8_t value) {
  _type = type;
  _value = value;
}
PageAction::~PageAction() {
}
uint8_t PageAction::getType() {
  return _type;
}
uint8_t PageAction::getValue() {
  return _value;
}
uint8_t PageAction::getCommand() {
  return _value;
}
void PageAction::setTypeAndAction(uint8_t type, uint8_t value) {
  _type = type;
  _value = value;
}


Vna_GFX_Button::Vna_GFX_Button(void) {
   _gfx = 0;
}
void Vna_GFX_Button::initButton(Adafruit_HX8357 *gfx,
					  uint8_t id,
					  int16_t x, int16_t y,
					  uint8_t w, uint16_t h,
					  uint16_t outline, uint16_t fill,
					  uint16_t textcolor,
					  char *label, uint8_t type, uint8_t position, uint8_t max, uint16_t baseAddr, uint8_t labelfont, uint8_t valuefont)
{
  _x = x;
  _y = y;
  _w = w;
  _h = h;
  _outlinecolor = outline;
  _fillcolor = fill;
  _textcolor = textcolor;
  _textsize = 1;
  _labelFont = labelfont;
  _valueFont = valuefont;
  _gfx = gfx;
  int n=0;
  char s;
  do {
    s = label[n];
	_label[n++] = s;
  } while (s != 0);
  _position = position;
  _type = type;
  _max = max;
  _id = id;
  _baseAddr = baseAddr;
}

void Vna_GFX_Button::initButton(Adafruit_HX8357 *gfx, ButtonDefinition bdef) {
  _x = bdef._buttonXorg;
  _y = bdef._buttonYorg;
  _w = bdef._buttonWidth;
  _h = bdef._buttonHeight;
  _outlinecolor = bdef._buttonOutlineColor;
  _fillcolor = bdef._buttonFillColor;
  _textcolor = bdef._buttonTextColor;
  _textsize = 1;
  _labelFont = bdef._buttonLabelFont;
  _valueFont = bdef._buttonValueFont;
  _gfx = gfx;
  int n=0;
  char s;
  do {
    s = bdef._buttonLabel[n];
	_label[n++] = s;
  } while (s != 0);
  _position = bdef._buttonValue;
  _type = bdef._buttonType;
  _max = bdef._buttonMaxValue;
  _id = bdef._buttonId;
  _baseAddr = bdef._buttonBaseAddr;
}

void Vna_GFX_Button::drawButton(boolean inverted) {
   uint16_t outline, text;
   if (! inverted) {
     outline = _outlinecolor;
     text = _textcolor;
   } else {
     outline = BUTTON_FILL_PRESSED_COLOR; //_outlinecolor;
     text = BUTTON_TEXT_PRESSED_COLOR;
   }
   //THE BUTTON
   _gfx->drawRoundRect(_x , _y, _w, _h, min(_w,_h)/8, outline);
   //_gfx->fillRoundRect(_x - (_w/2), _y - (_h/2), _w, _h, min(_w,_h)/8, fill);
   //THE LABEL - UPPER LEFT ALIGNMENT
   _gfx -> setFont(_labelFont);
   _gfx->setCursor(_x  + 4, _y  +2*_textsize + min(_w,_h)/8);
   _gfx->setTextColor(text);
   _gfx->setTextSize(_textsize);
   _gfx->print(_label);
   if((_type != BUTTON_TYPE_NAV) && (_type != BUTTON_TYPE_CALL)){
     //THE CURRENT VALUE - LOWER RIGHT ALIGNMENT
     _gfx->setCursor(_x  + 10,40+_y-4*_textsize);
     _gfx->setTextColor(text);
	 _gfx -> setFont(_valueFont);
     _gfx->print(_valueText);
   }
 }
boolean Vna_GFX_Button::contains(int16_t x, int16_t y) {
   if ((x < (_x )) || (x > (_x + _w))) return false;
   if ((y < (_y)) || (y > (_y + _h))) return false;
   uint16_t _rx = x - (_x);
   uint16_t _ry = y - (_y);
   //Serial.print((_h/2));Serial.print(" _rx=");Serial.print(_rx);Serial.print(",_ry=");Serial.print(_ry);
   if((_rx + _ry) > (_h)) {
     _isIncremented=false;
	 //Serial.println(" DEC");
   } else {
     _isIncremented=true;
     //Serial.println(" INC");
   }
   return true;
 }
boolean Vna_GFX_Button::isIncrement() {
   return _isIncremented;
}
void Vna_GFX_Button::setButtonPosition(uint8_t position) {
   _position = position;
}
void Vna_GFX_Button::setValueText(char* text) {
}

uint8_t Vna_GFX_Button::getButtonPosition() {
   return _position;
}
uint16_t Vna_GFX_Button::getBaseAddr() {
   return _baseAddr;
}
uint8_t Vna_GFX_Button::getId() {
   return _id;
}
uint8_t Vna_GFX_Button::getType() {
   return _type;
}
uint8_t Vna_GFX_Button::getPosition() {
   return _position;
}
uint8_t Vna_GFX_Button::getMax() {
   return _max;
}
void Vna_GFX_Button::press(boolean p) {
   laststate = currstate;
   currstate = p;
}
boolean Vna_GFX_Button::isPressed() { return currstate; }
boolean Vna_GFX_Button::justPressed() { return (currstate && !laststate); }
boolean Vna_GFX_Button::justReleased() { return (!currstate && laststate); }

ButtonDefinition::ButtonDefinition() {
 }

void ButtonDefinition::parseCSV(String s) {
  int startFromIndex = 0;
  boolean noMoreDelimiters = false;
  int nextDelimiterIndex = -1;
  uint8_t tokenIndex = 0;
  do {
    String token;
    nextDelimiterIndex =  s.indexOf(',', startFromIndex);
	if(nextDelimiterIndex>0)
      token = s.substring(startFromIndex,nextDelimiterIndex);
	else
      token = s.substring(startFromIndex);
	token.toUpperCase();

	switch(tokenIndex++) {
	  case BUTTON_ID  			: _buttonId = atoi(token); break;
      case BUTTON_XORG    		: _buttonXorg = atoi(token); break;
	  case BUTTON_YORG  		: _buttonYorg = atoi(token); break;
	  case BUTTON_WIDTH  		: _buttonWidth = atoi(token); break;
	  case BUTTON_HEIGHT   		: _buttonHeight = atoi(token); break;
	  case BUTTON_OUTLINE_COLOR : _buttonOutlineColor = atol(token); break;
	  case BUTTON_FILL_COLOR 	: _buttonFillColor = atol(token); break;
	  case BUTTON_TEXT_COLOR  	: _buttonTextColor = atol(token); break;
	  case BUTTON_LABEL  		: {
		  char c;
		  int n=0;
		  int m=0;
		  do {
			  c=token.charAt(n);
			  if(c!=34)
			    _buttonLabel[m++]=c;
			  n++;
		  } while(c!=0);
		  break;
	  }
	  case BUTTON_TYPE  		: _buttonType = atoi(token); break;
	  case BUTTON_VALUE  		: _buttonValue = atoi(token); break;
	  case BUTTON_MAX_VALUE  	: _buttonMaxValue = atoi(token); break;
	  case BUTTON_BASE_ADDR  	: _buttonBaseAddr = atoi(token); break;
	  case BUTTON_LABEL_FONT  	: _buttonLabelFont = atoi(token); break;
	  case BUTTON_VALUE_FONT 	: _buttonValueFont = atoi(token); break;
	}
	if(nextDelimiterIndex !=  -1)
       startFromIndex = nextDelimiterIndex + 1;
    else
       noMoreDelimiters = true;
  } while(!noMoreDelimiters);
}

ButtonDefinitionSet::ButtonDefinitionSet() {
  _buttonDefinitionCount = 0;
}
void ButtonDefinitionSet::addButtonDefinition(ButtonDefinition def) {
  if(_buttonDefinitionCount < (_MAX_BUTTONDEF_COUNT) )
    _buttonDefinition[_buttonDefinitionCount++] = def;
}

void ButtonDefinitionSet::addPageBanner(String s) {
	char c;
	//[BANNER]
	int n=8;
	do {
		c = s.charAt(n);
		_banner[n-8] = c;
		n++;
	} while(c!=0);
}
uint8_t ButtonDefinitionSet::getButtonDefinitionCount() {
  return _buttonDefinitionCount;
}
ButtonDefinition ButtonDefinitionSet::getButtonDefinition(int index) {
  return _buttonDefinition[index];
}

void PolarPlot::attach(Adafruit_HX8357 *gfx, uint16_t x_c, uint16_t y_c, uint16_t radius, uint16_t background_color) {
	_gfx = gfx;
	_x_center = x_c;
	_y_center = y_c;
	_radius = radius;
	_background_color = background_color;

}
void PolarPlot::setThetaRange(long hz_per_revolution, long minor_div, long major_div, uint16_t minor_div_color, uint16_t major_div_color) {
	_hz_per_revolution = hz_per_revolution;
	_t_minor_div = minor_div;
	_t_major_div = major_div;
	_t_minor_div_color = minor_div_color;
	_t_major_div_color = major_div_color;
	_scale_t = (double) ((double)360.0/(double)hz_per_revolution);
	char scale[16];
	sprintf(scale,"%f",_scale_t);
	Serial.print("_scale_t=");Serial.println(scale);
}
void PolarPlot::addRRange(const char *label, uint8_t range_id, long min_r_value,   long max_r_value,  long minor_div, long major_div, uint16_t range_color, uint16_t minor_div_color, uint16_t major_div_color) {
	  int n=0;
	char c;
	do {
		c = label[n];
		_label[range_id][n] = c;
		n++;
	} while(c!=0);
	_min_r[range_id] = min_r_value;
	_max_r[range_id] = max_r_value;
	_r_minor_div[range_id] = minor_div;
	_r_major_div[range_id] = major_div;
	_r_range_color[range_id] = range_color;
	_r_range_major_div_color[range_id] = major_div_color;
    _r_range_minor_div_color[range_id] = minor_div_color;
	_scale_r[range_id] = (double) ((double)POLARPLOT_RADIUS/(double)(max_r_value-min_r_value));
	char scale[16];
	sprintf(scale,"%f",_scale_r[range_id]);
	Serial.print("_scale_r[0]=");Serial.println(scale);
	_last_x[range_id] = _x_center;
	_last_y[range_id] = _y_center;

}
void PolarPlot::setThetaAndR(uint8_t range_id, long t_value, long r_value) {

}
void PolarPlot::plotThetaAndR(uint8_t range_id, long t_value, long r_value) {
	//calculate coordinates of point, save in array, and plot
	uint16_t pX;
	uint16_t pY;
	double theta = (double) (2.0 * 3.141592654 * (t_value % _hz_per_revolution) * _scale_t)/360.0;
	#define QUAD1  3.141592654/2
	#define QUAD2  3.141592654
	#define QUAD3  3.0*3.141592654/2
	Serial.print("DF="); Serial.println(t_value % _hz_per_revolution);
	Serial.print("theta=");Serial.println(theta);
	pX = _x_center + r_value * _scale_r[range_id] * cos(theta-QUAD3);
	pY = _y_center - r_value * _scale_r[range_id] * sin(theta-QUAD3);
	_gfx->drawLine(_last_x[range_id], _last_y[range_id], pX, pY, _r_range_color[range_id]);
	_last_x[range_id] = pX;
	_last_y[range_id] = pY;
	char point[16];
	sprintf(point,"x=%i,y=%i",pX, pY);
}
void PolarPlot::fill(uint8_t range_id) {

}
void PolarPlot::show(uint8_t range_id, long start_x, long stop_x, long step_x) {

}
void PolarPlot::showGraticule(uint8_t range_id, long step_t) {
	for(int n=POLARPLOT_AREA_START_Y; n<=POLARPLOT_AREA_STOP_Y; n++)
	  _gfx->drawFastHLine(POLARPLOT_AREA_START_X, n,(POLARPLOT_AREA_STOP_X-POLARPLOT_AREA_START_X)+1, _background_color);

	uint16_t pX;
	uint16_t pY;

	for(long freq=0; freq<_hz_per_revolution; freq+=_t_minor_div) {
		double theta = (double) (2.0 * 3.141592654 * (freq % _hz_per_revolution) * _scale_t)/360.0;
		uint16_t color;
		if(freq%_t_major_div==0) {
			color = _r_range_major_div_color[range_id];
		} else {
			color = _r_range_minor_div_color[range_id];
		}
	    pX = _x_center + 132.0 * cos(theta-QUAD3);
	    pY = _y_center - 132.0 * sin(theta-QUAD3);
	    _gfx->drawLine(_x_center, _y_center, pX, pY, color);
	}
    for(long rVal=_min_r[range_id]; rVal<_max_r[range_id]; rVal+=_r_minor_div[range_id]) {
		uint16_t color;
		if(rVal%_r_major_div[range_id]==0)
			color = _r_range_major_div_color[range_id];
		else
			color = _r_range_minor_div_color[range_id];
		uint16_t radius = rVal * _scale_r[range_id];
		_gfx->drawCircle(_x_center, _y_center, radius, color);
	}
	_gfx->drawCircle(_x_center, _y_center, 132, _r_range_major_div_color[range_id]);
}
void PolarPlot::showLegendBlock(uint8_t range_id, long start_x, long stop_x, long step_x) {}
void PolarPlot::clearPolarPlot() {

}
uint16_t PolarPlot::getTInfo(uint8_t range_id, long t_value) {
	return _r_values[range_id][0];
}

void XyPlot::attach(Adafruit_HX8357 *gfx, uint16_t x_org, uint16_t y_org, uint16_t x_width, uint16_t y_height, uint16_t background_color) {
  //e.g vswrPlot.attach(&tft, 5, 5, 315, 315, ILI9341_BLACK);
  _gfx = gfx;
  _x_org = x_org;
  _y_org = y_org;
  _x_width = x_width;
  _y_height = y_height;
  _background_color = _background_color;
}
void XyPlot::setXRange(long start_x, long stop_x, long minor_div, long major_div, uint16_t minor_div_color, uint16_t major_div_color) {
	//e.g vswrPlot.setXRange(7100000, 7300000, 10000, 50000, ILI9341_ORANGE);
	_start_x = start_x;
	_scale_x = (double)(_x_width-1)/(stop_x - start_x);		// [x-index] = (x_value - start_x) * scale_x
	_x_minor_div = minor_div;
	_x_major_div = major_div;
	_x_minor_div_color = minor_div_color;
	_x_major_div_color = major_div_color;
	//char dval[32];
	//sprintf(dval,"_x_scale=%2.8f",_scale_x);
	//Serial.println(dval);
}
#define LAST_VALUE_STARTUP 1024
void XyPlot::addYRange(const char *label, uint8_t range_id, long min_y,   long max_y, long minor_div, long major_div, uint16_t range_color, uint16_t minor_div_color, uint16_t major_div_color) {
	_min_y[range_id] = min_y;
	_max_y[range_id] = max_y;
	int n=0;
	char c;
	do {
		c = label[n];
		_label[range_id][n] = c;
		n++;
	} while(c!=0);
	_scale_y[range_id] = (double) (_y_height-1)/(max_y - min_y);
	_y_minor_div[range_id] = minor_div;
	_y_major_div[range_id] = major_div;
	_y_range_color[range_id] = range_color;
	_y_range_major_div_color[range_id] = major_div_color;
	_y_range_minor_div_color[range_id] = minor_div_color;
	for(uint16_t x=0; x<XYPLOT_X_WIDTH; x++) {
	  _y_values[range_id][x] = 0;
	}
}
void XyPlot::setXY(uint8_t range_id, long x_value, long y_value) {
	int index = (int) (x_value - _start_x) * _scale_x;
	_y_values[range_id][index] = (y_value-_min_y[range_id]) * _scale_y[range_id];
	char inputPair[32];
	sprintf(inputPair,"x=%lu,y=%lu",x_value,y_value);
	char outputPair[32];
	sprintf(outputPair,"x=%i,y=%i",index,_y_values[range_id][index]);
    Serial.print(inputPair);Serial.print("-->");Serial.println(outputPair);
}
void XyPlot::plotXY(uint8_t range_id, long x_value, long y_value) {
	int index = (int) (x_value - _start_x) * _scale_x;
	_y_values[range_id][index] = _y_height - ((y_value-_min_y[range_id]) * _scale_y[range_id]);
	long idx = (int) (x_value - _start_x) * _scale_x;
	if((_last_x[range_id]!=LAST_VALUE_STARTUP) && (_last_y[range_id]!=LAST_VALUE_STARTUP))
	  _gfx->drawLine(_last_x[range_id], _last_y[range_id], _x_org+idx, _y_org + _y_height - ((y_value-_min_y[range_id]) * _scale_y[range_id]), _y_range_color[range_id]);
    _last_x[range_id] = _x_org+idx;
    _last_y[range_id] = _y_org + _y_height - ((y_value-_min_y[range_id]) * _scale_y[range_id]);
	_y_values[range_id][idx]= ((y_value-_min_y[range_id]) * _scale_y[range_id]);
}
uint16_t XyPlot::getYInfo(uint8_t range_id, long x_value) {
	uint16_t value = _y_values[range_id][x_value-_x_org]/_scale_y[range_id];
	_gfx->setRotation(1);
    _gfx->setFont(FUTURA_14);
    _gfx->setTextSize(1);
    _gfx->setTextColor(_y_range_color[range_id]);
	_gfx->setCursor(VALUE_X	,VALUE_Y);
	char dValue[16];
	sprintf(dValue,"%i",value);
	clearRect(VALUE_X,VALUE_Y,100,20,ILI9341_BLACK);
	_gfx->println(dValue);
	return value;
}
void XyPlot::fill(uint8_t range_id) {
  	for(uint16_t x=0; x<XYPLOT_X_WIDTH; x++) {
		int m=1;
		if(_y_values[range_id][x]==0) {
			for(m=1; m<XYPLOT_X_WIDTH-x; m++) {
			  if(_y_values[range_id][x+m]!=0)
                break;
			}
			uint16_t fillValue=(m*_y_values[range_id][x-1])/(m+1) + _y_values[range_id][x+m]/(m+1);
			_y_values[range_id][x] = fillValue;
		}
	}
}
void XyPlot::show(uint8_t range_id, long start_x, long stop_x, long step_x) {
	_gfx->fillRect(_x_org, _y_org, _x_width, _y_height, _background_color);
	//y-axis minor and major divisions
	for(long y=_min_y[range_id]; y<=_max_y[range_id]; y+=_y_minor_div[range_id]) {
		if((y%_y_major_div[range_id])==0) {
			//char xandy[32];
			uint16_t ypoint = (y-_min_y[range_id]) * _scale_y[range_id];
			//sprintf(xandy,"maj x=%u,y=%u",_x_org,_y_org+ypoint);
			//Serial.println(xandy);
			_gfx->drawLine(_x_org, _y_org+ypoint, _x_org+_x_width, _y_org+ypoint, _y_range_major_div_color[range_id]);
		} else if((y%_y_minor_div[range_id])==0) {
			//char xandy[32];
			uint16_t ypoint = (y-_min_y[range_id]) * _scale_y[range_id];
			//sprintf(xandy,"min x=%u,y=%u",_x_org,_y_org+ypoint);
			//Serial.println(xandy);
			_gfx->drawLine(_x_org, _y_org+ypoint, _x_org+_x_width, _y_org+ypoint, _y_range_minor_div_color[range_id]);
		}
	}
	_gfx->setFont(ARIAL_8);
    _gfx->setTextSize(1);
    _gfx->setTextColor(_y_range_color[range_id]);
	//here we plot the points
	for(long index=start_x; index<=stop_x; index+=step_x) {
		long idx = (int) (index - _start_x) * _scale_x;
		_gfx->setCursor(_x_org+idx, _y_org+_y_values[range_id][idx]);
		_gfx->print(".");
		if((index%_x_major_div)==0) {
			_gfx->drawLine(_x_org+idx, _y_org, _x_org+idx, _y_org + _y_height, _x_major_div_color);
		} else if((index%_x_minor_div)==0) {
			_gfx->drawLine(_x_org+idx, _y_org, _x_org+idx, _y_org + _y_height, _x_minor_div_color);
		}
	}
}
void XyPlot::clearRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
	for(int n=y; n<y+h; n++)
			  _gfx->drawFastHLine(x, n, w, color);
}
void XyPlot::showLegendBlock(uint8_t range_id, long start_x, long stop_x, long step_x) {
	#define LABEL_OFFSET_X 10
	#define LABEL_OFFSET_Y 10
	//clearRect(LEGEND_X	,LEGEND_Y, LEGEND_W, LEGEND_H, ILI9341_BLACK );
    _gfx->setRotation(1);
    _gfx->setFont(FUTURA_14);
    _gfx->setTextSize(1);
    _gfx->setTextColor(_y_range_color[range_id]);
	_gfx->setCursor(LEGEND_X	,LEGEND_Y);
	_gfx->println(_label[range_id]);
	_gfx->setFont(ARIAL_11);
    char legend[16];

	_gfx->drawLine(CF_ICON_X1, CF_ICON_Y1, CF_ICON_X2, CF_ICON_Y2, _y_range_color[range_id]);
	_gfx->drawLine(CF_ICON_X3, CF_ICON_Y3, CF_ICON_X4, CF_ICON_Y4, _y_range_color[range_id]);
    _gfx->drawLine(BW_ICON_X1, BW_ICON_Y1, BW_ICON_X2, BW_ICON_Y2, _y_range_color[range_id]);
    _gfx->drawLine(BW_ICON_X2, BW_ICON_Y2, BW_ICON_X3, BW_ICON_Y3, _y_range_color[range_id]);
    _gfx->drawLine(BW_ICON_X3, BW_ICON_Y3, BW_ICON_X4, BW_ICON_Y4, _y_range_color[range_id]);
    _gfx->drawLine(BW_ICON_X4, BW_ICON_Y4, BW_ICON_X5, BW_ICON_Y5, _y_range_color[range_id]);
    _gfx->drawLine(BW_ICON_X5, BW_ICON_Y5, BW_ICON_X6, BW_ICON_Y6, _y_range_color[range_id]);
    _gfx->drawLine(ST_ICON_X1, ST_ICON_Y1, ST_ICON_X2, ST_ICON_Y2, _y_range_color[range_id]);
    _gfx->drawLine(ST_ICON_X2, ST_ICON_Y2, ST_ICON_X3, ST_ICON_Y3, _y_range_color[range_id]);
    _gfx->drawLine(ST_ICON_X3, ST_ICON_Y3, ST_ICON_X4, ST_ICON_Y4, _y_range_color[range_id]);
    sprintf(legend,"%luHz",(start_x+stop_x)/2);
    _gfx->setCursor(CF_TEXT_X, CF_TEXT_Y);
	_gfx->println(legend);
    sprintf(legend,"%luHz",(stop_x-start_x));
    _gfx->setCursor(BW_TEXT_X, BW_TEXT_Y);
	_gfx->println(legend);
    sprintf(legend,"%luHz",step_x);
    _gfx->setCursor(ST_TEXT_X, ST_TEXT_Y);
	_gfx->println(legend);
}
void XyPlot::showGraticule(uint8_t range_id, long start_x, long stop_x, long step_x) {
    _gfx->setRotation(1);
	_gfx->fillRect(XYPLOT_AREA_START_X , XYPLOT_AREA_START_Y, XYPLOT_AREA_STOP_X-XYPLOT_AREA_START_X+1, XYPLOT_AREA_STOP_Y-XYPLOT_AREA_START_Y+1, _background_color);
	//y-axis minor and major divisions
	for(long y=_min_y[range_id]; y<=_max_y[range_id]; y+=_y_minor_div[range_id]) {
		if((y%_y_major_div[range_id])==0) {
			//char xandy[32];
			uint16_t ypoint = (y-_min_y[range_id]) * _scale_y[range_id];
			//sprintf(xandy,"maj x=%u,y=%u",_x_org,_y_org+ypoint);
			//Serial.println(xandy);
			_gfx->drawLine(_x_org, _y_org+ypoint, _x_org+_x_width, _y_org+ypoint, _y_range_major_div_color[range_id]);
			_gfx->setFont(ARIAL_12);
			_gfx->setTextSize(1);
			_gfx->setTextColor(_y_range_color[range_id]);
			_gfx->setCursor(_x_org+_x_width-45, _y_org+ypoint-20);
			char xVal[8];
			if((y!=_min_y[range_id]) && (y!=_max_y[range_id])) {
			  sprintf(xVal,"%05ld",_max_y[range_id]-y);
			  _gfx->print(xVal);
			}
		} else if((y%_y_minor_div[range_id])==0) {
			//char xandy[32];
			uint16_t ypoint = (y-_min_y[range_id]) * _scale_y[range_id];
			//sprintf(xandy,"min x=%u,y=%u",_x_org,_y_org+ypoint);
			//Serial.println(xandy);
			_gfx->drawLine(_x_org, _y_org+ypoint, _x_org+_x_width, _y_org+ypoint, _y_range_minor_div_color[range_id]);
		}
	}
	for(long index=start_x; index<=stop_x; index+=step_x) {
		long idx = (int) (index - _start_x) * _scale_x;
		if(((index-start_x)%_x_major_div)==0) {
			_gfx->drawLine(_x_org+idx, _y_org, _x_org+idx, _y_org + _y_height, _x_major_div_color);
		} else if(((index-start_x)%_x_minor_div)==0) {
			_gfx->drawLine(_x_org+idx, _y_org, _x_org+idx, _y_org + _y_height, _x_minor_div_color);
		}
	}
	for(long index=start_x; index<=stop_x; index+=_x_major_div) {
		long idx = (int) (index - _start_x) * _scale_x;
        _gfx->setRotation(1);
		if((index%_x_major_div)==0) {
			_gfx->setFont(ARIAL_12);
			_gfx->setTextSize(1);
			_gfx->setTextColor(_y_range_color[range_id]);
			_gfx->setCursor(_x_org+idx +5, _y_org + _y_height-25);
			char xVal[8];
			if(index!=stop_x) {
			  sprintf(xVal,"%05ld",index/1000);
			  _gfx->print(xVal);
			}
		}
	}
	_last_x[range_id] = LAST_VALUE_STARTUP;
	_last_y[range_id] = LAST_VALUE_STARTUP;
}
