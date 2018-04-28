#ifndef _HTU21D_ENUM_
#define _HTU21D_ENUM_

namespace HTU{
    enum Command{
        WAIT_FOR_TEMPERATURE    = 0xE3,
        WAIT_FOR_HUMIDITY       = 0xE5,
        MEASURE_TEMPERATURE     = 0xF3,
        MEASURE_HUMIDITY        = 0xF5,
        WRITE            = 0xE6,
        READ             = 0xE7,
        RESET            = 0xFE
    };

    enum Resolution{
        H12_T14 = 0x00,
        H8_T12  = 0x01,
        H10_T13 = 0x80,
        H11_T11 = 0x81
    };
}
#endif