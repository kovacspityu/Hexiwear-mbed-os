#ifndef _SSD1351_FONTS_
#define _SSD1351_FONTS_
#include "mbed.h"

#define FONT_OFFSET 32
#define NUMBER_OF_CHARACTERS 95


struct Font{
    Font(uint8_t size, const bool *alphabet, const uint8_t width[], const uint8_t height[], const int8_t xPosition[], const int8_t yPosition[]){
        for(uint8_t i=0;i<NUMBER_OF_CHARACTERS;i++){ 
            this->alphabet[i] = alphabet + i*size;
        }
        this->width = width;
        (this->height) = height;
        this->xPosition = xPosition;
        this->yPosition = yPosition;
    }
    const bool *alphabet[NUMBER_OF_CHARACTERS];
    const uint8_t *width, *height;
    const int8_t *xPosition, *yPosition;
};

typedef Font *FontDatabase;

namespace fnt{
    extern const uint8_t fontSizes[];
    extern FontDatabase fontDatabase;
    extern void initializeFonts();
};

#endif