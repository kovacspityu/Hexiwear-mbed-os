#ifndef _MPL3115A2_ENUM_
#define _MPL3115A2_ENUM_

namespace MPL{

    enum Address
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
        FIFO_STATUS                 = 0x0D,
        FIFO_DATA                   = 0x0E,
        FIFO_SETUP                  = 0x0F,
        OVERFLOW_TIME               = 0x10,
        SYSTEM_MODE                 = 0x11,
        INTERRUPT_STATUS            = 0x12,
        EVENT_CONFIG                = 0x13,
        PRESSURE_REFERENCE_MSB      = 0x14,
        PRESSURE_REFERENCE_LSB      = 0x15,
        PRESSURE_TARGET_MSB         = 0x16,             ALTITUDE_TARGET_MSB         = 0x16,
        PRESSURE_TARGET_LSB         = 0x17,             ALTITUDE_TARGET_LSB         = 0x17,
        TEMPERATURE_TARGET          = 0x18,
        PRESSURE_WINDOW_MSB         = 0x19,             ALTITUDE_WINDOW_MSB         = 0x19,
        PRESSURE_WINDOW_LSB         = 0x1A,             ALTITUDE_WINDOW_LSB         = 0x1A,
        TEMPERATURE_WINDOW          = 0x1B,
        MIN_PRESSURE_MSB            = 0x1C,             MIN_ALTITUDE_MSB            = 0x1C,
        MIN_PRESSURE_CSB            = 0x1D,             MIN_ALTITUDE_CSB            = 0x1D,
        MIN_PRESSURE_LSB            = 0x1E,             MIN_ALTITUDE_LSB            = 0x1E,
        MIN_TEMPERATURE_MSB         = 0x1F,
        MIN_TEMPERATURE_LSB         = 0x20,
        MAX_PRESSURE_MSB            = 0x21,             MAX_ALTITUDE_MSB            = 0x21,
        MAX_PRESSURE_CSB            = 0x22,             MAX_ALTITUDE_CSB            = 0x22,
        MAX_PRESSURE_LSB            = 0x23,             MAX_ALTITUDE_LSB            = 0x23,
        MAX_TEMPERATURE_MSB         = 0x24,
        MAX_TEMPERATURE_LSB         = 0x25,
        CTRL_REG_1                  = 0x26,
        CTRL_REG_2                  = 0x27,
        CTRL_REG_3                  = 0x28,
        CTRL_REG_4                  = 0x29,
        CTRL_REG_5                  = 0x2A,
        PRESSURE_OFFSET             = 0x2B,
        TEMPERATURE_OFFSET          = 0x2C,
        ALTITUDE_OFFSET             = 0x2D
    };
    enum Mode
    {
        ALTIMETER    = 0x01,
        BAROMETER    = 0x00
    };

    enum Os_Ratio
    {
        OS_6_MS     = 0x00,       // 6 ms
        OS_10_MS    = 0x01,       // 10 ms
        OS_18_MS    = 0x02,       // 18 ms
        OS_34_MS    = 0x03,       // 34 ms
        OS_66_MS    = 0x04,       // 66 ms
        OS_130_MS   = 0x05,       // 130 ms
        OS_258_MS   = 0x06,       // 258 ms
        OS_512_MS   = 0x07        // 512 ms
    };

    enum Interrupt
    {
        I_NEW_DATA                  = 1<<7,
        I_FIFO                      = 1<<6,
        I_PRESSURE_THRESHOLD        = 1<<5,     I_ALTITUDE_THRESHOLD    = 1<<5,
        I_TEMPERATURE_THRESHOLD     = 1<<4,
        I_PRESSURE_WINDOW           = 1<<3,     I_ALTITUDE_WINDOW       = 1<<3,  
        I_TEMPERATURE_WINDOW        = 1<<2,
        I_PRESSURE_CHANGE           = 1<<1,     I_ALTITUDE_CHANGE       = 1<<1,
        I_TEMPERATURE_CHANGE        = 1<<0,
        I_NO_INTERRUPT              = 0
    };

    enum Time_Step
    {
    //It's just 2 to the power of the byte value, the names are for avoiding to do mental math
        TS_1S       = 0x00,
        TS_2S       = 0x01,
        TS_4S       = 0x02,
        TS_8S       = 0x03,
        TS_16S      = 0x04,
        TS_32S      = 0x05,
        TS_1M       = 0x06,
        TS_2M_8S    = 0x07,
        TS_4M_16S   = 0x08,
        TS_8M_30S   = 0x09,
        TS_17M      = 0x0A,
        TS_34M      = 0x0B,
        TS_1H_8M    = 0x0C,
        TS_2H_16M   = 0x0D,
        TS_4H_23M   = 0x0E,
        TS_9H_46M   = 0x0F
    };

    enum Interrupt_Pin{
    PIN_ONE = 1,
    PIN_TWO = 0
    };

    enum Data_Type{
        TYPE_PRESSURE    = 0,
        TYPE_ALTITUDE    = 1,
        TYPE_TEMPERATURE = 2
    };
    
}

#endif