#ifndef _MPL3115A2_enum_
#define _MPL3115A2_enum_
enum MPL3115A2_Register
{
    STATUS                      = 0x00,
    PRESSURE_MSB                = 0x01,             ALTITUDE_MSB                = 0x01,
    PRESSURE_CSB                = 0x02,             ALTITUDE_CSB                = 0x02,
    PRESSURE_LSB                = 0x03,             ALTITUDE_LSB                = 0x03,
    TEMPERATURE_MSB             = 0x04,
    TEMPERATURE_LSB             = 0x05,
    DR_STATUS                   = 0x06,
    PRESSURE_DELTA_MSB          = 0x07,             ALTITUDE_DELTA_MSB          = 0x07,
    PRESSURE_DELTA_CSB          = 0x08,             ALTITUDE_DELTA_CSB          = 0x08,
    PRESSURE_DELTA_LSB          = 0x09,             ALTITUDE_DELTA_LSB          = 0x09,
    TEMPERATURE_DELTA_MSB       = 0x0A,
    TEMPERATURE_DELTA_LSB       = 0x0B,
    WHO_AM_I                    = 0x0C,        // return 0xC4 by default
    FIFO_STATUS                 = 0X0D,
    FIFO_DATA                   = 0X0E,
    FIFO_SETUP                  = 0X0F,
    OVERFLOW_TIME               = 0X10,
    SYSTEM_MODE                 = 0X11,
    INTERRUPT_STATUS            = 0X12,
    EVENT_CONFIG                = 0X13,
    BAR_REF_MSB                 = 0X14,
    BAR_REF_LSB                 = 0X15,
    PRESSURE_TARGET_MSB         = 0X16,             ALTITUDE_TARGET_MSB         = 0X16,
    PRESSURE_TARGET_LSB         = 0X17,             ALTITUDE_TARGET_LSB         = 0X17,
    TEMPERATURE_TARGET          = 0X18,
    PRESSURE_WINDOW_TARGET_MSB  = 0X19,             ALTITUDE_WINDOW_TARGET_MSB  = 0X19,
    PRESSURE_WINDOW_TARGET_LSB  = 0X1A,             ALTITUDE_WINDOW_TARGET_LSB  = 0X1A,
    TEMPERATURE_WINDOW          = 0X1B,
    MIN_PRESSURE_MSB            = 0X1C,             MIN_ALTITUDE_MSB            = 0X1C,
    MIN_PRESSURE_CSB            = 0X1D,             MIN_ALTITUDE_CSB            = 0X1D,
    MIN_PRESSURE_LSB            = 0X1E,             MIN_ALTITUDE_LSB            = 0X1E,
    MIN_TEMPERATURE_MSB         = 0X1F,
    MIN_TEMPERATURE_LSB         = 0X20,
    MAX_PRESSURE_MSB            = 0X21,             MAX_ALTITUDE_MSB            = 0X21,
    MAX_PRESSURE_CSB            = 0X22,             MAX_ALTITUDE_CSB            = 0X22,
    MAX_PRESSURE_LSB            = 0X23,             MAX_ALTITUDE_LSB            = 0X23,
    MAX_TEMPERATURE_MSB         = 0X24,
    MAX_TEMPERATURE_LSB         = 0X25,
    CTRL_REG_1                  = 0x26,
    CTRL_REG_3                  = 0x28,
    CTRL_REG_4                  = 0x29,
    CTRL_REG_5                  = 0x2A,
    PRESSURE_OFFSET             = 0X2B,
    TEMPERATURE_OFFSET          = 0X2C,
    ALTITUDE_OFFSET             = 0X2D
    
};
enum MPL3315A2_Mode
{
    ALTIMETER     = 1,
    BAROMETER    = 0
};

enum MPL3315A2_Oversample_Ratio
{
    OS_6_MS     = 0,       // 6 ms
    OS_10_MS    = 1,       // 10 ms
    OS_18_MS    = 2,       // 18 ms
    OS_34_MS    = 3,       // 34 ms
    OS_66_MS    = 4,       // 66 ms
    OS_130_MS   = 5,       // 130 ms
    OS_258_MS   = 6,       // 258 ms
    OS_512_MS   = 7        // 512 ms
};
#endif