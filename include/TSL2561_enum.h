#ifndef _TSL2561_ENUM_
#define _TSL2561_ENUM_

enum TSL2561_Address
{
    CONTROL     = 0x00,
    TIMING      = 0x01,
    LOW_THRESHOLD_LSB   = 0x02,
    LOW_THRESHOLD_MSB   = 0x03,
    HIGH_THRESHOLD_LSB  = 0x04,
    HIGH_THRESHOLD_MSB  = 0x05,
    INTERRUPT           = 0x06,
    ID                  = 0x0A,
    WHOLE_DATA_LSB      = 0x0C,
    WHOLE_DATA_MSB      = 0x0D,
    INFRARED_DATA_LSB   = 0x0E,
    INFRARED_DATA_MSB   = 0x0F
};

enum TSL2561_OS_RATE
{
    OS_14MS     = 0,    // 13.7 mS
    OS_100MS    = 1,    // 101  mS
    OS_400MS    = 2     // 402  mS
};

enum TSL2561_GAIN
{
    LOW_GAIN    = 0,    // x1  gain 
    HIGH_GAIN   = 1     // x16 gain
};

#endif