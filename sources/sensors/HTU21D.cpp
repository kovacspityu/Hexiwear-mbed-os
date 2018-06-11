#include "HTU21D_enum.h"
#include "HTU21D.h"
#include "mbed.h"

using namespace HTU;

HTU21D::HTU21D(Resolution resolution) : mPower(PTB12), mI2C(PTB1, PTB0),
mAddress(0x80), mResolution(resolution), temperatureDelta(-1) {
    mPower = 0;
    mI2C.frequency(400000);
    reset();
    setResolution(resolution);
    waitForTemperature();
}

float HTU21D::waitForTemperature(){
    uint8_t dummy[1] = {WAIT_FOR_TEMPERATURE};
    uint8_t data[3];
    mI2C.write(mAddress, (char*)dummy, 1, true);
    mI2C.read(mAddress, (char*)data, 3);
    if(checkForErrors(data)){return waitForTemperature();}
    data[2]&=~3;
    return convertTemperature(data);
}

float HTU21D::waitForHumidity(){
    uint8_t dummy[1] = {WAIT_FOR_HUMIDITY};
    uint8_t data[3];
    mI2C.write(mAddress, (char*)dummy, 1, true);
    mI2C.read(mAddress, (char*)data, 3);
    if(checkForErrors(data)){return waitForHumidity();}
    data[2]&=~3;
    return convertHumidity(data);
 }

float HTU21D::getTemperature(){
    uint8_t dummy[1] = {MEASURE_TEMPERATURE};
    uint8_t data[3];
    mI2C.write(mAddress, (char*)dummy, 1);
    wait_ms(50);
    while(mI2C.read(mAddress, (char*)data, 3)){};
    if(checkForErrors(data)){return getTemperature();}
    return convertTemperature(data);
}

float HTU21D::getHumidity(){
    uint8_t dummy[1] = {MEASURE_HUMIDITY};
    uint8_t data[3];
    mI2C.write(mAddress, (char*)dummy, 1);
    wait_ms(16);
    while(mI2C.read(mAddress, (char*)data, 2)){};
    if(checkForErrors(data)){return getHumidity();}
    return convertHumidity(data);
}

void HTU21D::setResolution(Resolution resolution){
    uint8_t dummy[1] = {READ};
    uint8_t data[2];
    mI2C.write(mAddress, (char*)dummy, 1, true);
    mI2C.read(mAddress, (char*)data, 2);
    data[1] = data[0]&0b01111110;
    data[1] &= resolution;
    data[0] = dummy[0];
    mI2C.write(mAddress, (char*)data, 2);
    mResolution = resolution;
}

void HTU21D::setTemperatureDelta(float temperature){
    temperatureDelta = temperature;
}

void HTU21D::reset(){
    uint8_t dummy[1] = {RESET};
    mI2C.write(mAddress, (char*)dummy, 1);
    wait_ms(15);
}

float HTU21D::convertHumidity(uint8_t *rawHumidity){
    float result = -6 + ((float) ((rawHumidity[0]<<8) | (rawHumidity[1]))*125) / 65536.0f;
    result+= (25.0 - mTemperature) * TEMPERATURE_COEFFICIENT; //Correction described in the docs
    if(temperatureDelta>0){result -= temperatureDelta/(mTemperature - HEAT_TRANSFER_COEFFICIENT*temperatureDelta);}
    return result;
}

float HTU21D::convertTemperature(uint8_t *rawTemperature){
    float result = -46.85 + ( ((float) ((rawTemperature[0]<<8) | rawTemperature[1]))*175.72 ) / 65536;
    mTemperature = result;
    if(temperatureDelta>0){result-= HEAT_TRANSFER_COEFFICIENT*temperatureDelta;} //Correction due to heat coming from other electronics, found experimentally
    return result;
}

bool HTU21D::checkForErrors(uint8_t *data){
    uint32_t paddedData = (((uint32_t)data[0])<<16) + (((uint32_t)data[1])<<8) + data[2];
    for(int i=0;paddedData&0xFFF00;i++){
        if(!(paddedData&(1<<(23-i)))){
            continue;
        }
        else{
            paddedData^=(CRC_PATTERN<<(15-i));
        }
    }
    return paddedData;
}