#include "SSD1351_Fonts.h"


const bool roboto11Alphabet[][70] = {
    {false, false, false, false, false, false, false, false, false, true, true, true, true, false, false, true}, 
    {false, false, false, false, true, true, false, false, false, false, true, true}, 
    {false, false, false, false, false, false, false, false, true, false, true, false, false, true, true, true, true, true, false, false, true, false, true, false, false, true, true, true, true, true, false, false, true, false, true, false}, 
    {false, false, false, false, false, false, false, false, false, false, false, true, true, false, false, true, true, false, false, true, false, false, true, false, false, true, true, false, false, true, false, false, true, true, true, false}, 
    {false, false, false, false, false, false, true, false, false, true, false, false, true, false, false, false, false, false, true, false, false, true, false, false, true}, 
    {false, false, false, false, false, false, false, false, false, true, false, true, true, false, false, true, false, true, false, false, true, false, true, false, true, false, false, true, false, false, true, false, true, true, false, false, false, false, false, true, false, true}, 
    {false, false, false, false, true, true}, 
    {false, false, false, false, false, false, false, false, false, false, false, false, true, true, true, true, true, true, true, false, false, true, false, false, false, false, false, false, false, true}, 
    {false, false, false, false, false, false, false, false, false, false, false, true, false, false, false, false, false, false, false, true, false, false, true, true, true, true, true, true, true, false}, 
    {false, false, false, false, false, false, false, true, false, true, false, true, true, true, false, false, false, true, false, true}, 
    {false, false, false, false, false, false, false, false, false, true, false, false, false, false, false, true, false, false, false, true, true, true, true, true, false, false, false, true, false, false, false, false, false, true, false, false}, 
    {false, false, false, false, false, false, false, true, false, true, true, false}, 
    {false, false, false, true, false, true, false, true}, 
    {false, false, false, true}, 
    {false, false, false, false, false, false, false, false, false, false, false, false, false, false, true, true, false, false, false, true, true, true, false, false, false, true, true, false, false, false, false, false}, 
    {false, false, false, false, false, false, false, false, false, true, true, true, true, true, true, true, false, true, false, false, false, false, false, true, false, true, true, true, true, true, true, true}, 
    {false, false, false, false, false, false, false, false, false, false, true, false, false, false, false, false, false, true, true, true, true, true, true, true}, 
    {false, false, false, false, false, false, false, false, false, false, true, false, false, false, false, true, false, true, false, false, false, false, true, true, false, true, false, false, true, true, false, true, false, false, true, true, false, false, false, true}, 
    {false, false, false, false, false, false, false, false, false, false, true, false, false, false, true, false, false, true, false, false, false, false, false, true, false, true, false, false, true, false, false, true, false, false, true, true, false, true, true, false}, 
    {false, false, false, false, false, false, false, false, false, false, false, false, false, false, true, false, false, false, false, true, true, true, true, false, false, false, true, false, false, false, true, false, false, true, true, true, true, true, true, true}, 
    {false, false, false, false, false, false, false, false, false, true, true, true, true, false, true, true, false, true, false, false, true, false, false, true, false, true, false, false, true, true, true, true}, 
    {false, false, false, false, false, false, false, false, false, true, true, true, true, true, true, true, false, true, false, true, false, false, false, true, false, false, false, false, true, true, true, true}, 
    {false, false, false, false, false, false, false, false, false, true, false, false, false, false, false, false, false, true, false, false, false, false, false, true, false, true, false, false, true, true, true, false, false, true, true, true, false, false, false, false}, 
    {false, false, false, false, false, false, false, false, false, true, true, true, false, true, true, true, false, true, false, false, true, false, false, true, false, true, true, true, false, true, true, true}, 
    {false, false, false, false, false, false, false, false, false, true, true, true, true, true, false, true, false, true, false, false, false, true, false, true, false, false, true, true, true, true, true, false}, 
    {false, false, false, false, false, false, true, false, false, true}, 
    {false, false, false, false, false, false, false, false, false, false, false, true, false, true, false, true, false, false, true, true, false}, 
    {false, false, false, false, false, false, false, true, true, false, false, false, true, true, false, false, true, false, false, true}, 
    {false, false, false, false, false, true, false, true, false, true, false, true, false, true, false, true}, 
    {false, false, false, false, false, false, true, false, false, true, false, false, true, true, false, false, false, true, true, false}, 
    {false, false, false, false, false, false, false, false, false, true, true, false, false, false, false, false, false, true, false, false, false, true, false, true, false, true, true, true, true, false, false, false}, 
    {false, false, false, false, false, false, false, false, false, false, false, true, true, true, true, false, false, true, true, false, true, true, false, true, false, true, false, true, false, false, true, true, false, true, true, false, true, true, true, false, false, false, false, true, true, false, false, false}, 
    {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true, false, false, false, true, true, true, true, false, false, true, true, false, false, true, false, false, false, false, false, true, true, true, true, false, false, false, false, false, false, false, false, true}, 
    {false, false, false, false, false, false, false, false, false, true, true, true, true, true, true, true, false, true, false, false, true, false, false, true, false, true, true, true, false, true, true, true}, 
    {false, false, false, false, false, false, false, false, false, false, false, true, true, true, false, false, false, true, true, false, false, false, true, true, false, true, false, false, false, false, false, true, false, true, true, false, false, false, true, true}, 
    {false, false, false, false, false, false, false, false, false, true, true, true, true, true, true, true, false, true, false, false, false, false, false, true, false, false, true, true, true, true, true, false}, 
    {false, false, false, false, false, false, false, false, false, true, true, true, true, true, true, true, false, true, false, false, true, false, false, true, false, true, false, false, true, false, false, true}, 
    {false, false, false, false, false, false, false, false, false, true, true, true, true, true, true, true, false, true, false, false, true, false, false, false, false, true, false, false, true, false, false, false}, 
    {false, false, false, false, false, false, false, false, false, false, false, true, true, true, false, false, false, true, true, false, false, false, true, true, false, true, false, false, false, true, false, true, false, true, true, false, false, true, true, true}, 
    {false, false, false, false, false, false, false, false, false, true, true, true, true, true, true, true, false, false, false, false, true, false, false, false, false, true, true, true, true, true, true, true}, 
    {false, false, false, false, false, false, false, false, false, true, false, false, false, false, false, true, false, true, true, true, true, true, true, true, false, true, false, false, false, false, false, true}, 
    {false, false, false, false, false, false, false, false, false, false, false, false, false, false, true, false, false, false, false, false, false, false, false, true, false, false, false, false, false, false, false, true, false, true, true, true, true, true, true, false}, 
    {false, false, false, false, false, false, false, false, false, true, true, true, true, true, true, true, false, false, false, true, true, true, false, false, false, true, true, false, false, false, true, true, false, true, false, false, false, false, false, true}, 
    {false, false, false, false, false, false, false, false, false, true, true, true, true, true, true, true, false, false, false, false, false, false, false, true, false, false, false, false, false, false, false, true}, 
    {false, false, false, false, false, false, false, false, false, true, true, true, true, true, true, true, false, false, true, false, false, false, false, false, false, false, false, true, false, false, false, false, false, false, true, false, false, false, false, false, false, true, true, true, true, true, true, true}, 
    {false, false, false, false, false, false, false, false, false, true, true, true, true, true, true, true, false, false, true, true, true, false, false, false, false, false, false, false, false, true, true, false, false, true, true, true, true, true, true, true}, 
    {false, false, false, false, false, false, false, false, false, false, false, true, true, true, false, false, false, true, true, false, false, false, true, true, false, true, false, false, false, false, false, true, false, true, true, false, false, false, true, true, false, false, false, true, true, true, false, false}, 
    {false, false, false, false, false, false, false, false, false, true, true, true, true, true, true, true, false, true, false, false, false, true, false, false, false, true, true, true, true, true, false, false}, 
    {false, false, false, false, false, false, false, false, false, false, false, true, true, true, false, false, false, true, true, false, false, false, true, true, false, true, false, false, false, true, true, true, false, true, true, false, false, false, true, true, false, false, false, true, true, true, false, true}, 
    {false, false, false, false, false, false, false, false, false, true, true, true, true, true, true, true, false, true, false, false, true, false, false, false, false, true, true, true, false, true, true, true}, 
    {false, false, false, false, false, false, false, false, false, false, false, false, false, false, true, false, false, true, true, true, false, false, false, true, false, true, false, false, true, false, false, true, false, true, true, false, false, true, true, true}, 
    {false, false, false, false, false, false, false, false, false, true, false, false, false, false, false, false, false, true, false, false, false, false, false, false, false, true, true, true, true, true, true, true, false, true, false, false, false, false, false, false, false, true, false, false, false, false, false, false}, 
    {false, false, false, false, false, false, false, false, false, true, true, true, true, true, true, true, false, false, false, false, false, false, false, true, false, true, true, true, true, true, true, true}, 
    {false, false, false, false, false, false, false, false, false, true, false, false, false, false, false, false, false, false, true, true, true, true, false, false, false, false, false, false, false, true, true, true, false, true, true, true, true, false, false, false}, 
    {false, false, false, false, false, false, false, false, false, true, false, false, false, false, false, false, false, true, true, true, true, false, false, false, false, false, false, false, true, true, true, true, false, true, true, true, false, false, false, false, false, false, false, false, true, true, true, true, false, true, true, true, false, false, false, false}, 
    {false, false, false, false, false, false, false, false, false, true, false, false, false, false, false, true, false, true, true, false, false, true, true, false, false, false, false, true, true, true, false, false, false, true, true, false, false, false, true, true, false, true, false, false, false, false, false, true}, 
    {false, false, false, false, false, false, false, false, false, true, false, false, false, false, false, false, false, false, true, true, false, false, false, false, false, false, false, true, true, true, true, true, false, true, true, false, false, false, false, false}, 
    {false, false, false, false, false, false, false, false, false, true, false, false, false, false, true, true, false, true, false, false, true, true, false, true, false, true, false, true, true, false, false, true, false, true, true, false, false, false, false, true}, 
    {false, false, false, false, false, false, false, false, false, false, false, true, true, true, true, true, true, true, true, true, false, true, false, false, false, false, false, false, false, true}, 
    {false, false, false, false, false, false, false, false, false, true, true, false, false, false, false, false, false, false, false, true, true, true, false, false, false, false, false, false, false, false, true, true}, 
    {false, false, false, false, false, false, false, false, false, false, false, true, false, false, false, false, false, false, false, true, false, true, true, true, true, true, true, true, true, true}, 
    {false, false, false, false, false, false, true, true, false, true, true, false, false, false, false, true}, 
    {false, false, false, true, false, true, false, true}, 
    {false, false, false, true}, 
    {false, false, false, false, false, false, false, true, false, true, true, true, false, true, false, true, false, true, false, true, true, true, true, true}, 
    {false, false, false, false, false, false, false, false, false, true, true, true, true, true, true, true, false, false, false, true, false, false, false, true, false, false, false, true, true, true, true, true}, 
    {false, false, false, false, false, false, false, true, true, true, true, true, false, true, false, false, false, true, false, true, true, false, true, true}, 
    {false, false, false, false, false, false, false, false, false, false, false, true, true, true, true, true, false, false, false, true, false, false, false, true, false, true, true, true, true, true, true, true}, 
    {false, false, false, false, false, false, false, true, true, true, true, true, false, true, false, true, false, true, false, true, true, true, false, true}, 
    {false, false, false, false, false, false, false, false, false, false, false, true, false, false, false, false, false, true, true, true, true, true, true, true, false, true, false, true, false, false, false, false}, 
    {false, false, false, false, false, false, false, false, false, true, true, true, true, true, false, true, false, true, false, false, false, true, false, true, false, true, true, true, true, true, true, true}, 
    {false, false, false, false, false, false, false, false, false, true, true, true, true, true, true, true, false, false, false, true, false, false, false, false, false, false, false, true, true, true, true, true}, 
    {false, false, false, false, false, false, false, false, false, false, false, true, false, false, false, true, false, true, false, true, true, true, true, true, false, false, false, false, false, false, false, true}, 
    {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true, false, true, false, true, true, true, true, true, true, true}, 
    {false, false, false, false, false, false, false, false, false, true, true, true, true, true, true, true, false, false, false, false, true, true, true, false, false, false, false, true, false, false, false, true}, 
    {false, false, false, false, false, false, false, false, false, true, false, false, false, false, false, true, false, true, true, true, true, true, true, true, false, false, false, false, false, false, false, true}, 
    {false, false, false, false, false, false, false, true, true, true, true, true, false, true, false, false, false, false, false, true, true, true, true, true, false, true, false, false, false, false, false, true, true, true, true, true}, 
    {false, false, false, false, false, false, false, true, true, true, true, true, false, true, false, false, false, false, false, true, true, true, true, true}, 
    {false, false, false, false, false, false, false, false, true, true, true, false, false, true, false, false, false, true, false, true, false, false, false, true, false, true, true, true, true, true}, 
    {false, false, false, false, false, false, false, false, false, true, true, true, true, true, true, true, false, true, false, false, false, true, false, false, false, true, true, true, true, true, false, false}, 
    {false, false, false, false, false, false, false, false, false, true, true, true, true, true, false, false, false, true, false, false, false, true, false, false, false, true, true, true, true, true, true, true}, 
    {false, false, false, false, false, false, false, true, true, true, true, true, false, true, false, false, false, false, false, true, false, false, false, false}, 
    {false, false, false, false, false, false, false, true, true, false, false, true, false, true, false, true, false, true, false, true, false, false, true, true}, 
    {false, false, false, false, false, false, false, false, false, true, false, false, false, false, false, true, true, true, true, true, true, false, false, true, false, false, false, true}, 
    {false, false, false, false, false, false, false, true, true, true, true, true, false, false, false, false, false, true, false, true, true, true, true, true}, 
    {false, false, false, false, false, false, false, true, false, false, false, false, false, false, true, true, true, false, false, false, false, false, true, true, false, true, true, true, false, false}, 
    {false, false, false, false, false, false, false, true, true, true, false, false, false, false, false, true, true, true, false, true, true, true, false, false, false, false, false, true, true, true, false, true, true, false, false, false}, 
    {false, false, false, false, false, false, false, true, false, false, false, true, false, true, true, false, true, true, false, false, true, true, false, false, false, true, false, false, true, true}, 
    {false, false, false, false, false, false, false, false, false, true, false, false, false, false, false, false, false, false, true, true, true, false, false, true, false, false, false, false, true, true, true, false, false, true, true, true, false, false, false, false, false, true, false, false, false, false, false, false}, 
    {false, false, false, false, false, false, false, true, false, false, true, true, false, true, false, true, false, true, false, true, true, false, false, true}, 
    {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true, false, false, false, false, false, false, true, true, true, false, true, true, true, false, false, true, false, false, false, false, false, false, false, true}, 
    {false, false, false, false, false, false, false, false, false, false, false, true, true, true, true, true, true, true, true, true}, 
    {false, false, false, false, false, false, false, false, false, false, false, true, false, false, false, false, false, false, false, true, false, false, true, true, true, false, true, true, true, false, false, false, false, false, false, true, false, false, false, false}, 
    {false, false, false, false, false, true, false, true, false, false, false, true, false, false, true, false, true, false}
};

const uint8_t roboto11Widths[] = {
    2, 4, 6, 4, 5, 6, 2, 3, 3, 4, 6, 3, 4, 2, 4, 4, 3, 5, 5, 5, 4, 4, 5, 4, 4, 2, 3, 4, 4, 4, 4, 6, 6, 4, 5, 4, 4, 4, 5, 4, 4, 5, 5, 4, 6, 5, 6, 4, 6, 4, 5, 6, 4, 5, 7, 6, 5, 5, 3, 4, 3, 4, 4, 2, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 4, 4, 6, 4, 5, 4, 4, 4, 4, 4, 4, 5, 6, 5, 6, 4, 4, 2, 4, 6
};

const uint8_t roboto11Heights[] = {
    8, 3, 6, 9, 5, 7, 3, 10, 10, 5, 6, 4, 2, 2, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 5, 7, 5, 4, 5, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 10, 8, 10, 4, 2, 2, 6, 8, 6, 8, 6, 8, 8, 8, 8, 10, 8, 8, 6, 6, 6, 8, 8, 6, 6, 7, 6, 6, 6, 6, 8, 6, 10, 10, 10, 3
};

const int8_t roboto11XPositions[] = {
    1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 2, 1
};

const int8_t roboto11YPositions[] = {
    7, 7, 6, 7, 5, 6, 7, 7, 7, 6, 6, 1, 3, 1, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 4, 4, 5, 5, 5, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 8, 8, 8, 8, 0, 8, 5, 7, 5, 7, 5, 7, 5, 7, 7, 7, 7, 7, 5, 5, 5, 5, 5, 5, 5, 6, 5, 5, 5, 5, 4, 5, 7, 7, 7, 3
};

Font roboto11= Font(&roboto11Alphabet, roboto11Widths, roboto11Heights, roboto11XPositions, roboto11YPositions);

Font fontList[] = {
    roboto11
};

FontDatabase fontDatabase = fontList;
const uint8_t fontSizes[] = {
    11
};
