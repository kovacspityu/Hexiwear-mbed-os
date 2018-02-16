#ifndef _SSD1351_FONTS_
#define _SSD1351_FONTS_
#include "mbed.h"

#define NUMBER_OF_FONTS 1
#define FONT_OFFSET 32

enum SSD1351_Fonts{
    FONT_MINIMAL_FONT = 0
};

typedef bool *Character;

struct Font{
    Font(const Character* c, const uint8_t *w, const uint8_t *h, const int8_t *x, const int8_t *y){
        alphabet = c;
        width = w;
        height = h;
        xPosition = x;
        yPosition = y;
    }
    const uint8_t *width, *height;
    const int8_t *xPosition, *yPosition;
    const Character *alphabet;
};

struct FontList{
    FontList(uint8_t s, Font* f){
        sizes = s;
        fontCollection = f;
    }
    const uint8_t sizes;
    const Font* fontCollection;
};

struct FontDatabase{
    FontDatabase(uint8_t s, FontList* f){
        sizes = s;
        fontList = f;
    }
    const uint8_t sizes;
    const FontList * fontList;
}

#endif