#ifndef _TSL2561_ENUM_
#define _TSL2561_ENUM_

enum TSL2561_Address
{
    CONTROL             = 0xA0,
    TIMING              = 0xA1,
    LOW_THRESHOLD_LSB   = 0xA2,
    LOW_THRESHOLD_MSB   = 0xA3,
    HIGH_THRESHOLD_LSB  = 0xA4,
    HIGH_THRESHOLD_MSB  = 0xA5,
    INTERRUPT           = 0xA6,
    ID                  = 0xAA,
    WHOLE_DATA_LSB      = 0xAC,
    WHOLE_DATA_MSB      = 0xAD,
    INFRARED_DATA_LSB   = 0xAE,
    INFRARED_DATA_MSB   = 0xAF
};

enum TSL2561_Interrupt_Length{
    ALWAYS          = 0b0000,
    ONE_CYCLE       = 0b0001,
    TWO_CYCLES      = 0b0010,
    THREE_CYCLES    = 0b0011,
    FOUR_CYCLES     = 0b0100,
    FIVE_CYCLES     = 0b0101,
    SIX_CYCLES      = 0b0110,
    SEVEN_CYCLES    = 0b0111,
    EIGHT_CYCLES    = 0b1000,
    NINE_CYCLES     = 0b1001,
    TEN_CYCLES      = 0b1010,
    ELEVEN_CYCLES   = 0b1011,
    TWELVE_CYCLES   = 0b1100,
    THIRTEEN_CYCLES = 0b1101,
    FOURTEEN_CYCLES = 0b1110,
    FIFTEEN_CYCLES  = 0b1111
};

enum TSL2561_Os_Rate
{
    OS_14MS     = 0,    // 13.7 mS      0.034   times the normalized value
    OS_100MS    = 1,    // 101  mS      0.252   times the normalized value
    OS_400MS    = 2     // 402  mS      1       times the normalized value
};

enum TSL2561_Gain
{
    LOW_GAIN    = 0,    // x1  gain 
    HIGH_GAIN   = 1     // x16 gain
};

#endif