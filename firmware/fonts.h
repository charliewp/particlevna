//
//  fonts.h
//  

#ifndef _fonts_h
#define _fonts_h

//#include "Arduino.h"
#include "application.h"

// Font selection descriptors - Add an entry for each new font and number sequentially
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


#define FONT_START 0
#define FONT_END 1

struct FontDescriptor
{
	uint8_t	width;		// width in bits
	uint8_t	height; 	// char height in bits
	uint16_t offset;	// offset of char into char array
};

// Font references - add pair of references for each new font

extern const uint8_t digital7_18ptBitmaps[];
extern const FontDescriptor digital7_18ptDescriptors[];

extern const uint8_t digital7_20ptBitmaps[];
extern const FontDescriptor digital7_20ptDescriptors[];

extern const uint8_t sourceCodePro_22ptBitmaps[];
extern const FontDescriptor sourceCodePro_22ptDescriptors[];

extern const uint8_t futuraMdBT_14ptBitmaps[];
extern const FontDescriptor futuraMdBT_14ptDescriptors[];

extern const uint8_t arial_8ptBitmaps[];
extern const FontDescriptor arial_8ptDescriptors[];

extern const uint8_t arial_9ptBitmaps[];
extern const FontDescriptor arial_9ptDescriptors[];

extern const uint8_t arial_10ptBitmaps[];
extern const FontDescriptor arial_10ptDescriptors[];

extern const uint8_t arial_11ptBitmaps[];
extern const FontDescriptor arial_11ptDescriptors[];

extern const uint8_t arial_12ptBitmaps[];
extern const FontDescriptor arial_12ptDescriptors[];

extern const uint8_t _5x7practical_11ptBitmaps[];
extern const FontDescriptor _5x7practical_11ptDescriptors[];


#endif
