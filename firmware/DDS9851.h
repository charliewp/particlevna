/***************************************************
  DDS9851
  
  Written by Charlie Price/ICCD for Fun and Enjoyment.
  MIT license, all text above must be included in any redistribution
 ****************************************************/

#ifndef _DDS9851_H
#define _DDS9851_H

#ifdef ARDUINO
#if ARDUINO >= 100
 #include "Arduino.h"
 #include "Print.h"
#else
 #include "application.h"
#endif
#include <avr/pgmspace.h>
#include "pins_arduino.h"
#include "wiring_private.h"
#define cbi(reg, bitmask) *reg &= ~bitmask
#define sbi(reg, bitmask) *reg |= bitmask
#endif

#ifdef SPARK
#include "application.h"
#define digitalPinToBitMask(pin) (volatile uint32_t)pin
#define portOutputRegister(port) NULL
#define digitalPinToPort(pin) (volatile uint32_t)pin
#define SPARK_SPI_SPEED SPI_CLOCK_DIV4
#endif
#include "Adafruit_mfGFX.h"
#include "Adafruit_HX8357.h"
#include "VnaUi.h"

#define PLOT_X_WIDTH		315
#define PLOT_Y_HEIGHT		315
#define PLOT_MAX_Y_RANGES	3


#define EDIT_COLOR ILI9341_GREEN
#define EDIT_BACKGROUND ILI9341_BLACK
#define EDIT_FONT ARIAL_12

#define SMART_VFO_DIAL_DIGITAL_DX 0
#define SMART_VFO_FONT	   SOURCECODEPRO_22
#define SMART_VFO_DIAL_DIGITAL_CHAR_WIDTH 15

#define pulseHigh(pin) {digitalWrite(pin, HIGH); digitalWrite(pin, LOW); }
 
#define DDS_ENABLE  1
#define DDS_DISABLE 0
class DDS9851 {

 public:
  DDS9851(uint8_t load, uint8_t clock, uint8_t data);
  //void setFrequencyHz(uint freqHz); 
  void begin();
  void tfr_byte(byte data);
  void sendFrequency(long frequencyHz, uint8_t enable);
 private:
  uint8_t _load;
  uint8_t _clock;
  uint8_t _data; 
};
// DDS Offset in Hz
#define DDS_OSET  0  //DDS #2

// WSPR Output Frequency
//#define WSPR_TX_A    10.140100e6  // this is the bottom of the band. The station moves about.
#define WSPR_TX_160M 1.838000e6
#define WSPR_TX_80M  3.594000e6
#define WSPR_TX_60M  5.288600e6
#define WSPR_TX_40M  7.040000e6  // this is the bottom of the band. The station moves about.
#define WSPR_TX_30M  10.140100e6
#define WSPR_TX_20M  14.097000e6
#define WSPR_TX_17M  18.106000e6
#define WSPR_TX_15M  21.096000e6
#define WSPR_TX_12M  28.126000e6
#define WSPR_TX_10M  7.040000e6
#define WSPR_TX_6M  50.294400e6

#define WSPR_DUTY  3 // transmit every N slices.



class DdsUIWidget {
  public:
	void attach(Adafruit_HX8357 *gfx, DDS9851 *_dds, uint16_t x_org, uint16_t y_org, uint16_t text_color, uint16_t background_color);
	void setScan(long startFrequencyHz, long stopFrequencyHz, long stepFrequencyHz);
	boolean isRunning();
	void poll();
	void updateDisplay();
	void startScan();
	void scanAndPlot(XyPlot *plot, long startFrequencyHz, long stopFrequencyHz, long stepFrequencyHz);
	void scanAndPlotIt(XyPlot *plot);
	void scanAndPlotIt(PolarPlot *plot);
	void editFrequency(int whichFreq, char c);
	void editScanWidth(char c);
	long getStartFrequencyHz();
	long getStopFrequencyHz();
	long getStepFrequencyHz();
	void setFrequency(long freq);
  private:
    Adafruit_HX8357 *_gfx;
	DDS9851 *_dds;
	XyPlot *_plot;
	boolean _isRunning;
	long _currentFrequencyHz;
	long _lastDisplayFrequencyHz;
	long _centerFrequencyHz;
	long _scanWidthHz;
	long _startFrequencyHz;
	long _stopFrequencyHz;
	long _stepFrequencyHz;
	uint8_t _whichDigit;
	char _freq[9];
	char _lastDisplayFreq[9];
	uint16_t _x_org;
	uint16_t _y_org;
	uint16_t _text_color;
	uint16_t _background_color;
};
class WsprTx {
  public:
	WsprTx(DDS9851 *dds, DdsUIWidget *ddsWidget);
	void sendBeacon(uint8_t band);
  private:
    DDS9851 *_dds;
	DdsUIWidget *_ddsWidget;
	long wsprTransmitFrequency;
	const byte WSPR_DATA[162] = {3,1,0,0,2,0,2,2,1,2,2,2,3,3,1,0,2,2,3,0,0,3,2,1,1,3,3,2,0,0,0,2,0,0,3,2,0,1,0,3,0,0,2,2,2,2,1,0,1,1,2,2,1,1,0,3,2,2,2,3,3,2,1,0,2,0,2,3,1,0,3,0,3,2,1,2,1,0,0,1,0,0,3,0,1,3,2,0,2,1,1,2,1,2,1,2,0,2,3,2,2,0,2,2,1,2,0,3,2,0,1,3,3,2,3,1,0,2,3,3,0,1,0,0,2,1,3,1,2,0,2,2,0,3,0,1,0,0,3,1,0,0,2,2,0,2,0,3,1,0,3,0,3,3,2,0,0,1,1,2,0,0,};
};
#endif