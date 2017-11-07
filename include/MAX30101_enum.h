#ifndef _MAX30101_enum_
#define _MAX30101_enum_

enum MAX30101_Address{
    MAX_INTERRUPT_STATUS    =   0x00,       //  2 Bytes
    MAX_INTERRUPT_CONFIG    =   0x02,       //  2 Bytes
    MAX_FIFO_WRITE_PTR      =   0x04,
    MAX_FIFO_OVERFLOW_CTR   =   0x05,
    MAX_FIFO_READ_PTR       =   0x06,
    MAX_FIFO_DATA           =   0x07,
    MAX_FIFO_CONFIG         =   0x08,
    MAX_MODE_CONFIG         =   0x09,
    MAX_OXYGEN_CONFIG       =   0x0A,
    MAX_LED_CONFIG          =   0x0C,       //  3 Bytes
    MAX_P_LED_CONFIG        =   0x10,
    MAX_LED_TIMING          =   0x11,       //  2 Bytes
    MAX_TEMPERATURE         =   0x1F,        //  2 Bytes
    MAX_TEMPERATURE_START   =   0x21,
    MAX_START_DELAY         =   0x30,
    MAX_REV_ID              =   0xFE,
    MAX_PART_ID             =   0xFF
};

enum MAX30101_Interrupt{
    I_FIFO_FULL_MAX     =   1<<7,
    I_NEW_DATA_MAX      =   1<<6,
    I_LIGHT_MAX         =   1<<5,
    I_START_MAX         =   1<<4,
    I_POWER_UP_MAX      =   1<<0,
    I_TEMPERATURE_MAX   =   1<<1
};

enum MAX30101_Mode{
    MAX_HR_MODE     =   2,
    MAX_OX_MODE     =   3,
    MAX_MULTI_MODE  =   7
};

enum MAX30101_Oxygen_Range{
    MAX_2048    =   0,
    MAX_4096    =   1,
    MAX_8192    =   2,
    MAX_16384   =   3
};

enum MAX30101_Oxygen_Rate{
    MAX_OX_RATE_50      =    000, 
    MAX_OX_RATE_100     =    001, 
    MAX_OX_RATE_200     =    010, 
    MAX_OX_RATE_400     =    011, 
    MAX_OX_RATE_800     =    100, 
    MAX_OX_RATE_1000    =    101, 
    MAX_OX_RATE_1600    =    110, 
    MAX_OX_RATE_3200    =    111 
};

enum MAX30101_Pulse_Width{
                                //      Pulse Width     Resolution (bits)
    MAX_PULSE_WDT_69  =   00,   //      (68.95)         15
    MAX_PULSE_WDT_118 =   01,   //      (117.78)        16
    MAX_PULSE_WDT_215 =   10,   //      (215.44)        17
    MAX_PULSE_WDT_411 =   11    //      (410.75)        18
};

enum MAX30101_Led{
    // The P refers to Pilot Led, whose amplitude 
    // is specified in the MAX_P_LED_CONFIG register
    MAX_LED_NONE    =   0b000,
    MAX_LED_RED     =   0b001,
    MAX_LED_IR      =   0b010,
    MAX_LED_GREEN   =   0b011,
    MAX_LED_P_RED   =   0b101,
    MAX_LED_P_IR    =   0b110,
    MAX_LED_P_GREEN =   0b111
};

enum MAX30101_Oversample{
    MAX_OVERSAMPLE_1    =   0b000,
    MAX_OVERSAMPLE_2    =   0b001,
    MAX_OVERSAMPLE_4    =   0b010,
    MAX_OVERSAMPLE_8    =   0b011,
    MAX_OVERSAMPLE_16   =   0b100,
    MAX_OVERSAMPLE_32   =   0b101,
};


#endif