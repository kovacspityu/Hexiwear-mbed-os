#ifndef _TSL2561_ENUM_
#define _TSL2561_ENUM_

enum TSL2561_Address
{
    TSL_CONTROL             = 0xA0,
    TSL_TIMING              = 0xA1,
    TSL_LOW_THRESHOLD_LSB   = 0xA2,
    TSL_LOW_THRESHOLD_MSB   = 0xA3,
    TSL_HIGH_THRESHOLD_LSB  = 0xA4,
    TSL_HIGH_THRESHOLD_MSB  = 0xA5,
    TSL_INTERRUPT           = 0xA6,
    TSL_ID                  = 0xAA,
    TSL_WHOLE_DATA_LSB      = 0xAC,
    TSL_WHOLE_DATA_MSB      = 0xAD,
    TSL_INFRARED_DATA_LSB   = 0xAE,
    TSL_INFRARED_DATA_MSB   = 0xAF
};

enum TSL2561_Interrupt_Length{
    TSL_ALWAYS          = 0b0000,
    TSL_ONE_CYCLE       = 0b0001,
    TSL_TWO_CYCLES      = 0b0010,
    TSL_THREE_CYCLES    = 0b0011,
    TSL_FOUR_CYCLES     = 0b0100,
    TSL_FIVE_CYCLES     = 0b0101,
    TSL_SIX_CYCLES      = 0b0110,
    TSL_SEVEN_CYCLES    = 0b0111,
    TSL_EIGHT_CYCLES    = 0b1000,
    TSL_NINE_CYCLES     = 0b1001,
    TSL_TEN_CYCLES      = 0b1010,
    TSL_ELEVEN_CYCLES   = 0b1011,
    TSL_TWELVE_CYCLES   = 0b1100,
    TSL_THIRTEEN_CYCLES = 0b1101,
    TSL_FOURTEEN_CYCLES = 0b1110,
    TSL_FIFTEEN_CYCLES  = 0b1111
};

enum TSL2561_Os_Rate
{
    TSL_OS_14MS     = 0,    // 13.7 mS      0.034   times the normalized value
    TSL_OS_100MS    = 1,    // 101  mS      0.252   times the normalized value
    TSL_OS_400MS    = 2     // 402  mS      1       times the normalized value
};

enum TSL2561_Gain
{
    TSL_LOW_GAIN    = 0,    // x1  gain 
    TSL_HIGH_GAIN   = 1     // x16 gain
};

#endif