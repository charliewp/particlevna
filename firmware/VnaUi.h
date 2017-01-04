/***************************************************
  VnaUi - Vector Network Analyzer User Interface
  
  Written by Charlie Price/ICCD for Fun and Enjoyment.
  MIT license, all text above must be included in any redistribution
 ****************************************************/


#ifndef VnaUi_h
#define VnaUi_h
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
#include "Adafruit_mfGFX.h"
#include "Adafruit_HX8357.h"
#include "Res4WireTouchScreen.h"
#include "fonts.h"


#define EEPROM_ADDR 0x50
#define _I2C_WRITE write
#define _I2C_READ  read

//RGB565 COLOR DEFINITIONS
#define ILI9341_BLACK       0x0000
#define ILI9341_NAVY        0x000F
#define ILI9341_DARKGREEN   0x03E0
#define ILI9341_DARKCYAN    0x03EF
#define ILI9341_MAROON      0x7800
#define ILI9341_PURPLE      0x780F
#define ILI9341_OLIVE       0x7BE0
#define ILI9341_LIGHTGREY   0xC618
#define ILI9341_DARKGREY    0x7BEF
#define ILI9341_BLUE        0x001F
#define ILI9341_GREEN       0x07E0
#define ILI9341_CYAN        0x07FF
#define ILI9341_RED         0xF800
#define ILI9341_MAGENTA     0xF81F
#define ILI9341_YELLOW      0xFFE0
#define ILI9341_WHITE       0xFFFF
#define ILI9341_ORANGE      0xFD20
#define ILI9341_GREENYELLOW 0xAFE5
#define ILI9341_PINK        0xF81F
#define NIXIE_BACKGROUND    0x2222

#define RGB565_NEONORANGE    0xFE4F
#define BUTTON_W 48
#define BUTTON_H 48
#define BUTTON_MARGIN 2
#define BUTTON_SPACING_X 2
#define BUTTON_SPACING_Y 2
#define BUTTON_TEXTSIZE 1

//#define BUTTON_OUTLINE_COLOR		ILI9341_LIGHTGREY 
//#define BUTTON_FILL_COLOR			ILI9341_BLACK 
//#define BUTTON_TEXT_COLOR			ILI9341_LIGHTGREY
#define BUTTON_FILL_PRESSED_COLOR	ILI9341_ORANGE
#define BUTTON_TEXT_PRESSED_COLOR	ILI9341_ORANGE

#define BUTTON_ID_CENTERFREQUENCY	0
#define BUTTON_ID_BANDWIDTH			1
#define BUTTON_ID_KEYPAD_0			2
#define BUTTON_ID_KEYPAD_1			3
#define BUTTON_ID_KEYPAD_2			4
#define BUTTON_ID_KEYPAD_3			5
#define BUTTON_ID_KEYPAD_4			6
#define BUTTON_ID_KEYPAD_5			7
#define BUTTON_ID_KEYPAD_6			8
#define BUTTON_ID_KEYPAD_7			9
#define BUTTON_ID_KEYPAD_8			10
#define BUTTON_ID_KEYPAD_9			11
#define BUTTON_ID_KEYPAD_DECIMAL	12
#define BUTTON_ID_KEYPAD_MENU		13
#define BUTTON_ID_KEYPAD_SCAN		14
#define BUTTON_ID_KEYPAD_STEP		15
#define BUTTON_ID_KEYPAD_ENTER		16
#define BUTTON_ID_KEYPAD_1K			17
#define BUTTON_ID_KEYPAD_5K			18
#define BUTTON_ID_KEYPAD_10K		19

#define BUTTON_ID_KEYPAD_160M		17
#define BUTTON_ID_KEYPAD_80M		18
#define BUTTON_ID_KEYPAD_60M		19
#define BUTTON_ID_KEYPAD_40M		20
#define BUTTON_ID_KEYPAD_30M		21
#define BUTTON_ID_KEYPAD_20M		22
#define BUTTON_ID_KEYPAD_17M		23
#define BUTTON_ID_KEYPAD_15M		24
#define BUTTON_ID_KEYPAD_12M		25
#define BUTTON_ID_KEYPAD_10M		26
#define BUTTON_ID_KEYPAD_6M			27
#define BUTTON_ID_KEYPAD_CALIBRATE	28

#define BUTTON_ID_KEYPAD_WSPR_160M	29
#define BUTTON_ID_KEYPAD_WSPR_80M	30
#define BUTTON_ID_KEYPAD_WSPR_60M	31
#define BUTTON_ID_KEYPAD_WSPR_40M	32	
#define BUTTON_ID_KEYPAD_WSPR_30M	33
#define BUTTON_ID_KEYPAD_WSPR_20M	34
#define BUTTON_ID_KEYPAD_WSPR_17M	35
#define BUTTON_ID_KEYPAD_WSPR_15M	36
#define BUTTON_ID_KEYPAD_WSPR_12M	37
#define BUTTON_ID_KEYPAD_WSPR_10M	38
#define BUTTON_ID_KEYPAD_WSPR_6M	39

#define ARIAL_8				0
#define FUTURA_14   		1
#define DIGITAL7_20			2
#define DIGITAL7_18			3
#define SOURCECODEPRO_22	4
#define _5X7_11             5
#define ARIAL_9             6
#define ARIAL_10            7
#define ARIAL_11            8
#define ARIAL_12            9

#define R_1 1
#define R_2 54
#define R_3 107
#define R_4 160
#define R_5 213
#define R_6 266

#define C_1 2
#define C_2 55
#define C_3 108
#define C_4 161
#define C_5 214
#define C_6 267
#define C_7 320
#define C_8 373
#define C_9 426

#define B_W 51
#define B_H 51
#define B_OS_X 1
#define B_OS_Y 1

#define ASCII_0 48
#define ASCII_1 49
#define ASCII_2 50
#define ASCII_3 51
#define ASCII_4 52
#define ASCII_5 53
#define ASCII_6 54
#define ASCII_7 55
#define ASCII_8 56
#define ASCII_9 57


class StaticVars {
  public:
    static void EEPROM_write(uint16_t addr,uint8_t data);
    static byte EEPROM_read(uint16_t addr);	
    
	//Navigation Variables
	static uint8_t targetPage;	
};

#define BUTTON_TYPE_NULL 	99
#define BUTTON_TYPE_NAV   	0
#define BUTTON_TYPE_CALL  	1

#define BUTTON_TYPE_CF		0
#define BUTTON_TYPE_BW		1
#define BUTTON_TYPE_SCAN	2
#define BUTTON_TYPE_NUMBER	3
#define BUTTON_TYPE_DECIMAL	4
#define BUTTON_TYPE_BACK	5
#define BUTTON_TYPE_ST		6
#define BUTTON_TYPE_CMD		7
#define BUTTON_TYPE_BAND	8
#define BUTTON_TYPE_MENU	9
#define SWIPEBAR_TYPE_NULL	10
#define SWIPEBAR_TYPE_FOCUS 11
#define BUTTON_TYPE_WSPR	12

#define BUTTON_CMD_CF		0
#define BUTTON_CMD_BW		1
#define BUTTON_CMD_SCAN		2
#define BUTTON_CMD_DECIMAL	3
#define BUTTON_CMD_BACK		4
#define BUTTON_CMD_ST		5
#define BUTTON_CMD_ENT		6
#define BUTTON_CMD_MENU		7
#define BUTTON_CMD_1K		8
#define BUTTON_CMD_5K		9
#define BUTTON_CMD_10K		10
#define BUTTON_CMD_CAL		11

#define BUTTON_BAND_160		0
#define BUTTON_BAND_80		1
#define BUTTON_BAND_60		2
#define BUTTON_BAND_40		3
#define BUTTON_BAND_30		4
#define BUTTON_BAND_20		5
#define BUTTON_BAND_17		6
#define BUTTON_BAND_15		7
#define BUTTON_BAND_12		8
#define BUTTON_BAND_10		9
#define BUTTON_BAND_6		10

  
#define NUMBER_0	48
#define NUMBER_1	49
#define NUMBER_2	50
#define NUMBER_3	51
#define NUMBER_4	52
#define NUMBER_5	53
#define NUMBER_6	54
#define NUMBER_7	55
#define NUMBER_8	56
#define NUMBER_9	57

#define CF_ICON_X1	19
#define CF_ICON_Y1	45+53
#define CF_ICON_X2	34
#define CF_ICON_Y2	45+53
#define CF_ICON_X3	27
#define CF_ICON_Y3	45+53
#define CF_ICON_X4	27
#define CF_ICON_Y4	35+53

#define BW_ICON_X1	19
#define BW_ICON_Y1	60+53
#define BW_ICON_X2	21
#define BW_ICON_Y2	60+53
#define BW_ICON_X3	23
#define BW_ICON_Y3	50+53
#define BW_ICON_X4	31
#define BW_ICON_Y4	50+53
#define BW_ICON_X5	32
#define BW_ICON_Y5	60+53
#define BW_ICON_X6  34
#define BW_ICON_Y6	60+53

#define ST_ICON_X1	20
#define ST_ICON_Y1	74+53
#define ST_ICON_X2	25
#define ST_ICON_Y2	74+53
#define ST_ICON_X3	25
#define ST_ICON_Y3	64+53
#define ST_ICON_X4	30
#define ST_ICON_Y4	64+53
#define LEGEND_X		10
#define LEGEND_Y		65
#define LEGEND_W		110
#define LEGEND_H		63
#define CF_TEXT_X	37
#define CF_TEXT_Y	35+53
#define BW_TEXT_X	37
#define BW_TEXT_Y	50+53
#define ST_TEXT_X	37
#define ST_TEXT_Y	64+53

#define PLOT_WIDTH	310
#define PLOT_HEIGHT	264
//#define PLOT_RADIUS 154
#define XYPLOT_X_WIDTH		315
#define XYPLOT_Y_HEIGHT		315
#define XYPLOT_MAX_Y_RANGES	3
#define XYPLOT_MAX_Y_LABEL_LENGTH 8

#define POLARPLOT_RADIUS		132
#define POLARPLOT_RADIUS_POINTS	360
#define POLARPLOT_Y_HEIGHT		315
#define POLARPLOT_MAX_R_RANGES	3
#define POLARPLOT_MAX_R_LABEL_LENGTH 8

#define POLARPLOT_AREA_START_X 0
#define POLARPLOT_AREA_STOP_X  312
#define POLARPLOT_AREA_START_Y 54
#define POLARPLOT_AREA_STOP_Y  319

#define XYPLOT_AREA_START_X 0
#define XYPLOT_AREA_STOP_X  312
#define XYPLOT_AREA_START_Y 54
#define XYPLOT_AREA_STOP_Y  319


#define DDS_WIDGET_X	4
#define DDS_WIDGET_Y	4
#define DDS_DIGIT_PIXEL_WIDTH	15

#define VALUE_X 4
#define VALUE_Y 30

#define FREQUENCY_CENTER_HZ 0
#define FREQUENCY_SPAN_HZ   1
#define FREQUENCY_STEP_HZ   2

#define BUTTON_ID				0
#define BUTTON_XORG 			1
#define BUTTON_YORG			2
#define BUTTON_WIDTH			3 
#define BUTTON_HEIGHT			4
#define BUTTON_OUTLINE_COLOR	5
#define BUTTON_FILL_COLOR		6
#define BUTTON_TEXT_COLOR		7
#define BUTTON_LABEL			8
#define BUTTON_TYPE			9
#define BUTTON_VALUE			10
#define BUTTON_MAX_VALUE		11
#define BUTTON_BASE_ADDR		12	
#define BUTTON_LABEL_FONT		13
#define BUTTON_VALUE_FONT		14

#define EDIT_X C_7
#define EDIT_Y 5

#define MAX_LABEL_SIZE 5

#define KEYPAD_MAX_BUTTONS 20


class ButtonDefinition  {
  public:
    ButtonDefinition();
	void parseCSV(String s);	
  public:
    uint16_t _buttonId;
    uint16_t _buttonXorg;
	uint16_t _buttonYorg;
	uint16_t _buttonWidth;
	uint16_t _buttonHeight;
	uint16_t _buttonOutlineColor;
	uint16_t _buttonFillColor;
	uint16_t _buttonTextColor;
	char	 _buttonLabel[MAX_LABEL_SIZE];
	uint16_t _buttonType;
	uint16_t _buttonValue;
	uint16_t _buttonMaxValue;
	uint16_t _buttonBaseAddr;
	uint16_t _buttonLabelFont;
	uint16_t _buttonValueFont; 
};

#define _MAX_BUTTONDEF_COUNT 16

class ButtonDefinitionSet {
  public:
    ButtonDefinitionSet();
	void addPageBanner(String s);
	void addButtonDefinition(ButtonDefinition def);
	uint8_t getButtonDefinitionCount();
	ButtonDefinition getButtonDefinition(int index);
	char _banner[32];
  private:    
	ButtonDefinition _buttonDefinition[_MAX_BUTTONDEF_COUNT];
  protected:
    uint8_t _buttonDefinitionCount;
};

class Vna_GFX_Button {
 public:
  Vna_GFX_Button(void);
  
  void initButton(Adafruit_HX8357 *gfx,
					  uint8_t id, 
					  int16_t x, int16_t y, 
					  uint8_t w, uint16_t h, 
					  uint16_t outline, uint16_t fill, 
					  uint16_t textcolor,
					  char *label, uint8_t type, uint8_t position, uint8_t max, uint16_t baseAddr, uint8_t labelfont, uint8_t valuefont);
  void initButton(Adafruit_HX8357 *gfx, ButtonDefinition bdef);			  
  void drawButton(boolean inverted = false);
  void setButtonPosition(uint8_t val);
  void setValueText(char* text);
  uint8_t getButtonPosition();
  uint16_t getBaseAddr();
  uint8_t getMax();
  uint8_t getId();
  uint8_t getType();
  uint8_t getPosition();
  boolean contains(int16_t x, int16_t y);
  
  void press(boolean p);
  boolean isPressed();
  boolean justPressed();
  boolean justReleased();
  boolean isIncrement();
  
 private:
  Adafruit_HX8357 *_gfx;
  char _valueText[16];
  int16_t _x, _y;
  uint16_t _w, _h;
  uint8_t _textsize;
  uint16_t _outlinecolor, _fillcolor, _textcolor;
  boolean _isIncremented;
  char _label[16];
  uint8_t _mode;
  uint8_t _position;
  uint8_t _max;
  uint8_t _type;
  uint8_t _id;
  uint16_t _baseAddr;
  boolean currstate, laststate;
  uint8_t _labelFont;
  uint8_t _valueFont;
};

class PageAction {
  public:
    //PageAction();
    PageAction(uint8_t type, uint8_t value);
	virtual ~PageAction();
	uint8_t getType();
	uint8_t getValue();
	uint8_t getCommand();
	void setTypeAndAction(uint8_t type, uint8_t value);
  protected:
    uint8_t _type;
	uint8_t _value;
	uint8_t _action;
};

class Page {
  public:
    Page(uint8_t pageId);
	virtual void attach(Adafruit_HX8357 *gfx, ButtonDefinitionSet bset);
	virtual void addButton(Vna_GFX_Button button);
    virtual PageAction contains(int16_t x, int16_t y);
    uint8_t getPageId();
	virtual void show();
  protected:
    uint8_t _pageId;
    uint8_t _id;
};

//#define NAV_NOP     0
//#define PAGE_HOME 	0

class KeyPad  : public Page {
  public:
    KeyPad(uint8_t pageId);
	void attach(Adafruit_HX8357 *gfx);
	void attach(Adafruit_HX8357 *gfx, ButtonDefinitionSet bset);
	void addButton(Vna_GFX_Button button);
	PageAction contains(int16_t x, int16_t y);
	void clearRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
    void show();
  private:
    Adafruit_HX8357 *_gfx;
	uint8_t _buttonIds[KEYPAD_MAX_BUTTONS];	
	uint8_t _numberOfButtons;
	char _keypadLabel[32];
    Vna_GFX_Button _buttons[KEYPAD_MAX_BUTTONS];
};
#define SPEED_OF_LIGHT_METERSPERSECOND 300000000
#define MAX_MARKERS 20
#define SWIPEBAR_FREQ 		0
#define SWIPEBAR_WAVELENGTH 1
class SwipeBar{
  public:
	void attach(Adafruit_HX8357 *gfx, uint16_t x_org, uint16_t y_org, uint16_t width, uint16_t height, uint8_t type, long deviation, long markerSpacing, uint8_t text_division, char *fmt, uint16_t fmtdiv, uint16_t ratio, uint16_t background_color, uint16_t indicator_color, uint16_t cfmarker_color) ;
	void show();
	void setPosition(long position);
	void changePosition(long changePosition);
	void clearRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
	long getPosition();
	PageAction contains(int16_t x, int16_t y);
  private:
    Adafruit_HX8357 *_gfx;
	char _fmt[16];
	uint8_t _type;
	uint16_t _fmtdiv;
	uint16_t _width;
    uint16_t _height;
	uint16_t _x_org;
    uint16_t _y_org;
    uint16_t _background_color;
	uint16_t _indicator_color;
	uint16_t _cfmarker_color;
	uint16_t _ratio;
	long _markerSpacing;
	long _deviation;
	long _position;
	uint8_t _text_division;
	uint16_t _lastMarker[MAX_MARKERS];
	char _lastDisplayFrequency[MAX_MARKERS][16];
};	

class XyPlot {
  public:
	void attach(Adafruit_HX8357 *gfx, uint16_t x_org, uint16_t y_org, uint16_t x_width, uint16_t y_height, uint16_t background_color);
	void setXRange(long start_x, long stop_x, long minor_div, long major_div, uint16_t minor_div_color, uint16_t major_div_color);
	void addYRange(const char *label, uint8_t range_id, long min_y,   long max_y,  long minor_div, long major_div, uint16_t range_color, uint16_t minor_div_color, uint16_t major_div_color);
	void setXY(uint8_t range_id, long x_value, long y_value);
	void plotXY(uint8_t range_id, long x_value, long y_value);
	void fill(uint8_t range_id);
    void show(uint8_t range_id, long start_x, long stop_x, long step_x);
	void showGraticule(uint8_t range_id, long start_x, long stop_x, long step_x);
	void showLegendBlock(uint8_t range_id, long start_x, long stop_x, long step_x);
	void clearRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
	uint16_t getYInfo(uint8_t range_id, long x_value);
  private:
    Adafruit_HX8357 *_gfx;
	uint16_t _y_values[XYPLOT_MAX_Y_RANGES][XYPLOT_X_WIDTH];
	uint16_t _x_org;
    uint16_t _y_org;
    uint16_t _x_width;
    uint16_t _y_height;
    uint16_t _background_color;
	double _scale_x;
	long _start_x;
	uint16_t _last_x[XYPLOT_MAX_Y_RANGES];
	uint16_t _last_y[XYPLOT_MAX_Y_RANGES];
	char _label[XYPLOT_MAX_Y_RANGES][XYPLOT_MAX_Y_LABEL_LENGTH];
	long _min_y[XYPLOT_MAX_Y_RANGES];
	long _max_y[XYPLOT_MAX_Y_RANGES];
	double _scale_y[XYPLOT_MAX_Y_RANGES];
	long _x_minor_div;
	long _x_major_div;
	uint16_t _x_minor_div_color;
	uint16_t _x_major_div_color;
	long _y_minor_div[XYPLOT_MAX_Y_RANGES];
	long _y_major_div[XYPLOT_MAX_Y_RANGES];
	long _y_range_color[XYPLOT_MAX_Y_RANGES];
	long _y_range_major_div_color[XYPLOT_MAX_Y_RANGES];
    long _y_range_minor_div_color[XYPLOT_MAX_Y_RANGES];
};
class PolarPlot {
  public:
	void attach(Adafruit_HX8357 *gfx, uint16_t x_c, uint16_t y_c, uint16_t radius, uint16_t background_color);
	void setThetaRange(long hz_per_revolution, long minor_div, long major_div, uint16_t minor_div_color, uint16_t major_div_color);
	void addRRange(const char *label, uint8_t range_id, long min_r_value,   long max_r_value,  long minor_div, long major_div, uint16_t range_color, uint16_t minor_div_color, uint16_t major_div_color);
	void setThetaAndR(uint8_t range_id, long t_value, long r_value);
	void plotThetaAndR(uint8_t range_id, long t_value, long r_value);
	void fill(uint8_t range_id);
    void show(uint8_t range_id, long start_x, long stop_x, long step_x);
	void showGraticule(uint8_t range_id, long step_t);
	void showLegendBlock(uint8_t range_id, long start_x, long stop_x, long step_x);
	void clearPolarPlot();
	uint16_t getTInfo(uint8_t range_id, long t_value);
  private:
    Adafruit_HX8357 *_gfx;
	long _hz_per_revolution;
	uint16_t _x_center;
    uint16_t _y_center;
	uint16_t _radius;
	long _t_minor_div;
	long _t_major_div;
	uint16_t _t_minor_div_color;
	uint16_t _t_major_div_color;
	char _label[POLARPLOT_MAX_R_RANGES][POLARPLOT_MAX_R_LABEL_LENGTH];
	long _min_r[POLARPLOT_MAX_R_RANGES];
	long _max_r[POLARPLOT_MAX_R_RANGES];
	long _r_minor_div[POLARPLOT_MAX_R_RANGES];
	long _r_major_div[POLARPLOT_MAX_R_RANGES];
	long _r_range_color[POLARPLOT_MAX_R_RANGES];
	long _r_range_major_div_color[POLARPLOT_MAX_R_RANGES];
    long _r_range_minor_div_color[POLARPLOT_MAX_R_RANGES];
	double _scale_r[POLARPLOT_MAX_R_RANGES];
	
    uint16_t _r_values[POLARPLOT_MAX_R_RANGES][POLARPLOT_RADIUS_POINTS];
	uint16_t _x_width;
    uint16_t _y_height;
    uint16_t _background_color;
	double _scale_t;
	long _start_t;
	uint16_t _last_x[POLARPLOT_MAX_R_RANGES];
	uint16_t _last_y[POLARPLOT_MAX_R_RANGES];
};	
#endif
