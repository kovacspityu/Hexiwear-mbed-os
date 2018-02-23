#ifndef _SSD1351_enum_
#define _SSD1351_enum_

#define SCREEN_SIZE 96

enum SSD_Command{
    SSD_SELECT_HORIZONTAL       = 0x15,
    SSD_SELECT_VERTICAL         = 0x75,
    SSD_WRITE_ENABLE            = 0x5C,
    SSD_OPTIONS                 = 0xA0,
    SSD_DISPLAY_V_OFFSET        = 0xA1,     // This moves just the image across the display
    SSD_IMAGE_V_OFFSET          = 0xA2,     // Locked by default, needs SSD_COMMAND_LOCK and right option to be sent first. 
                                            // This changes where the display starts, which is different from the above 
                                            // IFF the active area is smaller than the physical screen.
    SSD_DISPLAY_OFF             = 0xA4,
    SSD_DISPLAY_WHITE           = 0xA5,
    SSD_DISPLAY_ON              = 0xA6,
    SSD_DISPLAY_NEGATIVE        = 0xA7,
    SSD_SLEEP_EXTRA             = 0xAB,     // To save power, this can be used to detach the regulator of the logic voltage
    SSD_SLEEP                   = 0xAE,
    SSD_WAKE_UP                 = 0xAF,
    SSD_PHASE_1_AND_2           = 0xB1,     // Locked by default, needs SSD_COMMAND_LOCK and right option to be sent first
    SSD_ENHANCE                 = 0xB2,     // No idea, not documented
    SSD_FPS                     = 0xB3,     // Locked by default, needs SSD_COMMAND_LOCK and right option to be sent first
    SSD_COLUMN_DEFAULT_V        = 0xB4,     
    SSD_PHASE_3                 = 0xB6,
    SSD_GAMMA_TABLE             = 0xB8,
    SSD_DEFAULT_GAMMA           = 0xB9,
    SSD_PHASE_2_V               = 0xBB,     // Locked by default, needs SSD_COMMAND_LOCK and right option to be sent first
    SSD_ROW_DEFAULT_V           = 0xBE,     // Locked by default, needs SSD_COMMAND_LOCK and right option to be sent first
    SSD_COLOUR_BRIGTHNESS       = 0xC1,
    SSD_BRIGHTNESS              = 0xC7,
    SSD_ACTIVE_ROWS             = 0xCA,
    SSD_COMMAND_LOCK            = 0xFD,
    SSD_SCROLLING               = 0x96,
    SSD_SCROLLING_STOP          = 0x9E,     // After the stop, the ram needs to be rewritten
    SSD_SCROLLING_START         = 0x9F
};

enum SSD_Error{
    SSD_NO_ERROR                    = 0,
    SSD_OUT_OF_TOP_BORDER           = 1,
    SSD_OUT_OF_BOTTOM_BORDER        = 2,
    SSD_OUT_OF_RIGHT_BORDER         = 4,
    SSD_OUT_OF_LEFT_BORDER          = 8,
    SSD_OUT_OF_VERTICAL_BORDERS     = 3,
    SSD_OUT_OF_TOP_RIGHT_BORDER     = 5,
    SSD_OUT_OF_TOP_LEFT_BORDER      = 9,
    SSD_OUT_OF_BOTTOM_RIGHT_BORDER  = 6,
    SSD_OUT_OF_BOTTOM_LEFT_BORDER   = 10,
    SSD_OUT_OF_HORIZONTAL_BORDER    = 12,
    SSD_OUT_ALL_BUT_TOP             = 14,
    SSD_OUT_ALL_BUT_BOTTOM          = 13,    
    SSD_OUT_ALL_BUT_RIGHT           = 11,
    SSD_OUT_ALL_BUT_LEFT            = 7,
    SSD_OUT_ALL_BORDERS             = 15
};

enum SSD_Scrolling_Period{
    SSD_TEST_MODE   = 0,
    SSD_NORMAL      = 1,
    SSD_SLOW        = 2,
    SSD_SLOWEST     = 3
};


#endif
