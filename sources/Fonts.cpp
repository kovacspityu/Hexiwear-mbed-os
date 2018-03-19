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
    Font fontList[] = {
        roboto11
    };

    FontDatabase fontDatabase = fontList;
    const uint8_t fontSizes[] = {
        11
    };
}
