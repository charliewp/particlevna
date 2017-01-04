/***************************************************
  DDS9851

  Written by Charlie Price/ICCD for Fun and Enjoyment.
  MIT license, all text above must be included in any redistribution
 ****************************************************/

#include "DDS9851.h"

#ifdef SPARK  //Only defined if a Particle device
  #include "application.h"
  //#include "fast_pin.h"
  //STM32_Pin_Info* PIN_MAP = HAL_Pin_Map(); // Pointer required for highest access speed
#if (PLATFORM_ID == 0)  // Core

#elif (PLATFORM_ID == 6) // Photon
#else
  #error "*** PLATFORM_ID not supported by this library. PLATFORM should be Core or Photon ***"
#endif
#endif

WsprTx::WsprTx(DDS9851 *dds, DdsUIWidget *ddsWidget) {
		_dds = dds;
		_ddsWidget = ddsWidget;
}
void WsprTx::sendBeacon(uint8_t band) {
	  switch(band) {
		  case BUTTON_BAND_160: wsprTransmitFrequency = WSPR_TX_160M; break;
          case BUTTON_BAND_80:  wsprTransmitFrequency = WSPR_TX_80M; break;
          case BUTTON_BAND_60:  wsprTransmitFrequency = WSPR_TX_60M; break;
          case BUTTON_BAND_40:  wsprTransmitFrequency = WSPR_TX_40M; break;
          case BUTTON_BAND_30:  wsprTransmitFrequency = WSPR_TX_30M; break;
          case BUTTON_BAND_20:  wsprTransmitFrequency = WSPR_TX_20M; break;
          case BUTTON_BAND_17:  wsprTransmitFrequency = WSPR_TX_17M; break;
          case BUTTON_BAND_15:  wsprTransmitFrequency = WSPR_TX_15M; break;
          case BUTTON_BAND_12:  wsprTransmitFrequency = WSPR_TX_12M; break;
          case BUTTON_BAND_10:  wsprTransmitFrequency = WSPR_TX_10M; break;
          case BUTTON_BAND_6:   wsprTransmitFrequency = WSPR_TX_6M; break;
		  default: wsprTransmitFrequency = (WSPR_TX_40M+DDS_OSET + random(10, 190)); break;
	  }
	  wsprTransmitFrequency += DDS_OSET + random(10, 190);
	  for (int i=0; i<162; i++) {
		_dds->sendFrequency(wsprTransmitFrequency + WSPR_DATA[i] * 2, DDS_ENABLE);
		_ddsWidget->setFrequency(wsprTransmitFrequency + WSPR_DATA[i] * 2);
		_ddsWidget->updateDisplay();
        delay(682);
      }
	  _dds->sendFrequency((wsprTransmitFrequency), DDS_DISABLE);
}

DDS9851::DDS9851(uint8_t load, uint8_t clock, uint8_t data) {
    _load = load;
	_clock = clock;
	_data = data;
}

void DDS9851::begin() {
    pinMode(_load, OUTPUT);
	digitalWrite(_load, LOW);
	pinMode(_clock, OUTPUT);
	digitalWrite(_clock, LOW);
	pinMode(_data, OUTPUT);
	digitalWrite(_data, LOW);
	delay(1);
	pulseHigh(_clock);
	digitalWrite(_load, HIGH);
	delay(2);
	for (int b=0; b<4; b++) {
      tfr_byte(0);
    }
	tfr_byte(0x01);
	pulseHigh(_load);
	sendFrequency(7000000, DDS_DISABLE);
}
void DDS9851::tfr_byte(byte data) {
  for (int i=0; i<8; i++, data>>=1) {
    digitalWrite(_data, data & 0x01);
    pulseHigh(_clock);   //after each bit sent, CLK is pulsed high
  }
}
void DDS9851::sendFrequency(long frequencyHz, uint8_t enable) {
	/*
	10MHz example:
	            Power-Down  b[2] 1=power-down, 0=power-up
	       ppppp  6x-REFCLK b[0] 1=enabled
		   43210| |
	W0  =  00001001   //phase_word(5b), control_bits(3b)
    W1  =  00001110
    W2  =  00111000
    W3  =  11100011
    W4  =  10001110
    tuning_word=0x0E38E38E=238609294
	phase_word 5b in MSB of W0, 11.25degree steps
    2^32 = 4294967296
    FoutMHz=(tuning_word)*SysClockMHz/4294967296
    tuning_word=foutMHz*4294967296/SysClockMHz
	           =foutHz*4294.967295/SysClockMHz
    */
  int32_t tuning_word = 6.0 * (frequencyHz * 4294.967296)/180.0;  // note 125 MHz clock on 9850, 180MHz on 9851
  for (int b=0; b<4; b++, tuning_word>>=8) {
    tfr_byte(tuning_word & 0xFF);
  }
  uint16_t control_word = 0x04;
  if(enable==DDS_ENABLE)
    control_word &= 0xFB;
  tfr_byte(control_word);   // Final control byte, all 0 for 9850 chip
  pulseHigh(_load);  // Done!  Should see output
}

/*
void DDS9851::setFrequencyHz(uint freqHz) {
	uint mask = 0x80000000;
	for(int b=0; b<32; b++) {
		digitalWrite(_clock, LOW);
		if((freqHz & mask)>0)
		  digitalWrite(_data, HIGH);
	    else
		  digitalWrite(_data, LOW);
	    digitalWrite(_clock, HIGH);
		mask = mask >> 1;
	}
	digitalWrite(_load, LOW);
	digitalWrite(_load, HIGH);
}
*/

void DdsUIWidget::attach(Adafruit_HX8357 *gfx, DDS9851 *dds, uint16_t x_org, uint16_t y_org, uint16_t text_color, uint16_t background_color) {
	_gfx = gfx;
	_dds = dds;
	_x_org = x_org;
	_y_org = y_org;
	_text_color = text_color;
	_background_color = background_color;
	_isRunning = false;
	_centerFrequencyHz = 7050000;
	_scanWidthHz       = 100000;
	_startFrequencyHz = _centerFrequencyHz - (_scanWidthHz/2);
	_stopFrequencyHz =  _centerFrequencyHz + (_scanWidthHz/2);
	_stepFrequencyHz =  1000; // stepFrequencyHz;
	_currentFrequencyHz = _startFrequencyHz;
	_gfx->setFont(EDIT_FONT);
    _gfx->setTextSize(1);
    _gfx->setCursor(_x_org + 8 *DDS_DIGIT_PIXEL_WIDTH+2, _y_org);
	_gfx->setTextColor(_text_color);
	_gfx->print("Hz");
	updateDisplay();
}
void DdsUIWidget::setScan(long startFrequencyHz, long stopFrequencyHz, long stepFrequencyHz) {
	_startFrequencyHz = startFrequencyHz;
	_stopFrequencyHz = stopFrequencyHz;
	_stepFrequencyHz = stepFrequencyHz;
	_scanWidthHz = _stopFrequencyHz - _startFrequencyHz;
	_centerFrequencyHz = (_startFrequencyHz + _stopFrequencyHz)/2;
}
void DdsUIWidget::scanAndPlot(XyPlot *plot, long startFrequencyHz, long stopFrequencyHz, long stepFrequencyHz) {
	_startFrequencyHz = startFrequencyHz;
	_stopFrequencyHz = stopFrequencyHz;
	_stepFrequencyHz = stepFrequencyHz;
	for(long freq=startFrequencyHz; freq<=stopFrequencyHz; freq+=stepFrequencyHz) {
		uint16_t aValue = analogRead(A0);
		plot->plotXY(0,freq,aValue);
		_currentFrequencyHz = freq;
		updateDisplay();
	}
}
long DdsUIWidget::getStartFrequencyHz() {
	return _startFrequencyHz;
}
long DdsUIWidget::getStopFrequencyHz() {
	return _stopFrequencyHz;
}
long DdsUIWidget::getStepFrequencyHz() {
	return _stepFrequencyHz;
}
void DdsUIWidget::scanAndPlotIt(XyPlot *plot) {
	_startFrequencyHz = _centerFrequencyHz - (_scanWidthHz/2);
	_stopFrequencyHz =  _centerFrequencyHz + (_scanWidthHz/2);
	for(long freq=_startFrequencyHz; freq<=_stopFrequencyHz; freq+=_stepFrequencyHz) {
		uint16_t aValue = analogRead(A0);
		_dds->sendFrequency(freq, DDS_ENABLE);
		plot->plotXY(0,freq,aValue);
		_currentFrequencyHz = freq;
		updateDisplay();
	}
	_dds->sendFrequency(7000000, DDS_DISABLE);
}
void DdsUIWidget::scanAndPlotIt(PolarPlot *plot) {
	_startFrequencyHz = _centerFrequencyHz - (_scanWidthHz/2);
	_stopFrequencyHz =  _centerFrequencyHz + (_scanWidthHz/2);
	for(long freq=_startFrequencyHz; freq<=_stopFrequencyHz; freq+=_stepFrequencyHz) {
		uint16_t aValue = analogRead(A0);
		_dds->sendFrequency(freq, DDS_ENABLE);
		plot->plotThetaAndR(0,freq,aValue);
		_currentFrequencyHz = freq;
		updateDisplay();
	}
	_dds->sendFrequency(7000000, DDS_DISABLE);
}
void DdsUIWidget::startScan() {
	_currentFrequencyHz = _startFrequencyHz;
	_lastDisplayFrequencyHz = 99999999;
	_isRunning = true;
}
void DdsUIWidget::poll() {
	if(_isRunning) {
	  if(_currentFrequencyHz <= (_stopFrequencyHz - _stepFrequencyHz)) {
	     _currentFrequencyHz += _stepFrequencyHz;
	  } else {
         _currentFrequencyHz = _startFrequencyHz;
	     _isRunning = false;
	  }
	  updateDisplay();
	}
}
boolean DdsUIWidget::isRunning() {
	return _isRunning;
}

void DdsUIWidget::editFrequency(int whichFreq, char c) {
	switch(c) {
		case BUTTON_CMD_CF:
		case BUTTON_CMD_ST:
		case BUTTON_CMD_BW: {
			for(int n=EDIT_Y; n<EDIT_Y+15; n++)
			  _gfx->drawFastHLine(320, n, 160, ILI9341_BLACK);
			_whichDigit=0;
			for(int n=0; n<8; n++) {
				_freq[n]=48;
			    _lastDisplayFreq[n]=48;
			}
			_freq[8]=0;
			_lastDisplayFreq[8]=0;
			_gfx->setFont(EDIT_FONT);
            _gfx->setTextSize(1);
            _gfx->setCursor(EDIT_X, EDIT_Y);
	        _gfx->setTextColor(EDIT_COLOR);
			for(int n=0; n<8; n++) {
				int cursorX = EDIT_X + n * DDS_DIGIT_PIXEL_WIDTH;
				_gfx->setCursor(cursorX, EDIT_Y);
				_gfx->setTextColor(EDIT_COLOR);
				_gfx->write(_freq[n]);
			}
			_gfx->setCursor(450, EDIT_Y);
	        _gfx->setTextColor(EDIT_COLOR);
			switch(c) {
			  case BUTTON_CMD_CF:	_gfx->print("CF"); break;
		      case BUTTON_CMD_ST:	_gfx->print("ST"); break;
		      case BUTTON_CMD_BW:	_gfx->print("BW"); break;
			}
			break;
		}
		case BUTTON_CMD_SCAN: Serial.println("SCAN"); break;
		case BUTTON_CMD_DECIMAL: {
			switch(whichFreq) {
				case FREQUENCY_CENTER_HZ:	_centerFrequencyHz = atol(_freq); break;
				case FREQUENCY_SPAN_HZ:		_scanWidthHz = atol(_freq); break;
				case FREQUENCY_STEP_HZ:		_stepFrequencyHz = atol(_freq); Serial.print("ST="); Serial.println(_stepFrequencyHz); break;
			}
			_startFrequencyHz = _centerFrequencyHz - (_scanWidthHz/2);
	        _stopFrequencyHz =  _centerFrequencyHz + (_scanWidthHz/2);
			for(int n=EDIT_Y; n<EDIT_Y+15; n++)
			  _gfx->drawFastHLine(EDIT_X, n, 160, ILI9341_BLACK);
			break;
		}
		case BUTTON_CMD_BACK: break;
		default: {
			for(int n=0; n<7; n++)
               _freq[n] = _freq[n+1];
			_freq[7]=c;
            _gfx->setFont(EDIT_FONT);
            _gfx->setTextSize(1);
            _gfx->setCursor(EDIT_X, EDIT_Y);
	        _gfx->setTextColor(EDIT_COLOR);
			for(int n=0; n<8; n++) {
				int cursorX = EDIT_X + n * DDS_DIGIT_PIXEL_WIDTH;
				if(_freq[n]!=_lastDisplayFreq[n]) {
				  _gfx->setCursor(cursorX, EDIT_Y);
				  _gfx->setTextColor(EDIT_BACKGROUND);
				  _gfx->write(_lastDisplayFreq[n]);
				  _gfx->setTextColor(EDIT_COLOR);
                  _gfx->setCursor(cursorX, EDIT_Y);
				  _gfx->write(_freq[n]);
				  _lastDisplayFreq[n]=_freq[n];
				}
			}
			_whichDigit++;
			Serial.print(c);Serial.print(" ");Serial.println(_freq);
			break;
		}
	}
}
void DdsUIWidget::setFrequency(long freq) {
	_currentFrequencyHz = freq;
}
void DdsUIWidget::updateDisplay() {
	char currentFreq[10];
	char lastFreq[10];
	sprintf(currentFreq,"%08lu",_currentFrequencyHz);
	sprintf(lastFreq,"%08lu",_lastDisplayFrequencyHz);
	_gfx->setFont(SOURCECODEPRO_22);
	//_gfx->setFont(ARIAL_12);
    _gfx->setTextSize(1);
    for(int n=0; n<8; n++) {
	  int cursorX = _x_org + (n) * DDS_DIGIT_PIXEL_WIDTH;
	  if(currentFreq[n]!=lastFreq[n]) {
		_gfx->setCursor(cursorX, _y_org);
		_gfx->setTextColor(_background_color);
		_gfx->write(lastFreq[n]);
	  }
	  _gfx->setCursor(cursorX, _y_org);
	  _gfx->setTextColor(_text_color);
	  _gfx->write(currentFreq[n]);
	}

   _lastDisplayFrequencyHz = _currentFrequencyHz;
}
