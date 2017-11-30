#ifndef _FXAS21002C_enum_
#define _FXAS21002C_enum_

enum FXAS21002C_Address{
    FXA_STATUS                  = 0x00,   //Aliases either DR_STATUS or FIFO_STATUS depending on if in FIFO mode.
    FXA_X_ANGLE_MSB             = 0x01,
    FXA_X_ANGLE_LSB             = 0x02,
    FXA_Y_ANGLE_MSB             = 0x03,
    FXA_Y_ANGLE_LSB             = 0x04,
    FXA_Z_ANGLE_MSB             = 0x05,
    FXA_Z_ANGLE_LSB             = 0x06,
    FXA_DR_STATUS               = 0x07,
    FXA_FIFO_STATUS             = 0x08,
    FXA_FIFO_SETUP              = 0x09,
    FXA_FIFO_EVENT              = 0x0A,
    FXA_INTERRUPT_STATUS        = 0x0B,
    FXA_WHO_AM_I                = 0x0C,
    FXA_CTRL_REG_0              = 0x0D,
    FXA_RT_INT_CONFIG           = 0x0E,
    FXA_THRESHOLD_STATUS        = 0x0F,
    FXA_THRESHOLD_CONFIG        = 0x10,
    FXA_THRESHOLD_COUNTER       = 0x11,
    FXA_TEMPERATURE             = 0x12,
    FXA_CTRL_REG_1              = 0x13,
    FXA_CTRL_REG_2              = 0x14,
    FXA_CTRL_REG_3              = 0x15
};

enum FXAS21002C_Axis{
    FXA_AXIS_X  = 0,
    FXA_AXIS_Y  = 1,
    FXA_AXIS_Z  = 2
};

enum FXAS21002C_Interrupt{
    I_NEW_DATA_FXA  = 1,
    I_THRESHOLD_FXA = 2,
    I_FIFO_FXA      = 4
};

enum FXAS21002C_Interrupt_Pin{
    FXA_PIN_ONE = 1,
    FXA_PIN_TWO = 0
};

enum FXAS21002C_Low{
                       //OSR=  800 Hz | 400 Hz | 200 Hz | 100 Hz | 50 Hz | 25 Hz | 12.5 Hz |
FXA_LOW0 = 0b00<<6,    //      256    | 128    | 64     | 32     | 16    | 8     | 4       |                                            
FXA_LOW1 = 0b01<<6,    //      128    | 64     | 32     | 16     | 8     | 4     | -       |
FXA_LOW2 = 0b10<<6     //      64     | 32     | 16     | 8      | 4     | -     | -       |
};

enum FXAS21002C_High{
    //All the values for an active High Pass also contain the bit that activates High Pass itself.

                          //OSR =  800 Hz | 400 Hz | 200 Hz | 100 Hz | 50 Hz | 25 Hz | 12.5 Hz |
FXA_HIGH0   = 0b001<<2,   //       15     | 7.5    | 3.75   | 1.875  | 0.937 | 0.468 | 0.234   |                                            
FXA_HIGH1   = 0b011<<2,   //       7.7    | 3.85   | 1.925  | 0.963  | 0.481 | 0.241 | 0.120   |
FXA_HIGH2   = 0b101<<2,   //       3.9    | 1.95   | 0.975  | 0.488  | 0.244 | 0.122 | 0.061   |
FXA_HIGH3   = 0b111<<2,   //       1.98   | 0.99   | 0.495  | 0.248  | 0.124 | 0.062 | 0.031   |
FXA_HIGHOFF = 0b000<<2
};

enum FXAS21002C_Range{
                                   //  Nominal Sensitivity (mdps/LSB)
    FXA_RANGE2000   = 0b000,       //  62.5
    FXA_RANGE1000   = 0b001,       //  31.25
    FXA_RANGE500    = 0b010,       //  15.625
    FXA_RANGE250    = 0b011,       //  7.8125
    
    FXA_RANGE4000   = 0b100,       //  125.0
};

enum FXAS21002C_Mode{

    FXA_STANDBY = 0b00,
    FXA_READY   = 0b01,
    FXA_ACTIVE  = 0b11
};

enum FXAS21002C_ODR{
    FXA_ODR800 = 0<<2,
    FXA_ODR400 = 1<<2,
    FXA_ODR200 = 2<<2,
    FXA_ODR100 = 3<<2,
    FXA_ODR50  = 4<<2,
    FXA_ODR25  = 5<<2,
    FXA_ODR12  = 6<<2
};
#endif