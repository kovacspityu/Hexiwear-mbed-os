#ifndef _SSD1351_enum_
#define _SSD1351_enum_

namespace SSD{

    enum Command{
        SELECT_HORIZONTAL       = 0x15,
        SELECT_VERTICAL         = 0x75,
        WRITE_ENABLE            = 0x5C,
        OPTIONS                 = 0xA0,
        DISPLAY_V_OFFSET        = 0xA1,     // This moves just the image across the display
        IMAGE_V_OFFSET          = 0xA2,     // Locked by default, needs COMMAND_LOCK and right option to be sent first. 
                                            // This changes where the display starts, which is different from the above 
                                            // IFF the active area is smaller than the physical screen.
        DISPLAY_OFF             = 0xA4,
        DISPLAY_WHITE           = 0xA5,
        DISPLAY_ON              = 0xA6,
        DISPLAY_NEGATIVE        = 0xA7,
        SLEEP_EXTRA             = 0xAB,     // To save power, this can be used to detach the regulator of the logic voltage
        NO_OP                   = 0xAD,
        SLEEP                   = 0xAE,
        WAKE_UP                 = 0xAF,
        PHASE_1_AND_2           = 0xB1,     // Locked by default, needs COMMAND_LOCK and right option to be sent first
        ENHANCE                 = 0xB2,     // No idea, not documented
        FPS                     = 0xB3,     // Locked by default, needs COMMAND_LOCK and right option to be sent first
        COLUMN_DEFAULT_V        = 0xB4,     
        PHASE_3                 = 0xB6,
        GAMMA_TABLE             = 0xB8,
        DEFAULT_GAMMA           = 0xB9,
        PHASE_2_V               = 0xBB,     // Locked by default, needs COMMAND_LOCK and right option to be sent first
        ROW_DEFAULT_V           = 0xBE,     // Locked by default, needs COMMAND_LOCK and right option to be sent first
        COLOUR_BRIGTHNESS       = 0xC1,
        BRIGHTNESS              = 0xC7,
        ACTIVE_ROWS             = 0xCA,
        COMMAND_LOCK            = 0xFD,
        SCROLLING               = 0x96,
        SCROLLING_STOP          = 0x9E,     // After the stop, the ram needs to be rewritten
        SCROLLING_START         = 0x9F
    };

    enum Error{
        NO_ERROR                    = 0,
        OUT_OF_TOP_BORDER           = 1,
        OUT_OF_BOTTOM_BORDER        = 2,
        OUT_OF_RIGHT_BORDER         = 4,
        OUT_OF_LEFT_BORDER          = 8,
        OUT_OF_VERTICAL_BORDERS     = 3,
        OUT_OF_TOP_RIGHT_BORDER     = 5,
        OUT_OF_TOP_LEFT_BORDER      = 9,
        OUT_OF_BOTTOM_RIGHT_BORDER  = 6,
        OUT_OF_BOTTOM_LEFT_BORDER   = 10,
        OUT_OF_HORIZONTAL_BORDERS   = 12,
        OUT_ALL_BUT_TOP             = 14,
        OUT_ALL_BUT_BOTTOM          = 13,    
        OUT_ALL_BUT_RIGHT           = 11,
        OUT_ALL_BUT_LEFT            = 7,
        OUT_ALL_BORDERS             = 15
    };

    enum Scrolling_Period{
        TEST_MODE   = 0,
        NORMAL      = 1,
        SLOW        = 2,
        SLOWEST     = 3
    };
}

#endif
