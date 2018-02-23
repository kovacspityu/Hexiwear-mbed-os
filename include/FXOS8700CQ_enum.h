#ifndef _FXOS8700CQ_enum_
#define _FXOS8700CQ_enum_

enum FXOS8700CQ_Address{
    FXO_STATUS                  = 0x00,   //Acts as either DR_STATUS or FIFO_STATUS depending on if in FIFO mode.
    FXO_X_MSB                   = 0x01,
    FXO_X_LSB                   = 0x02,
    FXO_Y_MSB                   = 0x03,
    FXO_Y_LSB                   = 0x04,
    FXO_Z_MSB                   = 0x05,
    FXO_Z_LSB                   = 0x06,
    FXO_FIFO_SETUP              = 0x09,
    FXO_FIFO_TRIGGER            = 0x0A,
    FXO_SYSMOD                  = 0x0B,
    FXO_INT_STATUS              = 0x0C,
    FXO_WHO_AM_I                = 0x0D,
    FXO_DATA_CFG                = 0x0E,
    FXO_FILTER_CFG              = 0x0F,
    FXO_ORIENTATION             = 0x10,
    FXO_ORIENTATION_CFG         = 0x11,
    FXO_ORIENTATION_COUNT       = 0x12,
    FXO_LOCKING_ANGLES_CFG      = 0x13,
    FXO_ORIENTATION_THRESHOLD   = 0x14,
    FXO_MOTION_CFG              = 0x15,
    FXO_MOTION_INT_STATUS       = 0x16,
    FXO_MOTION_THRESHOLD        = 0x17,
    FXO_MOTION_COUNT            = 0x18,
    FXO_TRANSIENT_CFG           = 0x1D,
    FXO_TRANSIENT_INT_STATUS    = 0x1E,
    FXO_TRANSIENT_THRESHOLD     = 0x1F,
    FXO_TRANSIENT_COUNT         = 0x20,
    FXO_PULSE_CFG               = 0x21,
    FXO_PULSE_INT               = 0x22,
    FXO_PULSE_X_THRESHOLD       = 0x23,
    FXO_PULSE_Y_THRESHOLD       = 0x24,
    FXO_PULSE_Z_THRESHOLD       = 0x25,
    FXO_PULSE_TIME_LIMIT        = 0x26,
    FXO_PULSE_LATENCY           = 0x27,
    FXO_PULSE_WINDOW            = 0x28,
    FXO_SLEEP_COUNTER           = 0x29,
    FXO_CTRL_REG_1              = 0x2A,
    FXO_CTRL_REG_2              = 0x2B,
    FXO_SLEEP_INT_CONFIG        = 0x2C,
    FXO_INT_CONFIG              = 0x2D,
    FXO_INT_PIN_CONFIG          = 0x2E,
    FXO_ACC_X_OFFSET            = 0x2F,
    FXO_ACC_Y_OFFSET            = 0x30,
    FXO_ACC_Z_OFFSET            = 0x31,
    FXO_MAG_DR_STATUS           = 0x32,
    FXO_MAG_X_MSB               = 0x33,
    FXO_MAG_X_LSB               = 0x34,
    FXO_MAG_Y_MSB               = 0x35,
    FXO_MAG_Y_LSB               = 0x36,
    FXO_MAG_Z_MSB               = 0x37,
    FXO_MAG_Z_LSB               = 0x38,
    FXO_ACC_X_MSB               = 0x39,
    FXO_ACC_X_LSB               = 0x3A,
    FXO_ACC_Y_MSB               = 0x3B,
    FXO_ACC_Y_LSB               = 0x3C,
    FXO_ACC_Z_MSB               = 0x3D,
    FXO_ACC_Z_LSB               = 0x3E,
    FXO_MAG_X_OFFSET_MSB        = 0x3F,
    FXO_MAG_X_OFFSET_LSB        = 0x40,
    FXO_MAG_Y_OFFSET_MSB        = 0x41,
    FXO_MAG_Y_OFFSET_LSB        = 0x42,
    FXO_MAG_Z_OFFSET_MSB        = 0x43,
    FXO_MAG_Z_OFFSET_LSB        = 0x44,
    FXO_MAG_MAX_X_MSB           = 0x45,
    FXO_MAG_MAX_X_LSB           = 0x46,
    FXO_MAG_MAX_Y_MSB           = 0x47,
    FXO_MAG_MAX_Y_LSB           = 0x48,
    FXO_MAG_MAX_Z_MSB           = 0x49,
    FXO_MAG_MAX_Z_LSB           = 0x4A,
    FXO_MAG_MIN_X_MSB           = 0x4B,
    FXO_MAG_MIN_X_LSB           = 0x4C,
    FXO_MAG_MIN_Y_MSB           = 0x4D,
    FXO_MAG_MIN_Y_LSB           = 0x4E,
    FXO_MAG_MIN_Z_MSB           = 0x4F,
    FXO_MAG_MIN_Z_LSB           = 0x50,
    FXO_TEMPERATURE             = 0x51,
    FXO_MAG_THRESHOLD_CFG       = 0x52,
    FXO_MAG_THRESHOLD_INT       = 0x53,
    FXO_MAG_THRESHOLD_X_MSB     = 0x54,
    FXO_MAG_THRESHOLD_X_LSB     = 0x55,
    FXO_MAG_THRESHOLD_Y_MSB     = 0x56,
    FXO_MAG_THRESHOLD_Y_LSB     = 0x57,
    FXO_MAG_THRESHOLD_Z_MSB     = 0x58,
    FXO_MAG_THRESHOLD_Z_LSB     = 0x59,
    FXO_MAG_THRESHOLD_COUNT     = 0x5A,
    FXO_MAG_CTRL_REG_1          = 0x5B,
    FXO_MAG_CTRL_REG_2          = 0x5C,
    FXO_MAG_CTRL_REG_3          = 0x5D,
    FXO_MAG_INT                 = 0x5E,
    FXO_ACC_VEC_DIFF_CFG        = 0x5F,
    FXO_ACC_VEC_DIFF_MSB        = 0x60,
    FXO_ACC_VEC_DIFF_LSB        = 0x61,
    FXO_ACC_VEC_DIFF_COUNT      = 0x62,
    FXO_ACC_REF_VEC_X_MSB       = 0x63,
    FXO_ACC_REF_VEC_X_LSB       = 0x64,
    FXO_ACC_REF_VEC_Y_MSB       = 0x65,
    FXO_ACC_REF_VEC_Y_LSB       = 0x66,
    FXO_ACC_REF_VEC_Z_MSB       = 0x67,
    FXO_ACC_REF_VEC_Z_LSB       = 0x68,
    FXO_MAG_VEC_DIFF_CFG        = 0x69,
    FXO_MAG_VEC_DIFF_MSB        = 0x6A,
    FXO_MAG_VEC_DIFF_LSB        = 0x6B,
    FXO_MAG_VEC_DIFF_COUNT      = 0x6C,
    FXO_MAG_REF_VEC_X_MSB       = 0x6D,
    FXO_MAG_REF_VEC_X_LSB       = 0x6E,
    FXO_MAG_REF_VEC_Y_MSB       = 0x6F,
    FXO_MAG_REF_VEC_Y_LSB       = 0x70,
    FXO_MAG_REF_VEC_Z_MSB       = 0x71,
    FXO_MAG_REF_VEC_Z_LSB       = 0x72,
    FXO_MOTION_X_MSB            = 0x73,
    FXO_MOTION_X_LSB            = 0x74,
    FXO_MOTION_Y_MSB            = 0x75,
    FXO_MOTION_Y_LSB            = 0x76,
    FXO_MOTION_Z_MSB            = 0x77,
    FXO_MOTION_Z_LSB            = 0x78,
};

enum FXOS8700CQ_Axis{
    FXO_AXIS_X  = 4,
    FXO_AXIS_Y  = 2,
    FXO_AXIS_Z  = 1
};

enum FXOS8700CQ_Interrupt{
    I_SLEEP_WAKE_FXO    = 1,
    I_FIFO_FXO          = 2,
    I_TRANSIENT_FXO     = 4,
    I_ORIENTATION_FXO   = 8,
    I_PULSE_FXO         = 16,
    I_FREEFALL_FXO      = 32,
    I_ACC_MAG_FXO       = 64,
    I_NEW_DATA_FXO      = 128,
    I_MAG_THRESHOLD_FXO = 256,
    I_MAG_MAG_FXO       = 512,
    I_MAG_NEW_DATA_FXO  = 1024
};


enum FXOS8700CQ_Interrupt_Pin{
    FXO_PIN_ONE = 1,
    FXO_PIN_TWO = 0
};

enum FXOS8700CQ_Low{
                       //OSR=  800 Hz | 400 Hz | 200 Hz | 100 Hz | 50 Hz | 25 Hz | 12.5 Hz |
FXO_LOW0 = 0b00<<6,    //      256    | 128    | 64     | 32     | 16    | 8     | 4       |                                            
FXO_LOW1 = 0b01<<6,    //      128    | 64     | 32     | 16     | 8     | 4     | -       |
FXO_LOW2 = 0b10<<6     //      64     | 32     | 16     | 8      | 4     | -     | -       |
};

enum FXOS8700CQ_High{
    //All the values for an active High Pass also contain the bit that activates High Pass itself.

                          //OSR =  800 Hz | 400 Hz | 200 Hz | 100 Hz | 50 Hz | 25 Hz | 12.5 Hz |
FXO_HIGH0   = 0b001<<2,   //       15     | 7.5    | 3.75   | 1.875  | 0.937 | 0.468 | 0.234   |                                            
FXO_HIGH1   = 0b011<<2,   //       7.7    | 3.85   | 1.925  | 0.963  | 0.481 | 0.241 | 0.120   |
FXO_HIGH2   = 0b101<<2,   //       3.9    | 1.95   | 0.975  | 0.488  | 0.244 | 0.122 | 0.061   |
FXO_HIGH3   = 0b111<<2,   //       1.98   | 0.99   | 0.495  | 0.248  | 0.124 | 0.062 | 0.031   |
FXO_HIGHOFF = 0b000<<2
};

enum FXOS8700CQ_Range{
                                   //  Nominal Sensitivity (mdps/LSB)
    FXO_RANGE2000   = 0b000,       //  62.5
    FXO_RANGE1000   = 0b001,       //  31.25
    FXO_RANGE500    = 0b010,       //  15.625
    FXO_RANGE250    = 0b011,       //  7.8125
    
    FXO_RANGE4000   = 0b100,       //  125.0
};

enum FXOS8700CQ_Mode{
    FXO_STANDBY = 2,
    FXO_ACCELEROMETER   = 0,
    FXO_MAGNETOMETER    = 1,
    FXO_HYBRID          = 3
};

enum FXOS8700CQ_ODR{
    FXO_ODR800 = 0<<3,              // All numbers are approximated for readability, and valid for accelerometer and magnetic mode.
    FXO_ODR400 = 1<<3,              // In hybrid mode the real ODR is half that expected.
    FXO_ODR200 = 2<<3,
    FXO_ODR100 = 3<<3,
    FXO_ODR50  = 4<<3,
    FXO_ODR12  = 5<<3,
    FXO_ODR6   = 6<<3,
    FXO_ODR1   = 7<<3
};
#endif