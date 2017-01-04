// Touch screen library with X Y and Z (pressure) readings as well
// as oversampling to avoid 'bouncing'
// (c) ladyada / adafruit
// Code under MIT License

#include <Adafruit_TFTLCD.h>
#include <Adafruit_mfGFX.h>

#include <stdint.h>

//RGB565 COLOR DEFINITIONS
#define ILI9341_BLACK       0x0000      /*   0,   0,   0 */
#define ILI9341_NAVY        0x000F      /*   0,   0, 128 */
#define ILI9341_DARKGREEN   0x03E0      /*   0, 128,   0 */
#define ILI9341_DARKCYAN    0x03EF      /*   0, 128, 128 */
#define ILI9341_MAROON      0x7800      /* 128,   0,   0 */
#define ILI9341_PURPLE      0x780F      /* 128,   0, 128 */
#define ILI9341_OLIVE       0x7BE0      /* 128, 128,   0 */
#define ILI9341_LIGHTGREY   0xC618      /* 192, 192, 192 */
#define ILI9341_DARKGREY    0x7BEF      /* 128, 128, 128 */
#define ILI9341_BLUE        0x001F      /*   0,   0, 255 */
#define ILI9341_GREEN       0x07E0      /*   0, 255,   0 */
#define ILI9341_CYAN        0x07FF      /*   0, 255, 255 */
#define ILI9341_RED         0xF800      /* 255,   0,   0 */
#define ILI9341_MAGENTA     0xF81F      /* 255,   0, 255 */
#define ILI9341_YELLOW      0xFFE0      /* 255, 255,   0 */
#define ILI9341_WHITE       0xFFFF      /* 255, 255, 255 */
#define ILI9341_ORANGE      0xFD20      /* 255, 165,   0 */
#define ILI9341_GREENYELLOW 0xAFE5      /* 173, 255,  47 */
#define ILI9341_PINK        0xF81F
#define NIXIE_BACKGROUND    0x2222

class SoftButton {

 public:
  SoftButton(void);
  void init(Adafruit_GFX *gfx, int16_t x, int16_t y, 
		      uint8_t w, uint8_t h, 
		      uint16_t outline, uint16_t fill, uint16_t textcolor,
		      uint8_t id, char *label, uint8_t type, uint8_t value, uint8_t max, uint16_t baseAddr, uint8_t textsize);			  
  void draw(boolean inverted = false);
  void setPosition(uint8_t val);
  void setValueText(char* text);
  uint8_t getPosition();
  uint16_t getBaseAddr();
  uint8_t getMax();
  uint8_t getId();
  uint8_t getType();
  boolean contains(int16_t x, int16_t y);
  
  void press(boolean p);
  boolean isPressed();
  boolean justPressed();
  boolean justReleased();
  boolean isIncrement();
  
 private:
  Adafruit_GFX *_gfx;
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
};

class PageAction {
  public:
    //PageAction();
    PageAction(uint8_t type);
	virtual ~PageAction();
	uint8_t getType();
  protected:
    uint8_t _type;
	uint8_t _action;
};

class Page {
  public:
    Page(uint8_t pageId);
    virtual void attach(Adafruit_GFX *gfx);
    virtual PageAction contains(int16_t x, int16_t y);
    virtual void sync();
	uint8_t getPageId();
  protected:
    uint8_t _pageId;
    uint8_t _id;
};
#define NAV_NOP     0
class NavAction : public PageAction {
  public:
    NavAction(uint8_t type);    
	Page* getPage();
};

class GenericPage  : public Page {
  public:
    GenericPage(uint8_t pageId, uint8_t buttonIds[]);
	void attach(Adafruit_GFX *gfx);
	PageAction contains(int16_t x, int16_t y);
	void sync();	
  private:
    Adafruit_GFX *_gfx;
	uint8_t _buttonIds[8];	
    SoftButton _buttons[8];
};

#endif
