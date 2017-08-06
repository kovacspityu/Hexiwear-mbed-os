#include "MPL3115A2_enum.h"
#include "MPL3115A2.h"
#include "mbed.h"

MPL3115A2::MPL3115A2(MPL3315A2_Mode mode, MPL3315A2_Oversample_Ratio ratio) : mI2C(PTC11, PTC10), mAddress(0xC0)
{
    uint8_t data = 1  + (ratio<<3) + (mode<<7);
    write(CTRL_REG_1, &data);
}

int MPL3115A2::write(MPL3115A2_Address address, uint8_t *data){
    const char bigData[2] = {(char) address, (char) *data};
    return mI2C.write(mAddress, bigData, 2); 
}

void MPL3115A2::read(MPL3115A2_Address address, uint8_t *data, int length){
    mI2C.write(mAddress, (*char) &address, 1, true);
    mI2C.read(mAddress,  data, length);
}

int MPL3115A2::getOsR(){
    uint8_t data;
    read(CTRL_REG_1, &data, 1);
    return (data&&0b00111000)>>3;
}

void MPL3115A2::setOsR(MPL3115A2_Oversample_Ratio ratio){
    uint8_t data;
    read(CTRL_REG_1, &data, 1);
    data&&~1;
    write(CTRL_REG_1, &data);
    data&&0b11000111;
    data||(ratio<<3 + 1);
    write(CTRL_REG_1, &data);
}

bool MPL3115A2::isAltimeter(){
    uint8_t data;
    read(CTRL_REG_1, &data, 1);
    return (data&&(1<<7));
}

double MPL3115A2::getData(){
    uint8_t data[3];
    double returnValue;
    read(PRESSURE_MSB, &data, 3);
    if (isAltimeter()){
        returnValue =    (
                            (((uint32_t) data[0]) << 24) || 
                            (((uint32_t) data[1]) << 16) || 
                            (((uint32_t) data[2]) << 8)
                            ) / 65536;
    }
    else{
        returnValue =    (
                            (((uint32_t) data[0]) << 16) || 
                            (((uint32_t) data[1]) << 8) || 
                            ((uint32_t) data[2])
                            ) / 64;
    }
    return returnValue;
}

double MPL3115A2::getTemperature(){
    uint8_t data[2];
    double returnValue;
    read(TEMPERATURE_MSB, &data, 2);
    returnValue =        (
                            (((uint16_t) data[3]) << 8) ||
                            ((uint16_t) data[4])
                            ) / 256;
    return returnValue;
}

double MPL3115A2::getDataDelta(){
    uint8_t data[3];
    double returnValue;
    read(PRESSURE_DELTA_MSB, &data, 3);
    if (isAltimeter()){
        returnValue =    (
                            (((uint32_t) data[0]) << 24) || 
                            (((uint32_t) data[1]) << 16) || 
                            (((uint32_t) data[2]) << 8)
                            ) / 65536;
    }
    else{
        returnValue =    (
                            (((uint32_t) data[0]) << 16) || 
                            (((uint32_t) data[1]) << 8) || 
                            ((uint32_t) data[2])
                            ) / 64;
    }
    return returnValue;
}

double MPL3115A2::getTemperatureDelta(){
    uint8_t data[2];
    double returnValue;
    read(TEMPERATURE_DELTA_MSB, &data, 2);
    returnValue =        (
                            (((uint16_t) data[3]) << 8) ||
                            ((uint16_t) data[4])
                            ) / 256;
    return returnValue;
}

double MPL3115A2::getMaxData(){
    uint8_t data[3];
    double returnValue;
    read(MAX_PRESSURE_MSB, &data, 3);
    if (isAltimeter()){
        returnValue =    (
                            (((uint32_t) data[0]) << 24) || 
                            (((uint32_t) data[1]) << 16) || 
                            (((uint32_t) data[2]) << 8)
                            ) / 65536;
    }
    else{
        returnValue =    (
                            (((uint32_t) data[0]) << 16) || 
                            (((uint32_t) data[1]) << 8) || 
                            ((uint32_t) data[2])
                            ) / 64;
    }
    return returnValue;
}
double MPL3115A2::getMaxTemperature(){
    uint8_t data[2];
    double returnValue;
    read(MAX_TEMPERATURE_MSB, &data, 2);
    returnValue =        (
                            (((uint16_t) data[3]) << 8) ||
                            ((uint16_t) data[4])
                            ) / 256;
    return returnValue;
}
double MPL3115A2::getMinData(){
    uint8_t data[3];
    double returnValue;
    read(MIN_PRESSURE_MSB, &data, 3);
    if (isAltimeter()){
        returnValue =    (
                            (((uint32_t) data[0]) << 24) || 
                            (((uint32_t) data[1]) << 16) || 
                            (((uint32_t) data[2]) << 8)
                            ) / 65536;
    }
    else{
        returnValue =    (
                            (((uint32_t) data[0]) << 16) || 
                            (((uint32_t) data[1]) << 8) || 
                            ((uint32_t) data[2])
                            ) / 64;
    }
    return returnValue;
}
double MPL3115A2::getMinTemperature(){
    uint8_t data[2];
    double returnValue;
    read(MAX_TEMPERATURE_MSB, &data, 2);
    returnValue =        (
                            (((uint16_t) data[3]) << 8) ||
                            ((uint16_t) data[4])
                            ) / 256;
    return returnValue;
}