#ifndef _FXOS8700CQ_enum_
#define _FXOS8700CQ_enum_

namespace FXO{
    enum FXOS8700CQ_Address{
        STATUS                  = 0x00,   //Acts as either DR_STATUS or FIFO_STATUS depending on if in FIFO mode.
        X_MSB                   = 0x01,
        X_LSB                   = 0x02,
        Y_MSB                   = 0x03,
        Y_LSB                   = 0x04,
        Z_MSB                   = 0x05,
        Z_LSB                   = 0x06,
        FIFO_SETUP              = 0x09,
        FIFO_TRIGGER            = 0x0A,
        SYSMOD                  = 0x0B,
        INT_STATUS              = 0x0C,
        WHO_AM_I                = 0x0D,
        DATA_CFG                = 0x0E,
        FILTER_CFG              = 0x0F,
        ORIENTATION             = 0x10,
        ORIENTATION_CFG         = 0x11,
        ORIENTATION_COUNT       = 0x12,
        LOCKING_ANGLES_CFG      = 0x13,
        ORIENTATION_THRESHOLD   = 0x14,
        MOTION_CFG              = 0x15,
        MOTION_INT_STATUS       = 0x16,
        MOTION_THRESHOLD        = 0x17,
        MOTION_COUNT            = 0x18,
        TRANSIENT_CFG           = 0x1D,
        TRANSIENT_INT_STATUS    = 0x1E,
        TRANSIENT_THRESHOLD     = 0x1F,
        TRANSIENT_COUNT         = 0x20,
        PULSE_CFG               = 0x21,
        PULSE_INTERRUPT         = 0x22,
        PULSE_X_THRESHOLD       = 0x23,
        PULSE_Y_THRESHOLD       = 0x24,
        PULSE_Z_THRESHOLD       = 0x25,
        PULSE_TIME_LIMIT        = 0x26,
        PULSE_LATENCY           = 0x27,
        PULSE_WINDOW            = 0x28,
        SLEEP_COUNTER           = 0x29,
        CTRL_REG_1              = 0x2A,
        CTRL_REG_2              = 0x2B,
        SLEEP_INT_CONFIG        = 0x2C,
        INT_CONFIG              = 0x2D,
        INT_PIN_CONFIG          = 0x2E,
        ACC_X_OFFSET            = 0x2F,
        ACC_Y_OFFSET            = 0x30,
        ACC_Z_OFFSET            = 0x31,
        MAG_DR_STATUS           = 0x32,
        MAG_X_MSB               = 0x33,
        MAG_X_LSB               = 0x34,
        MAG_Y_MSB               = 0x35,
        MAG_Y_LSB               = 0x36,
        MAG_Z_MSB               = 0x37,
        MAG_Z_LSB               = 0x38,
        ACC_X_MSB               = 0x39,
        ACC_X_LSB               = 0x3A,
        ACC_Y_MSB               = 0x3B,
        ACC_Y_LSB               = 0x3C,
        ACC_Z_MSB               = 0x3D,
        ACC_Z_LSB               = 0x3E,
        MAG_X_OFFSET_MSB        = 0x3F,
        MAG_X_OFFSET_LSB        = 0x40,
        MAG_Y_OFFSET_MSB        = 0x41,
        MAG_Y_OFFSET_LSB        = 0x42,
        MAG_Z_OFFSET_MSB        = 0x43,
        MAG_Z_OFFSET_LSB        = 0x44,
        MAG_MAX_X_MSB           = 0x45,
        MAG_MAX_X_LSB           = 0x46,
        MAG_MAX_Y_MSB           = 0x47,
        MAG_MAX_Y_LSB           = 0x48,
        MAG_MAX_Z_MSB           = 0x49,
        MAG_MAX_Z_LSB           = 0x4A,
        MAG_MIN_X_MSB           = 0x4B,
        MAG_MIN_X_LSB           = 0x4C,
        MAG_MIN_Y_MSB           = 0x4D,
        MAG_MIN_Y_LSB           = 0x4E,
        MAG_MIN_Z_MSB           = 0x4F,
        MAG_MIN_Z_LSB           = 0x50,
        TEMPERATURE             = 0x51,
        MAG_THRESHOLD_CFG       = 0x52,
        MAG_THRESHOLD_INT       = 0x53,
        MAG_THRESHOLD_X_MSB     = 0x54,
        MAG_THRESHOLD_X_LSB     = 0x55,
        MAG_THRESHOLD_Y_MSB     = 0x56,
        MAG_THRESHOLD_Y_LSB     = 0x57,
        MAG_THRESHOLD_Z_MSB     = 0x58,
        MAG_THRESHOLD_Z_LSB     = 0x59,
        MAG_THRESHOLD_COUNT     = 0x5A,
        MAG_CTRL_REG_1          = 0x5B,
        MAG_CTRL_REG_2          = 0x5C,
        MAG_CTRL_REG_3          = 0x5D,
        MAG_INT_STATUS          = 0x5E,
        ACC_MAGNITUDE_CFG       = 0x5F,
        ACC_MAGNITUDE_MSB       = 0x60,
        ACC_MAGNITUDE_LSB       = 0x61,
        ACC_MAGNITUDE_COUNT     = 0x62,
        ACC_REF_X_MSB           = 0x63,
        ACC_REF_X_LSB           = 0x64,
        ACC_REF_Y_MSB           = 0x65,
        ACC_REF_Y_LSB           = 0x66,
        ACC_REF_Z_MSB           = 0x67,
        ACC_REF_Z_LSB           = 0x68,
        MAG_MAGNITUDE_CFG       = 0x69,
        MAG_MAGNITUDE_MSB       = 0x6A,
        MAG_MAGNITUDE_LSB       = 0x6B,
        MAG_MAGNITUDE_COUNT     = 0x6C,
        MAG_REF_X_MSB           = 0x6D,
        MAG_REF_X_LSB           = 0x6E,
        MAG_REF_Y_MSB           = 0x6F,
        MAG_REF_Y_LSB           = 0x70,
        MAG_REF_Z_MSB           = 0x71,
        MAG_REF_Z_LSB           = 0x72,
        MOTION_X_MSB            = 0x73,
        MOTION_X_LSB            = 0x74,
        MOTION_Y_MSB            = 0x75,
        MOTION_Y_LSB            = 0x76,
        MOTION_Z_MSB            = 0x77,
        MOTION_Z_LSB            = 0x78,
    };

    enum FXOS8700CQ_Axis{
        AXIS_X  = 4,
        AXIS_Y  = 2,
        AXIS_Z  = 1
    };

    enum FXOS8700CQ_Interrupt{
        I_NEW_DATA      = 1,
        I_ACC_MAGNITUDE = 2,
        I_FREEFALL      = 4,
        I_PULSE         = 8,
        I_ORIENTATION   = 16,
        I_TRANSIENT     = 32,
        I_FIFO          = 64,
        I_SLEEP_WAKE    = 128,
        I_MAG_THRESHOLD = 256,
        I_MAG_MAGNITUDE = 512,
        I_MAG_NEW_DATA  = 1024
    };

    enum FXOS8700CQ_Interrupt_Pin{
        PIN_ONE = 1,
        PIN_TWO = 0
    };

    enum FXOS8700CQ_Low{
                        //OSR=  800 Hz | 400 Hz | 200 Hz | 100 Hz | 50 Hz | 25 Hz | 12.5 Hz |
    LOW0 = 0b00<<6,    //      256    | 128    | 64     | 32     | 16    | 8     | 4       |                                            
    LOW1 = 0b01<<6,    //      128    | 64     | 32     | 16     | 8     | 4     | -       |
    LOW2 = 0b10<<6     //      64     | 32     | 16     | 8      | 4     | -     | -       |
    };

    enum FXOS8700CQ_High{
        //All the values for an active High Pass also contain the bit that activates High Pass itself.

                            //OSR =  800 Hz | 400 Hz | 200 Hz | 100 Hz | 50 Hz | 25 Hz | 12.5 Hz |
    HIGH0   = 0b001<<2,   //       15     | 7.5    | 3.75   | 1.875  | 0.937 | 0.468 | 0.234   |                                            
    HIGH1   = 0b011<<2,   //       7.7    | 3.85   | 1.925  | 0.963  | 0.481 | 0.241 | 0.120   |
    HIGH2   = 0b101<<2,   //       3.9    | 1.95   | 0.975  | 0.488  | 0.244 | 0.122 | 0.061   |
    HIGH3   = 0b111<<2,   //       1.98   | 0.99   | 0.495  | 0.248  | 0.124 | 0.062 | 0.031   |
    HIGHOFF = 0b000<<2
    };

    enum FXOS8700CQ_Range{
                                    // Nominal Sensitivity      Range
        RANGE2000   = 0,            // ±0.244 mg/LSB            ±2 g
        RANGE1000   = 1,            // ±0.488 mg/LSB            ±4 g
        RANGE500    = 2             // ±0.976 mg/LSB            ±8 g
    };

    enum FXOS8700CQ_Mode{
        STANDBY         = 2,
        ACCELEROMETER   = 0,
        MAGNETOMETER    = 1,
        HYBRID          = 3
    };

    enum FXOS8700CQ_Status{
        S_STANDBY   = 0,
        S_AWAKE     = 1,
        S_ASLEEP    = 2         //TODO Needs to be checked, the docs have a typo where this is indicated as 1
    };

    enum FXOS8700CQ_ODR{
        ODR800 = 0,              // All numbers are approximated for readability, and valid for accelerometer and magnetic mode.
        ODR400 = 1,              // In hybrid mode the real ODR is half that expected.
        ODR200 = 2,
        ODR100 = 3,
        ODR50  = 4,
        ODR12  = 5,
        ODR6   = 6,
        ODR1   = 7,
        ODR1_2 = 8
    };

    enum FXOS8700CQ_Acc_OSR{
        OSR_LOWEST  = 1,
        OSR_LOW     = 3,
        OSR_NORMAL  = 0,
        OSR_HIGH    = 2
    };

    enum FXOS8700CQ_FIFO_Mode{
        CIRCULAR    = 1,
        NO_OVERFLOW = 2,
        TRIGGER     = 3
    };

    enum FXOS8700CQ_Mag_OSR{
        OSR_0 = 0,    
        OSR_1 = 1,
        OSR_2 = 2,
        OSR_3 = 3,
        OSR_4 = 4,
        OSR_5 = 5,
        OSR_6 = 6,
        OSR_7 = 7
    };

    enum FXOS8700CQ_Freefall_Config{
        F_LATCH = 128,
        MOTION  = 64,
        ENABLE_X= 32,
        ENABLE_Y= 16,
        ENABLE_Z= 8
    };

    enum FXOS8700CQ_Acc_Magnitude_Config{
        A_LATCH         = 64,
        THRESHOLD       = 32,
        KEEP_THRESHOLD  = 16,
        ACC_MAGNITUDE_ON= 8,
    };

    enum FXOS8700CQ_Transient_Config{
        T_LATCH     = 16,
        T_ENABLE_Z  = 8,
        T_ENABLE_Y  = 4,
        T_ENABLE_X  = 2,
        BYPASS_HF   = 1
    };

    enum FXOS8700CQ_Pulse_Config{
        ENABLE_LATENCY  = 128,
        P_LATCH         = 64,
        ENABLE_DOUBLE_Z = 32,
        ENABLE_SINGLE_Z = 16,
        ENABLE_DOUBLE_Y = 32,
        ENABLE_SINGLE_Y = 16,
        ENABLE_DOUBLE_X = 32,
        ENABLE_SINGLE_X = 16,
    };

    enum FXOS8700CQ_Magnetic_Threshold_Config{
        M_LATCH         = 130,
        ABOVE_THRESHOLD = 66,
        M_ENABLE_Z      = 34,
        M_ENABLE_Y      = 18,
        M_ENABLE_X      = 10
    };
    
}
#endif