#ifndef _SSD1351_FONTS_
#define _SSD1351_FONTS_
#include "mbed.h"

#define FONT_OFFSET 32


struct Font{
    Font(const void* c, const uint8_t *w, const uint8_t *h, const int8_t *x, const int8_t *y){
        alphabet = *((const bool***) c);
        width = w;
        height = h;
        xPosition = x;
        yPosition = y;
    }
    const uint8_t *width, *height;
    const int8_t *xPosition, *yPosition;
    const bool **alphabet;
};

typedef Font *FontDatabase;

#endif