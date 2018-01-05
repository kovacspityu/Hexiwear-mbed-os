#include "SSD1351_Fonts.h"


const Character minimalAlphabet5[] = {

};

const uint8_t minimalWidth5[] = {

};

const uint8_t minimalHeight5[] = {

};

const int8_t minimalxPosition5[] = {

};

const int8_t minimalyPosition5[] = {

};


const Character minimalAlphabet7[] = {

};

const uint8_t minimalWidth7[] = {

};

const uint8_t minimalHeight7[] = {

};

const int8_t minimalxPosition7[] = {

};

const int8_t minimalyPosition7[] = {

};

Font minimalFont5 = Font(minimalAlphabet5, minimalWidth5, minimalHeight5, minimalxPosition5, minimalyPosition5);
Font minimalFont7 = Font(minimalAlphabet7, minimalWidth7, minimalHeight7, minimalxPosition7, minimalyPosition7);

Font* minimalF[2] = {&minimalFont5, &minimalFont7};
FontList minimalFont = FontList(2, minimalF);









FontDatabase fontDatabase = &minimalFont;


