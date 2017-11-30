#ifndef _MPL3115A2_ENUM_
#define _MPL3115A2_ENUM_
enum MPL3115A2_Address
{
    MPL_STATUS                      = 0x00,
    MPL_PRESSURE_MSB                = 0x01,             MPL_ALTITUDE_MSB                = 0x01,
    MPL_PRESSURE_CSB                = 0x02,             MPL_ALTITUDE_CSB                = 0x02,
    MPL_PRESSURE_LSB                = 0x03,             MPL_ALTITUDE_LSB                = 0x03,
    MPL_TEMPERATURE_MSB             = 0x04,
    MPL_TEMPERATURE_LSB             = 0x05,
    MPL_DR_STATUS                   = 0x06,
    MPL_PRESSURE_DELTA_MSB          = 0x07,             MPL_ALTITUDE_DELTA_MSB          = 0x07,
    MPL_PRESSURE_DELTA_CSB          = 0x08,             MPL_ALTITUDE_DELTA_CSB          = 0x08,
    MPL_PRESSURE_DELTA_LSB          = 0x09,             MPL_ALTITUDE_DELTA_LSB          = 0x09,
    MPL_TEMPERATURE_DELTA_MSB       = 0x0A,
    MPL_TEMPERATURE_DELTA_LSB       = 0x0B,
    MPL_WHO_AM_I                    = 0x0C,        // return 0xC4 by default
    MPL_FIFO_STATUS                 = 0x0D,
    MPL_FIFO_DATA                   = 0x0E,
    MPL_FIFO_SETUP                  = 0x0F,
    MPL_OVERFLOW_TIME               = 0x10,
    MPL_SYSTEM_MODE                 = 0x11,
    MPL_INTERRUPT_STATUS            = 0x12,
    MPL_EVENT_CONFIG                = 0x13,
    MPL_PRESSURE_REFERENCE_MSB      = 0x14,
    MPL_PRESSURE_REFERENCE_LSB      = 0x15,
    MPL_PRESSURE_TARGET_MSB         = 0x16,             MPL_ALTITUDE_TARGET_MSB         = 0x16,
    MPL_PRESSURE_TARGET_LSB         = 0x17,             MPL_ALTITUDE_TARGET_LSB         = 0x17,
    MPL_TEMPERATURE_TARGET          = 0x18,
    MPL_PRESSURE_WINDOW_MSB         = 0x19,             MPL_ALTITUDE_WINDOW_MSB         = 0x19,
    MPL_PRESSURE_WINDOW_LSB         = 0x1A,             MPL_ALTITUDE_WINDOW_LSB         = 0x1A,
    MPL_TEMPERATURE_WINDOW          = 0x1B,
    MPL_MIN_PRESSURE_MSB            = 0x1C,             MPL_MIN_ALTITUDE_MSB            = 0x1C,
    MPL_MIN_PRESSURE_CSB            = 0x1D,             MPL_MIN_ALTITUDE_CSB            = 0x1D,
    MPL_MIN_PRESSURE_LSB            = 0x1E,             MPL_MIN_ALTITUDE_LSB            = 0x1E,
    MPL_MIN_TEMPERATURE_MSB         = 0x1F,
    MPL_MIN_TEMPERATURE_LSB         = 0x20,
    MPL_MAX_PRESSURE_MSB            = 0x21,             MPL_MAX_ALTITUDE_MSB            = 0x21,
    MPL_MAX_PRESSURE_CSB            = 0x22,             MPL_MAX_ALTITUDE_CSB            = 0x22,
    MPL_MAX_PRESSURE_LSB            = 0x23,             MPL_MAX_ALTITUDE_LSB            = 0x23,
    MPL_MAX_TEMPERATURE_MSB         = 0x24,
    MPL_MAX_TEMPERATURE_LSB         = 0x25,
    MPL_CTRL_REG_1                  = 0x26,
    MPL_CTRL_REG_2                  = 0x27,
    MPL_CTRL_REG_3                  = 0x28,
    MPL_CTRL_REG_4                  = 0x29,
    MPL_CTRL_REG_5                  = 0x2A,
    MPL_PRESSURE_OFFSET             = 0x2B,
    MPL_TEMPERATURE_OFFSET          = 0x2C,
    MPL_ALTITUDE_OFFSET             = 0x2D
};
enum MPL3315A2_Mode
{
    MPL_ALTIMETER    = 0x01,
    MPL_BAROMETER    = 0x00
};

enum MPL3315A2_Os_Ratio
{
    MPL_OS_6_MS     = 0x00,       // 6 ms
    MPL_OS_10_MS    = 0x01,       // 10 ms
    MPL_OS_18_MS    = 0x02,       // 18 ms
    MPL_OS_34_MS    = 0x03,       // 34 ms
    MPL_OS_66_MS    = 0x04,       // 66 ms
    MPL_OS_130_MS   = 0x05,       // 130 ms
    MPL_OS_258_MS   = 0x06,       // 258 ms
    MPL_OS_512_MS   = 0x07        // 512 ms
};

enum MPL3115A2_Interrupt
{
    I_NEW_DATA_MPL                  = 0x80,
    I_FIFO_MPL                      = 0x40,
    I_PRESSURE_THRESHOLD_MPL        = 0x20,     I_ALTITUDE_THRESHOLD_MPL    = 0x20,
    I_TEMPERATURE_THRESHOLD_MPL     = 0x0F,
    I_PRESSURE_WINDOW_MPL           = 0x08,     I_ALTITUDE_WINDOW_MPL       = 0x08,  
    I_TEMPERATURE_WINDOW_MPL        = 0x04,
    I_PRESSURE_CHANGE_MPL           = 0x02,     I_ALTITUDE_CHANGE_MPL       = 0x02,
    I_TEMPERATURE_CHANGE_MPL        = 0x01
};

enum MPL3115A2_Time_Step
{
//It's just 2 to the power of the byte value, the names are for avoiding to do mental math
    MPL_TS_1S       = 0x00,
    MPL_TS_2S       = 0x01,
    MPL_TS_4S       = 0x02,
    MPL_TS_8S       = 0x03,
    MPL_TS_16S      = 0x04,
    MPL_TS_32S      = 0x05,
    MPL_TS_1M       = 0x06,
    MPL_TS_2M_8S    = 0x07,
    MPL_TS_4M_16S   = 0x08,
    MPL_TS_8M_30S   = 0x09,
    MPL_TS_17M      = 0x0A,
    MPL_TS_34M      = 0x0B,
    MPL_TS_1H_8M    = 0x0C,
    MPL_TS_2H_16M   = 0x0D,
    MPL_TS_4H_23M   = 0x0E,
    MPL_TS_9H_46M   = 0x0F
};

enum MPL3115A2_Interrupt_Pin{
MPL_PIN_ONE = 0x01,
MPL_PIN_TWO = 0x00
};

enum MPL3115A2_Data_Type{
            MPL_TYPE_PRESSURE    = 0,
            MPL_TYPE_ALTITUDE    = 1,
            MPL_TYPE_TEMPERATURE = 2
        };

#endif