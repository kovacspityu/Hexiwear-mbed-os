#include "MPL3115A2.h"
#include "mbed.h"

MPL3115A2::MPL3115A2(MPL3115A2_Mode mode, MPL3115A2_Oversample_Ratio ratio)
{
    uint8_t data = 1  + ratio<<3 + mode<<7;
    write(CTRL_REG_1, &data);
}

int MPL3115A2::write(MPL3115A2_Address address, uint8_t *data){
    const char bigData[2] = {(char) address, (char) *data};
    mI2C.write(mAddress, bigData, 2); 
}

uint8_t MPL3115A2::read(MPL3115A2_address address, uint8_t *data, int length){
    mI2C.write(mAddress, &address, 1, true);
    mI2C.read(mAddress, (char*) data, length);
}

int MPL3115A2::getOsR(){
    uint8_t data;
    read()
}
