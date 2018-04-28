#ifndef _MAX30101_enum_
#define _MAX30101_enum_

namespace MAX{

    enum Address{
        INTERRUPT_STATUS    =   0x00,       //  2 Bytes
        INTERRUPT_CONFIG    =   0x02,       //  2 Bytes
        FIFO_WRITE_PTR      =   0x04,
        FIFO_OVERFLOW_CTR   =   0x05,
        FIFO_READ_PTR       =   0x06,
        FIFO_DATA           =   0x07,
        FIFO_CONFIG         =   0x08,
        MODE_CONFIG         =   0x09,
        OXYGEN_CONFIG       =   0x0A,
        LED_CONFIG          =   0x0C,       //  3 Bytes
        P_LED_CONFIG        =   0x10,
        LED_TIMING          =   0x11,       //  2 Bytes
        TEMPERATURE         =   0x1F,        //  2 Bytes
        TEMPERATURE_START   =   0x21,
        START_DELAY         =   0x30,
        REV_ID              =   0xFE,
        PART_ID             =   0xFF
    };

    enum Interrupt{
        I_FIFO_FULL     =   1<<7,
        I_NEW_DATA      =   1<<6,
        I_LIGHT         =   1<<5,
        I_START         =   1<<4,
        I_POWER_UP      =   1<<0,
        I_TEMPERATURE   =   1<<1
    };

    enum Mode{
        HR_MODE     =   2,
        OX_MODE     =   3,
        MULTI_MODE  =   7
    };

    enum Oxygen_Range{
        RANGE_2048    =   0,
        RANGE_4096    =   1,
        RANGE_8192    =   2,
        RANGE_16384   =   3
    };

    enum Oxygen_Rate{
        OX_RATE_50      =    000, 
        OX_RATE_100     =    001, 
        OX_RATE_200     =    010, 
        OX_RATE_400     =    011, 
        OX_RATE_800     =    100, 
        OX_RATE_1000    =    101, 
        OX_RATE_1600    =    110, 
        OX_RATE_3200    =    111 
    };

    enum Pulse_Width{
                                    //      Pulse Width     Resolution (bits)
        PULSE_WDT_69  =   00,   //      (68.95)         15
        PULSE_WDT_118 =   01,   //      (117.78)        16
        PULSE_WDT_215 =   10,   //      (215.44)        17
        PULSE_WDT_411 =   11    //      (410.75)        18
    };

    enum Led{
        // The P refers to Pilot Led, whose amplitude 
        // is specified in the P_LED_CONFIG register
        NONE_LED    =   0b000,
        RED_LED     =   0b001,
        IR_LED      =   0b010,
        GREEN_LED   =   0b011,
        P_RED_LED   =   0b101,
        P_IR_LED    =   0b110,
        P_GREEN_LED =   0b111
    };

    enum Oversample{
        OVERSAMPLE_1    =   0b000,
        OVERSAMPLE_2    =   0b001,
        OVERSAMPLE_4    =   0b010,
        OVERSAMPLE_8    =   0b011,
        OVERSAMPLE_16   =   0b100,
        OVERSAMPLE_32   =   0b101,
    };
}

#endif