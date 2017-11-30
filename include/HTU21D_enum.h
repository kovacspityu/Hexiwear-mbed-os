#ifndef _HTU21D_ENUM_
#define _HTU21D_ENUM_

enum HTU21D_Command{
    HTU_WAIT_FOR_TEMPERATURE    = 0xE3,
    HTU_WAIT_FOR_HUMIDITY       = 0xE5,
    HTU_MEASURE_TEMPERATURE     = 0xF3,
    HTU_MEASURE_HUMIDITY        = 0xF5,
    HTU_WRITE            = 0xE6,
    HTU_READ             = 0xE7,
    HTU_RESET            = 0xFE
};

enum HTU21D_Resolution{
    HTU_H12_T14 = 0x00,
    HTU_H8_T12  = 0x01,
    HTU_H10_T13 = 0x80,
    HTU_H11_T11 = 0x81
};

#endif