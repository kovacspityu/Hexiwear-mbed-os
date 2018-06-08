#ifndef _FXAS21002C_enum_
#define _FXAS21002C_enum_

namespace FXA{

    enum Address{
        STATUS                  = 0x00,   //Aliases either DR_STATUS or FIFO_STATUS depending on if in FIFO mode.
        X_ANGLE_MSB             = 0x01,
        X_ANGLE_LSB             = 0x02,
        Y_ANGLE_MSB             = 0x03,
        Y_ANGLE_LSB             = 0x04,
        Z_ANGLE_MSB             = 0x05,
        Z_ANGLE_LSB             = 0x06,
        DR_STATUS               = 0x07,
        FIFO_STATUS             = 0x08,
        FIFO_SETUP              = 0x09,
        FIFO_EVENT              = 0x0A,
        INTERRUPT_STATUS        = 0x0B,
        WHO_AM_I                = 0x0C,
        CTRL_REG_0              = 0x0D,
        RT_INT_CONFIG           = 0x0E,
        THRESHOLD_STATUS        = 0x0F,
        THRESHOLD_CONFIG        = 0x10,
        THRESHOLD_COUNTER       = 0x11,
        TEMPERATURE             = 0x12,
        CTRL_REG_1              = 0x13,
        CTRL_REG_2              = 0x14,
        CTRL_REG_3              = 0x15
    };

    enum Axis{
        AXIS_X  = 0,
        AXIS_Y  = 1,
        AXIS_Z  = 2
    };

    enum Interrupt{
        I_NO_INTERRUPT  = 0,
        I_NEW_DATA      = 1,
        I_THRESHOLD     = 2,
        I_FIFO          = 4
    };

    enum Interrupt_Pin{
        PIN_ONE = 1,
        PIN_TWO = 0
    };

    enum Low{
                        //OSR=  800 Hz | 400 Hz | 200 Hz | 100 Hz | 50 Hz | 25 Hz | 12.5 Hz |
    LOW0 = 0b00<<6,    //      256    | 128    | 64     | 32     | 16    | 8     | 4       |                                            
    LOW1 = 0b01<<6,    //      128    | 64     | 32     | 16     | 8     | 4     | -       |
    LOW2 = 0b10<<6     //      64     | 32     | 16     | 8      | 4     | -     | -       |
    };

    enum High{
        //All the values for an active High Pass also contain the bit that activates High Pass itself.

                            //OSR =  800 Hz | 400 Hz | 200 Hz | 100 Hz | 50 Hz | 25 Hz | 12.5 Hz |
    HIGH0   = 0b001<<2,   //       15     | 7.5    | 3.75   | 1.875  | 0.937 | 0.468 | 0.234   |                                            
    HIGH1   = 0b011<<2,   //       7.7    | 3.85   | 1.925  | 0.963  | 0.481 | 0.241 | 0.120   |
    HIGH2   = 0b101<<2,   //       3.9    | 1.95   | 0.975  | 0.488  | 0.244 | 0.122 | 0.061   |
    HIGH3   = 0b111<<2,   //       1.98   | 0.99   | 0.495  | 0.248  | 0.124 | 0.062 | 0.031   |
    HIGHOFF = 0b000<<2
    };

    enum Range{
                                    //  Nominal Sensitivity (mdps/LSB)
        RANGE2000   = 0b000,       //  62.5
        RANGE1000   = 0b001,       //  31.25
        RANGE500    = 0b010,       //  15.625
        RANGE250    = 0b011,       //  7.8125
        
        RANGE4000   = 0b100,       //  125.0
    };

    enum Mode{

        STANDBY = 0b00,
        READY   = 0b01,
        ACTIVE  = 0b11
    };

    enum ODR{
        ODR800 = 0<<2,
        ODR400 = 1<<2,
        ODR200 = 2<<2,
        ODR100 = 3<<2,
        ODR50  = 4<<2,
        ODR25  = 5<<2,
        ODR12  = 6<<2
    };
}
#endif