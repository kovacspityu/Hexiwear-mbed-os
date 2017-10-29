#ifndef _HTU21D_ENUM_
#define _HTU21D_ENUM_

enum HTU21D_Command{
    WAIT_FOR_TEMPERATURE    = 0xE3,
    WAIT_FOR_HUMIDITY       = 0xE5,
    MEASURE_TEMPERATURE     = 0xF3,
    MEASURE_HUMIDITY        = 0xF5,
    HTU21D_WRITE            = 0xE6,
    HTU21D_READ             = 0xE7,
    HTU21D_RESET            = 0xFE
};

enum HTU21D_Resolution{
    H12_T14 = 0x00,
    H8_T12  = 0x01,
    H10_T13 = 0x80,
    H11_T11 = 0x81
};

#endif