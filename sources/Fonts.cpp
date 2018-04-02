#include "SSD1351_Fonts.h"


namespace fnt{
    const bool roboto11Alphabet[NUMBER_OF_CHARACTERS][54] =
        {
            {false, false, false, false, false},
            {true, true, true, true, false, false, true},
            {true, false, true, true, false, true},
            {false, true, false, true, false, true, true, true, true, true, false, true, false, true, false, true, true, true, true, true, false, true, false, true, false},
            {false, true, false, true, false, true, true, false, false, false, true, false, false, false, true, true, false, true, true, true, true, false, true, false},
            {true, false, false, true, false, true, false, false, false, false, true, false, true, false, false, true},
            {false, true, true, false, false, true, false, false, true, false, false, true, true, false, false, true, false, false, true, true, true, false, false, true, false, false, true, true, false, true},
            {true, true},
            {false, true, true, false, true, false, true, false, true, false, true, false, true, false, true, false, false, true},
            {true, false, false, true, false, true, false, true, false, true, false, true, false, true, false, true, true, false},
            {false, true, false, true, true, true, false, true, false, true, false, true},
            {false, false, true, false, false, false, false, true, false, false, true, true, true, true, true, false, false, true, false, false, false, false, true, false, false},
            {false, true, false, true, true, false},
            {true, true, true},
            {true},
            {false, false, true, false, false, true, false, true, false, false, true, false, false, true, false, true, false, false, true, false, false},
            {true, true, true, true, false, true, true, false, true, true, false, true, true, false, true, true, false, true, true, true, true},
            {false, true, true, true, false, true, false, true, false, true, false, true, false, true},
            {false, true, true, false, true, false, false, true, false, false, false, true, false, false, true, false, false, false, true, false, false, true, false, false, true, true, true, true},
            {false, true, true, false, true, false, false, true, false, false, false, true, false, false, true, false, false, false, false, true, true, false, false, true, false, true, true, false},
            {false, false, false, true, false, false, true, true, false, true, false, true, false, true, false, true, false, true, false, true, true, true, true, true, false, false, false, true},
            {true, true, true, true, false, false, true, false, false, true, true, true, false, false, true, true, false, true, true, true, true},
            {true, true, false, true, false, false, true, true, false, true, false, true, true, false, true, true, false, true, true, true, true},
            {true, true, true, true, false, false, false, true, false, false, false, true, false, false, true, false, false, false, true, false, false, false, true, false, false, true, false, false},
            {true, true, true, true, false, true, true, false, true, false, true, false, true, false, true, true, false, true, true, true, true},
            {true, true, false, true, false, true, true, false, true, true, false, true, true, true, true, false, false, true, true, true, false},
            {true, false, false, true},
            {false, true, false, false, false, false, true, true, false, true, true, false},
            {false, false, true, true, true, false, true, true, false, false, false, true},
            {true, true, true, false, false, false, true, true, true},
            {true, false, false, false, true, true, false, true, true, true, false, false},
            {true, true, true, true, false, true, false, false, true, false, false, true, false, true, false, false, false, false, false, true, false},
            {false, true, true, true, false, false, true, false, true, false, true, false, true, false, true, true, true, false, true, true, true, true, false, true, false, true, false, true, true, false, false, true, true, false, false},
            {false, false, true, false, false, false, false, true, false, false, false, true, false, true, false, false, true, false, true, false, false, true, true, true, false, false, true, false, true, false, true, false, false, false, true},
            {true, true, true, true, false, true, true, false, true, true, true, false, true, false, true, true, false, true, true, true, true},
            {false, true, true, true, false, true, false, true, true, false, false, false, true, false, false, false, true, false, false, false, false, true, false, true, false, true, true, true},
            {true, true, false, true, false, true, true, false, true, true, false, true, true, false, true, true, false, true, true, true, false},
            {true, true, true, true, false, false, true, false, false, true, true, true, true, false, false, true, false, false, true, true, true},
            {true, true, true, true, false, false, true, false, false, true, true, true, true, false, false, true, false, false, true, false, false},
            {false, true, true, true, false, true, false, true, true, false, false, false, true, false, false, false, true, false, true, true, false, true, false, true, false, true, true, true},
            {true, false, true, true, false, true, true, false, true, true, true, true, true, false, true, true, false, true, true, false, true},
            {true, true, true, false, true, false, false, true, false, false, true, false, false, true, false, false, true, false, true, true, true},
            {false, false, false, true, false, false, false, true, false, false, false, true, false, false, false, true, false, false, false, true, true, false, false, true, false, true, true, false},
            {true, false, true, true, true, false, true, false, true, true, false, false, true, true, false, false, true, true, false, false, true, false, true, false, true, false, true, true},
            {true, false, false, true, false, false, true, false, false, true, false, false, true, false, false, true, false, false, true, true, true},
            {true, false, false, false, true, true, true, false, true, true, true, false, true, false, true, true, false, false, false, true, true, false, false, false, true, true, false, false, false, true, true, false, false, false, true},
            {true, false, false, true, true, true, false, true, true, true, false, true, true, true, false, true, true, false, true, true, true, false, true, true, true, false, false, true},
            {false, true, true, true, false, false, true, false, true, false, true, false, false, false, true, true, false, false, false, true, true, false, false, false, true, false, true, false, true, false, false, true, true, true, false},
            {true, true, true, true, false, true, true, false, true, true, false, true, true, true, true, true, false, false, true, false, false},
            {false, true, true, true, false, false, true, false, true, false, true, false, false, false, true, true, false, false, false, true, true, false, true, false, true, false, true, true, true, false, false, true, true, true, true},
            {true, true, true, true, false, true, true, false, true, true, true, false, true, false, true, true, false, true, true, false, true},
            {false, true, true, true, false, true, false, true, false, true, false, false, false, false, true, false, false, false, false, true, true, false, false, true, false, true, true, true},
            {true, true, true, true, true, false, false, true, false, false, false, false, true, false, false, false, false, true, false, false, false, false, true, false, false, false, false, true, false, false, false, false, true, false, false},
            {true, false, true, true, false, true, true, false, true, true, false, true, true, false, true, true, false, true, true, true, true},
            {true, false, false, true, false, true, false, true, false, true, false, true, false, true, false, true, false, true, true, false, false, false, true, false, false, false, true, false},
            {true, true, false, true, false, true, false, true, false, true, false, true, false, true, false, true, false, true, false, true, true, false, true, false, false, false, true, false, true, false, false, false, true, false, true, false, false, false, true, false, true, false},
            {true, true, false, true, true, false, true, false, true, false, false, false, true, false, false, false, false, true, false, false, false, true, true, false, false, false, true, false, true, false, true, false, false, true, true},
            {true, false, false, true, false, true, false, true, false, true, true, false, false, false, true, false, false, false, true, false, false, false, true, false, false, false, true, false},
            {true, true, true, true, false, false, false, true, false, false, true, false, false, true, true, false, false, true, false, false, true, false, false, false, true, true, true, true},
            {true, true, true, false, true, false, true, false, true, false, true, false, true, false, true, false, true, true},
            {true, false, false, true, false, false, false, true, false, false, true, false, false, true, false, false, false, true, false, false, true},
            {true, true, false, true, false, true, false, true, false, true, false, true, false, true, false, true, true, true},
            {false, true, false, true, true, false, true, false, true},
            {true, true, true},
            {true},
            {true, true, true, false, false, true, true, true, true, true, false, true, true, true, true},
            {true, false, false, true, false, false, true, true, true, true, false, true, true, false, true, true, false, true, true, true, true},
            {true, true, true, true, false, true, true, false, false, true, false, true, true, true, true},
            {false, false, true, false, false, true, true, true, true, true, false, true, true, false, true, true, false, true, true, true, true},
            {true, true, true, true, false, true, true, true, true, true, false, false, true, true, true},
            {false, true, true, false, true, false, true, true, true, false, true, false, false, true, false, false, true, false, false, true, false},
            {true, true, true, true, false, true, true, false, true, true, false, true, true, true, true, false, false, true, true, true, true},
            {true, false, false, true, false, false, true, true, true, true, false, true, true, false, true, true, false, true, true, false, true},
            {false, true, false, false, false, false, true, true, false, false, true, false, false, true, false, false, true, false, true, true, true},
            {false, true, false, false, false, true, false, true, false, true, false, true, false, true, false, true, true, true},
            {true, false, false, true, false, false, true, false, true, true, true, false, true, true, false, true, true, false, true, false, true},
            {true, true, false, false, true, false, false, true, false, false, true, false, false, true, false, false, true, false, true, true, true},
            {true, true, true, true, true, true, false, true, false, true, true, false, true, false, true, true, false, true, false, true, true, false, true, false, true},
            {true, true, true, true, false, true, true, false, true, true, false, true, true, false, true},
            {false, true, true, true, true, false, false, true, true, false, false, true, true, false, false, true, false, true, true, true},
            {true, true, true, true, false, true, true, false, true, true, false, true, true, true, true, true, false, false, true, false, false},
            {true, true, true, true, false, true, true, false, true, true, false, true, true, true, true, false, false, true, false, false, true},
            {true, true, true, true, false, false, true, false, false, true, false, false, true, false, false},
            {true, true, true, true, false, false, false, true, false, false, false, true, true, true, true},
            {false, true, false, true, true, true, false, true, false, false, true, false, false, true, false, false, true, true},
            {true, false, true, true, false, true, true, false, true, true, false, true, true, true, true},
            {true, false, false, true, false, true, false, true, false, true, false, true, false, true, true, false, false, false, true, false},
            {true, false, true, false, true, true, false, true, false, true, true, true, true, true, false, false, true, false, true, false, false, true, false, true, false},
            {true, true, false, true, false, true, true, false, false, false, true, false, false, true, false, true, true, true, false, true},
            {true, false, false, true, true, false, true, false, true, false, false, true, false, true, false, false, true, true, false, false, false, false, true, false, false, false, false, true, false, false, false, true, false, false, false},
            {true, true, true, false, false, true, false, true, false, true, false, false, true, true, true},
            {false, false, true, false, true, false, false, true, false, false, true, false, true, false, false, false, true, false, false, true, false, false, true, false, false, false, true},
            {true, true, true, true, true, true, true, true, true},
            {true, false, false, false, true, false, false, true, false, false, true, false, false, false, true, false, true, false, false, true, false, false, true, false, true, false, false},
            {false, true, false, false, true, true, false, true, true, false}
        };

    const uint8_t roboto11Widths[] = {
        5, 1, 3, 5, 3, 4, 5, 1, 2, 2, 3, 5, 2, 3, 1, 3, 3, 2, 4, 4, 4, 3, 3, 4, 3, 3, 1, 2, 3, 3, 3, 3, 5, 5, 3, 4, 3, 3, 3, 4, 3, 3, 4, 4, 3, 5, 4, 5, 3, 5, 3, 4, 5, 3, 4, 6, 5, 4, 4, 2, 3, 2, 3, 3, 1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 3, 3, 5, 3, 4, 3, 3, 3, 3, 3, 3, 4, 5, 4, 5, 3, 3, 1, 3, 5
    };

    const uint8_t roboto11Heights[] = {
        1, 7, 2, 5, 8, 4, 6, 2, 9, 9, 4, 5, 3, 1, 1, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 4, 6, 4, 3, 4, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 9, 7, 9, 3, 1, 1, 5, 7, 5, 7, 5, 7, 7, 7, 7, 9, 7, 7, 5, 5, 5, 7, 7, 5, 5, 6, 5, 5, 5, 5, 7, 5, 9, 9, 9, 2
    };

    const int8_t roboto11XPositions[] = {
      1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 2, 1
    };

    const int8_t roboto11YPositions[] = {
       7, 1, 1, 2, 1, 3, 2, 1, 1, 1, 2, 2, 7, 5, 7, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 4, 3, 3, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 8, 0, 3, 1, 3, 1, 3, 1, 3, 1, 1, 1, 1, 1, 3, 3, 3, 3, 3, 3, 3, 2, 3, 3, 3, 3, 4, 3, 1, 1, 1, 5
    };

    Font roboto11= Font(54, &(roboto11Alphabet[0][0]), roboto11Widths, roboto11Heights, roboto11XPositions, roboto11YPositions);
    const bool roboto15Alphabet[NUMBER_OF_CHARACTERS][112] =
        {
            {true, true, true, true, true, true, false, false, true},
            {true, false, true, true, false, true, true, false, true},
            {false, false, false, true, false, false, true, false, false, false, true, false, true, false, false, true, true, true, true, true, true, false, false, true, false, false, true, false, false, false, true, false, false, true, false, true, true, true, true, true, true, true, false, false, true, false, true, false, false, false, false, true, false, true, false, false, false, true, false, false, true, false, false},
            {false, false, false, true, false, false, false, false, true, true, false, false, false, true, false, false, true, false, true, true, false, false, false, true, true, true, false, false, false, false, false, true, true, true, false, false, false, false, false, true, true, false, false, false, false, false, true, true, true, false, false, false, false, true, true, true, false, false, true, true, false, true, true, true, true, false, false, false, false, true, false, false},
            {false, true, true, false, false, false, false, false, true, false, false, true, false, true, false, false, true, false, false, true, false, true, false, false, false, true, true, false, true, false, false, false, false, false, false, true, false, false, false, false, false, false, false, true, false, true, true, false, false, false, true, false, true, false, false, true, false, false, true, false, true, false, false, true, false, false, false, false, false, true, true, false},
            {false, true, true, true, false, false, false, false, true, false, false, true, false, false, true, true, false, false, true, false, false, false, true, true, true, false, false, false, false, true, true, false, false, false, false, true, true, true, true, false, true, false, true, false, false, true, true, true, false, true, false, false, false, true, false, false, false, true, true, true, false, true, true},
            {true, true, true},
            {false, false, true, false, true, false, true, false, false, true, false, false, true, false, false, true, false, false, true, false, false, true, false, false, true, false, false, true, false, false, true, false, false, false, true, false, false, false, true},
            {true, false, false, false, true, false, false, false, true, false, false, true, false, false, true, false, false, true, false, false, true, false, false, true, false, false, true, true, false, true, false, false, true, false, true, false, true, false, false},
            {false, false, true, true, false, false, false, false, true, true, false, false, true, true, true, true, true, true, false, false, true, true, false, false, false, false, true, true, false, false, false, true, false, false, true, false},
            {false, false, true, true, false, false, false, false, true, true, false, false, false, false, true, true, false, false, true, true, true, true, true, true, false, false, true, true, false, false, false, false, true, true, false, false, false, false, true, true, false, false},
            {false, true, false, true, false, true, true, false},
            {true, true, true, true, true},
            {true, true, true, true},
            {false, false, false, true, false, false, false, true, false, false, true, true, false, false, true, false, false, false, true, false, false, true, false, false, false, true, false, false, false, true, false, false, true, false, false, false, true, false, false, false},
            {false, true, true, true, true, false, true, true, false, false, true, true, true, false, false, false, false, true, true, false, false, false, true, true, true, false, true, true, false, true, true, true, false, false, false, true, true, false, false, false, false, true, true, true, false, false, true, true, false, true, true, true, true, false},
            {false, false, true, true, true, true, false, true, false, false, false, true, false, false, false, true, false, false, false, true, false, false, false, true, false, false, false, true, false, false, false, true, false, false, false, true},
            {false, true, true, true, false, false, true, false, false, false, true, false, true, false, false, false, true, false, false, false, false, false, true, false, false, false, false, true, false, false, false, false, true, true, false, false, false, true, true, false, false, false, true, true, false, false, false, false, true, true, true, true, true, true},
            {false, true, true, true, false, true, false, false, false, true, false, false, false, false, true, false, false, false, false, true, false, true, true, true, false, false, false, false, false, true, false, false, false, false, true, true, false, false, false, true, false, true, true, true, false},
            {false, false, false, false, true, true, false, false, false, false, false, true, true, false, false, false, false, true, false, true, false, false, false, true, false, false, true, false, false, false, true, false, false, true, false, false, true, false, false, false, true, false, true, true, true, true, true, true, true, false, false, false, false, false, true, false, false, false, false, false, false, true, false},
            {false, true, true, true, true, true, false, true, false, false, false, false, false, true, false, false, false, false, false, true, true, true, true, false, false, true, false, false, true, true, false, false, false, false, false, true, true, false, false, false, false, true, true, true, false, false, true, true, false, true, true, true, true, false},
            {false, false, true, true, false, false, false, true, false, false, false, false, true, false, false, false, false, false, true, false, true, true, true, false, true, true, false, false, true, false, true, false, false, false, false, true, true, false, false, false, false, true, true, true, false, false, true, false, false, true, true, true, true, false},
            {true, true, true, true, true, true, false, false, false, false, true, true, false, false, false, false, true, false, false, false, false, false, true, false, false, false, false, true, false, false, false, false, false, true, false, false, false, false, true, false, false, false, false, false, true, false, false, false, false, true, false, false, false, false},
            {false, true, true, true, true, false, false, true, false, false, true, true, true, true, false, false, false, true, false, true, false, false, true, true, false, false, true, true, false, false, true, true, false, false, true, true, true, false, false, false, false, true, true, true, false, false, true, true, false, true, true, true, true, false},
            {false, true, true, true, true, false, true, true, false, false, true, true, true, false, false, false, false, true, true, false, false, false, false, true, true, true, false, false, true, true, false, true, true, true, true, true, false, false, false, false, true, false, false, false, false, false, true, false, false, true, true, true, false, false},
            {true, true, true, true, false, false, false, false, false, false, false, false, true, true, true, true},
            {true, true, true, true, false, false, false, false, false, false, false, false, false, false, false, true, false, true, false, true, true, false},
            {false, false, false, false, true, false, true, true, true, true, true, true, false, false, false, true, true, false, false, false, false, true, true, true, true, false, false, false, false, true},
            {true, true, true, true, true, true, false, false, false, false, false, false, false, false, false, false, false, false, true, true, true, true, true, true},
            {true, false, false, false, false, false, true, true, true, true, true, false, false, false, false, false, true, true, false, false, false, false, true, true, true, true, true, true, true, false, true, false, false, false, false, false},
            {false, true, true, true, true, false, true, true, false, false, true, true, false, false, false, false, true, true, false, false, false, false, true, false, false, false, false, true, true, false, false, false, true, true, false, false, false, false, true, false, false, false, false, false, false, false, false, false, false, false, true, false, false, false},
            {false, false, true, true, true, true, false, false, true, true, false, false, false, true, false, true, false, true, true, false, true, true, false, true, false, false, true, true, true, false, true, false, true, false, true, true, false, true, false, true, false, true, false, true, true, true, false, true, true, false, true, false, false, false, false, false, false, false, true, true, true, true, false},
            {false, false, true, true, false, false, false, false, true, true, false, false, false, false, true, true, false, false, false, true, false, false, true, false, false, true, false, false, true, false, false, true, true, true, true, false, true, false, false, false, false, true, true, false, false, false, false, true, true, false, false, false, false, true},
            {true, true, true, true, true, false, true, false, false, false, true, true, true, false, false, false, false, true, true, false, false, false, true, true, true, true, true, true, true, false, true, false, false, false, false, true, true, false, false, false, false, true, true, false, false, false, false, true, true, true, true, true, true, false},
            {false, true, true, true, true, false, true, true, false, false, true, true, true, false, false, false, false, true, true, false, false, false, false, false, true, false, false, false, false, false, true, false, false, false, false, false, true, false, false, false, false, true, true, true, false, false, true, true, false, true, true, true, true, false},
            {true, true, true, true, false, false, true, false, false, false, true, false, true, false, false, false, false, true, true, false, false, false, false, true, true, false, false, false, false, true, true, false, false, false, false, true, true, false, false, false, false, true, true, false, false, false, true, false, true, true, true, true, false, false},
            {true, true, true, true, true, true, true, false, false, false, false, false, true, false, false, false, false, false, true, false, false, false, false, false, true, true, true, true, true, false, true, false, false, false, false, false, true, false, false, false, false, false, true, false, false, false, false, false, true, true, true, true, true, true},
            {true, true, true, true, true, true, false, false, false, false, true, false, false, false, false, true, false, false, false, false, true, true, true, true, false, true, false, false, false, false, true, false, false, false, false, true, false, false, false, false, true, false, false, false, false},
            {false, false, true, true, true, true, false, false, true, true, false, false, true, true, false, true, false, false, false, false, false, false, true, false, false, false, false, false, false, true, false, false, true, true, true, false, true, false, false, false, false, true, false, true, false, false, false, false, true, false, true, true, false, false, false, true, false, false, true, true, true, true, false},
            {true, false, false, false, false, true, true, false, false, false, false, true, true, false, false, false, false, true, true, false, false, false, false, true, true, true, true, true, true, true, true, false, false, false, false, true, true, false, false, false, false, true, true, false, false, false, false, true, true, false, false, false, false, true},
            {true, true, true, true, true, false, false, true, false, false, false, false, true, false, false, false, false, true, false, false, false, false, true, false, false, false, false, true, false, false, false, false, true, false, false, false, false, true, false, false, true, true, true, true, true},
            {false, false, false, false, false, true, false, false, false, false, false, true, false, false, false, false, false, true, false, false, false, false, false, true, false, false, false, false, false, true, false, false, false, false, false, true, true, false, false, false, false, true, true, false, false, false, true, false, false, true, true, true, false, false},
            {true, false, false, false, true, true, false, true, false, false, false, true, false, false, true, false, false, true, false, false, false, true, false, true, false, false, false, false, true, true, true, false, false, false, false, true, true, false, true, false, false, false, true, false, false, true, true, false, false, true, false, false, false, true, true, false, true, false, false, false, false, true, true},
            {true, false, false, false, false, false, true, false, false, false, false, false, true, false, false, false, false, false, true, false, false, false, false, false, true, false, false, false, false, false, true, false, false, false, false, false, true, false, false, false, false, false, true, false, false, false, false, false, true, true, true, true, true, true},
            {true, true, false, false, true, true, true, true, false, false, true, true, true, true, false, false, true, true, true, false, true, true, false, true, true, false, true, true, false, true, true, false, true, true, false, true, true, false, false, false, false, true, true, false, false, false, false, true, true, false, false, false, false, true},
            {true, false, false, false, false, true, true, true, false, false, false, true, true, true, false, false, false, true, true, false, true, false, false, true, true, false, true, false, false, true, true, false, false, true, false, true, true, false, false, true, true, true, true, false, false, false, true, true, true, false, false, false, false, true},
            {false, true, true, true, true, false, true, true, false, false, true, false, true, false, false, false, false, true, true, false, false, false, false, true, true, false, false, false, false, true, true, false, false, false, false, true, true, false, false, false, false, true, true, true, false, false, true, true, false, true, true, true, true, false},
            {true, true, true, true, true, false, true, false, false, false, false, true, true, false, false, false, false, true, true, false, false, false, false, true, true, true, true, true, true, false, true, false, false, false, false, false, true, false, false, false, false, false, true, false, false, false, false, false, true, false, false, false, false, false},
            {false, true, true, true, true, false, true, true, false, false, true, true, true, false, false, false, false, true, true, false, false, false, false, true, true, false, false, false, false, true, true, false, false, false, false, true, true, false, false, false, false, true, true, true, false, false, true, true, false, true, true, true, true, false, false, false, false, false, true, true},
            {true, true, true, true, true, false, true, false, false, false, true, true, true, false, false, false, false, true, true, false, false, false, true, true, true, true, true, true, true, false, true, false, false, true, true, false, true, false, false, false, true, false, true, false, false, false, true, true, true, false, false, false, false, true},
            {false, true, true, true, true, false, true, true, false, false, true, true, true, false, false, false, false, true, true, true, false, false, false, false, false, false, true, true, true, false, false, false, false, false, true, true, true, false, false, false, false, true, true, true, false, false, false, true, false, true, true, true, true, false},
            {true, true, true, true, true, true, true, false, false, false, true, false, false, false, false, false, false, true, false, false, false, false, false, false, true, false, false, false, false, false, false, true, false, false, false, false, false, false, true, false, false, false, false, false, false, true, false, false, false, false, false, false, true, false, false, false, false, false, false, true, false, false, false},
            {true, false, false, false, false, true, true, false, false, false, false, true, true, false, false, false, false, true, true, false, false, false, false, true, true, false, false, false, false, true, true, false, false, false, false, true, true, false, false, false, false, true, true, true, false, false, true, true, false, true, true, true, true, false},
            {true, true, false, false, false, false, true, false, true, false, false, false, false, true, false, true, false, false, false, true, true, false, false, true, false, false, true, false, false, false, true, false, false, true, false, false, false, true, false, true, true, false, false, false, false, true, true, false, false, false, false, false, true, true, false, false, false, false, false, true, true, false, false},
            {true, true, false, true, true, false, true, true, false, true, false, true, true, false, true, false, false, true, false, true, true, false, true, false, false, true, false, true, true, false, true, false, false, true, false, true, true, false, true, false, false, true, false, true, true, false, true, false, false, true, false, true, true, false, true, false, false, true, true, true, true, true, true, false, false, false, true, false, false, true, false, false},
            {true, false, false, false, false, true, true, true, false, false, true, false, false, true, false, false, true, false, false, false, true, true, false, false, false, false, true, true, false, false, false, false, true, true, false, false, false, true, false, false, true, false, true, true, false, false, true, false, true, false, false, false, false, true},
            {true, true, false, false, false, false, true, false, true, false, false, false, true, true, false, true, true, false, false, true, false, false, false, true, false, true, false, false, false, false, true, true, true, false, false, false, false, false, true, false, false, false, false, false, false, true, false, false, false, false, false, false, true, false, false, false, false, false, false, true, false, false, false},
            {true, true, true, true, true, true, false, false, false, false, true, false, false, false, false, true, true, false, false, false, false, true, false, false, false, false, true, false, false, false, false, true, true, false, false, false, false, true, false, false, false, false, true, false, false, false, false, false, true, true, true, true, true, true},
            {true, true, true, false, true, false, true, false, true, false, true, false, true, false, true, false, true, false, true, false, true, false, true, false, true, true},
            {true, false, false, false, true, false, false, false, true, false, false, false, false, true, false, false, false, true, false, false, false, false, true, false, false, false, true, false, false, false, true, false, false, false, false, true, false, false, false, true},
            {true, true, false, true, false, true, false, true, false, true, false, true, false, true, false, true, false, true, false, true, false, true, false, true, true, true},
            {false, false, true, true, false, false, false, false, true, true, false, false, false, true, true, true, true, false, false, true, false, false, true, false, true, true, false, false, true, true},
            {true, true, true, true, true, true},
            {true, true},
            {false, true, true, true, true, false, true, true, false, false, true, false, false, false, false, false, true, true, false, true, true, true, true, true, true, false, false, false, true, true, true, false, false, false, true, true, false, true, true, true, true, true},
            {true, false, false, false, false, false, true, false, false, false, false, false, true, false, false, false, false, false, true, false, true, true, true, false, true, true, false, false, true, true, true, false, false, false, false, true, true, false, false, false, false, true, true, false, false, false, false, true, true, true, false, false, true, true, true, false, true, true, true, false},
            {false, true, true, true, true, false, true, true, false, false, true, true, true, false, false, false, false, false, true, false, false, false, false, false, true, false, false, false, false, false, true, true, false, false, true, true, false, true, true, true, true, false},
            {false, false, false, false, true, true, false, false, false, false, true, true, false, false, false, false, true, true, false, true, true, true, true, true, true, true, false, false, true, true, true, false, false, false, true, true, true, false, false, false, true, true, true, false, false, false, true, true, true, true, false, false, true, true, false, true, true, true, false, true},
            {false, true, true, true, true, false, true, true, false, false, true, true, true, false, false, false, false, true, true, true, true, true, true, true, true, false, false, false, false, false, true, true, false, false, false, true, false, true, true, true, true, false},
            {false, false, false, true, true, true, false, false, true, false, false, false, false, false, true, false, false, false, true, true, true, true, true, true, false, false, true, false, false, false, false, false, true, false, false, false, false, false, true, false, false, false, false, false, true, false, false, false, false, false, true, false, false, false, false, false, true, false, false, false},
            {false, true, true, true, false, true, true, true, false, false, true, true, true, false, false, false, false, true, true, false, false, false, false, true, true, false, false, false, false, true, true, true, false, false, true, true, false, true, true, true, false, true, false, false, false, false, true, true, true, false, false, false, true, false, false, true, true, true, true, false},
            {true, false, false, false, false, false, true, false, false, false, false, false, true, false, false, false, false, false, true, false, true, true, true, false, true, true, false, false, true, true, true, false, false, false, false, true, true, false, false, false, false, true, true, false, false, false, false, true, true, false, false, false, false, true, true, false, false, false, false, true},
            {false, false, true, true, false, false, false, false, false, false, false, false, true, true, true, true, false, false, false, false, false, true, false, false, false, false, false, true, false, false, false, false, false, true, false, false, false, false, false, true, false, false, false, false, false, true, false, false, true, true, true, true, true, true},
            {false, false, false, true, false, false, false, false, false, true, true, true, false, false, false, true, false, false, false, true, false, false, false, true, false, false, false, true, false, false, false, true, false, false, false, true, false, false, false, true, false, false, false, true, true, true, true, false},
            {true, false, false, false, false, false, true, false, false, false, false, false, true, false, false, false, false, false, true, false, false, false, true, true, true, false, false, true, false, false, true, false, true, false, false, false, true, true, true, false, false, false, true, false, false, true, false, false, true, false, false, false, true, false, true, false, false, false, true, true},
            {true, true, true, true, false, false, false, false, false, true, false, false, false, false, false, true, false, false, false, false, false, true, false, false, false, false, false, true, false, false, false, false, false, true, false, false, false, false, false, true, false, false, false, false, false, true, false, false, false, false, false, true, false, false, true, true, true, true, true, true},
            {true, true, true, true, true, true, true, false, true, true, false, true, true, false, true, false, false, true, true, false, true, false, false, true, true, false, true, false, false, true, true, false, true, false, false, true, true, false, true, false, false, true},
            {true, false, true, true, true, false, true, true, false, false, true, true, true, false, false, false, false, true, true, false, false, false, false, true, true, false, false, false, false, true, true, false, false, false, false, true, true, false, false, false, false, true},
            {false, true, true, true, true, false, true, true, false, false, true, true, true, false, false, false, false, true, true, false, false, false, false, true, true, false, false, false, false, true, true, true, false, false, true, true, false, true, true, true, true, false},
            {true, false, true, true, true, true, true, false, false, true, true, false, false, false, false, true, false, false, false, false, true, false, false, false, false, true, true, false, false, true, true, false, true, true, true, true, false, false, false, false, true, false, false, false, false, true, false, false, false, false},
            {false, true, true, true, false, true, true, true, false, false, true, true, true, false, false, false, true, true, true, false, false, false, true, true, true, false, false, false, true, true, true, true, false, false, true, true, false, true, true, true, true, true, false, false, false, false, true, true, false, false, false, false, true, true, false, false, false, false, true, true},
            {true, false, true, true, true, true, true, false, false, false, true, false, false, false, false, true, false, false, false, false, true, false, false, false, false, true, false, false, false, false, true, false, false, false, false},
            {false, true, true, true, true, false, true, true, false, false, true, true, true, true, false, false, false, false, false, false, true, true, true, false, false, false, false, false, true, true, true, true, false, false, false, true, false, true, true, true, true, false},
            {false, false, true, false, false, false, false, false, true, false, false, false, true, true, true, true, true, true, false, false, true, false, false, false, false, false, true, false, false, false, false, false, true, false, false, false, false, false, true, false, false, false, false, false, true, false, false, false, false, false, true, true, true, true},
            {true, false, false, false, false, true, true, false, false, false, false, true, true, false, false, false, false, true, true, false, false, false, false, true, true, false, false, false, false, true, true, true, false, false, true, true, false, true, true, true, false, true},
            {true, false, false, false, false, true, true, false, false, false, true, true, false, true, false, false, true, false, false, true, false, false, true, false, false, true, true, true, false, false, false, false, true, true, false, false, false, false, true, true, false, false},
            {true, false, false, true, false, false, true, true, false, true, false, true, true, false, true, false, false, true, false, true, true, false, true, false, false, true, false, true, true, false, true, false, false, true, true, false, true, false, true, false, false, true, true, false, false, true, false, false, false, false, true, false, false, true, false, false},
            {true, false, false, false, true, true, false, true, false, false, true, false, false, false, true, true, false, false, false, false, true, true, false, false, false, false, true, true, false, false, false, true, false, false, true, false, true, false, false, false, false, true},
            {true, true, false, false, false, false, true, false, true, false, false, false, true, true, false, false, true, false, false, true, false, false, false, true, false, false, true, false, false, false, true, true, true, false, false, false, false, false, true, true, false, false, false, false, false, true, true, false, false, false, false, false, true, false, false, false, false, false, true, true, false, false, false, false, true, true, false, false, false, false},
            {true, true, true, true, true, true, false, false, false, false, true, false, false, false, false, true, false, false, false, false, true, true, false, false, false, true, true, false, false, false, false, true, false, false, false, false, true, true, true, true, true, true},
            {false, false, false, true, false, false, true, false, false, false, true, false, false, false, true, false, false, true, true, false, false, true, false, false, true, false, false, false, false, true, false, false, false, true, true, false, false, false, true, false, false, false, true, false, false, false, true, false, false, false, false, true},
            {true, true, true, true, true, true, true, true, true, true, true, true, true, true},
            {true, false, false, false, false, true, false, false, false, true, false, false, false, true, false, false, false, true, true, false, false, false, true, false, false, false, false, true, false, false, true, false, false, true, true, false, false, true, false, false, false, true, false, false, false, true, false, false, true, false, false, false},
            {false, true, true, true, false, false, true, true, false, false, true, true, false, true, true, false, false, false, true, true, false}
        };

    const uint8_t roboto15Widths[] = {
        1, 3, 7, 6, 8, 7, 1, 3, 3, 6, 6, 2, 5, 2, 4, 6, 4, 6, 5, 7, 6, 6, 6, 6, 6, 2, 2, 5, 6, 6, 6, 7, 6, 6, 6, 6, 6, 5, 7, 6, 5, 6, 7, 6, 6, 6, 6, 6, 6, 6, 6, 7, 6, 7, 8, 6, 7, 6, 2, 4, 2, 6, 6, 2, 6, 6, 6, 6, 6, 6, 6, 6, 6, 4, 6, 6, 6, 6, 6, 5, 6, 5, 6, 6, 6, 6, 8, 6, 7, 6, 4, 1, 4, 7
    };

    const uint8_t roboto15Heights[] = {
        9, 3, 9, 12, 9, 9, 3, 13, 13, 6, 7, 4, 1, 2, 10, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 8, 11, 6, 4, 6, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 10, 9, 9, 9, 9, 9, 9, 9, 9, 9, 13, 10, 13, 5, 1, 1, 7, 10, 7, 10, 7, 10, 10, 10, 9, 12, 10, 10, 7, 7, 7, 10, 10, 7, 7, 9, 7, 7, 7, 7, 10, 7, 13, 14, 13, 3
    };

    const int8_t roboto15XPositions[] = {
      1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
    };

    const int8_t roboto15YPositions[] = {
       3, 2, 3, 1, 3, 3, 3, 2, 2, 4, 4, 12, 8, 11, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 5, 6, 5, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 1, 3, 1, 3, 12, 3, 5, 2, 5, 2, 5, 2, 5, 2, 3, 3, 2, 2, 5, 5, 5, 5, 5, 5, 5, 3, 5, 5, 5, 5, 5, 5, 1, 1, 1, 6
    };

    Font roboto15= Font(112, &(roboto15Alphabet[0][0]), roboto15Widths, roboto15Heights, roboto15XPositions, roboto15YPositions);
    Font fontList[] = {
        roboto11,     roboto15
    };

    FontDatabase fontDatabase = fontList;
    const uint8_t fontSizes[] = {
        11,         15
    };
}
