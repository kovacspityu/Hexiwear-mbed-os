#include "FXOS8700CQ_enum.h"
#include "FXOS8700CQ.h"
#include "mbed.h"

FXOS8700CQ::TEMPERATURE_SENSITIVITY = 0.96;

FXOS8700CQ::FXOS8700CQ(FXOS8700CQ_Mode mode, FXOS8700CQ_Range range, FXOS8700CQ_ODR dataRate) : mI2C(PTC11, PTC10), mAddress(0x1E),
 mInterruptOne(PTC1), mInterruptTwo(PTD13), mReset(PTD11){
     mI2C.frequency(400000);
     setMode(mode);
     setRange(range);
     setODR(dataRate);
}

void FXOS8700CQ::standby(){
    uint8_t data;
    read(FXO_CTRL_REG_1, &data);
    data&=~1;
    write(FXO_CTRL_REG_1, &data);
    mMode = FXO_STANDBY;
}

void FXOS8700CQ::setAccelerometer(){
    uint8_t data;
    read(FXO_MAG_CTRL_REG_1, &data);
    data&=~3;
    write(FXO_MAG_CTRL_REG_1, &data);
    read(FXO_CTRL_REG_1, &data);
    data&=~1;
    write(FXO_CTRL_REG_1, &data);
    if(mMode==FXO_HYBRID){mSensitivity*=2;}
    mMode = FXO_ACCELEROMETER;
}

void FXOS8700CQ::setMagnetometer(){
    uint8_t data;
    read(FXO_MAG_CTRL_REG_1, &data);
    data&=~3;
    data|=1;
    write(FXO_MAG_CTRL_REG_1, &data);
    read(FXO_CTRL_REG_1, &data);
    data&=~1;
    write(FXO_CTRL_REG_1, &data);
    if(mMode==FXO_HYBRID){mSensitivity*=2;}
    mMode = FXO_MAGNETOMETER;
}

void FXOS8700CQ::setHybrid(){
    uint8_t data;
    read(FXO_MAG_CTRL_REG_1, &data);
    data|=3;
    write(FXO_MAG_CTRL_REG_1, &data);
    read(FXO_CTRL_REG_1, &data);
    data&=~1;
    write(FXO_CTRL_REG_1, &data);
    mSensitivity/=2;
    mMode = FXO_HYBRID;
}

FXOS8700CQ_Mode FXOS8700CQ::getStatus(){
    uint8_t data;
    read(FXO_CTRL_REG_1, &data);
    if(data&1==0){return FXO_STANDBY;}
    read(FXO_MAG_CTRL_REG_1, &data);
    mMode = static_cast<FXOS8700CQ_Mode> data&3;
    return static_cast<FXOS8700CQ_Mode> data&3;
}

void FXOS8700CQ::setODR(FXOS8700CQ_ODR dataRate){
    uint8_t data;
    read(FXO_CTRL_REG_1, &data);
    data&=~FXO_ODR1;
    data|=dataRate;
    FXOS8700CQ_Mode tempMode = mMode;
    standby();
    write(FXO_CTRL_REG_1, &data);
    mODR = dataRate;
    setMode(tempMode);
}

void FXOS8700CQ::setMagOversample(FXOS8700CQ_OSR oversample){
    uint8_t data;
    read(FXO_MAG_CTRL_REG_1, &data);
    data&=~(FXO_OSR_7<<2);
    data|=(oversample<<2);
}

float* FXOS8700CQ::getAcceleration(){
    uint8_t *data = getRawAcceleration();
    float *result = convertAcceleration(data);
    delete[] data;
    return result;
}

float* FXOS8700CQ::getMagnetic(){
    uint8_t *data = getRawMagnetic();
    float *result = convertMagnetic(data);
    delete[] data;
    return result;
}

float FXOS8700CQ::getTemperature(){
    if(mMode!=FXO_ACCELEROMETER){
        uint8_t data;
        read(FXO_TEMPERATURE, &data);
        return TEMPERATURE_SENSITIVITY * (int)(~data + 1);
    }
    return -300.; // Not pretty, but a number under 0 K should be detected by well written code. 
}


uint8_t* FXOS8700CQ::getRawAcceleration(){
    //TODO Determine if the data is 3 or 6 bytes depending on the current precision
    uint8_t *data = new uint8_t[6];
    read(FXO_ACC_X_MSB, data, 6);
    return data;
}

uint8_t* FXOS8700CQ::getRawMagnetic(){
    //TODO Determine if the data is 3 or 6 bytes depending on the current precision
    uint8_t *data = new uint8_t[6];
    read(FXO_MAG_X_MSB, data, 6);
    return data;
}


float FXOS8700CQ::convertAcceleration(uint8_t *rawAcc){
//TODO Needs to check whether we have 3 or 6 bytes to convert.
    float result[] = new float[3];
    for(uint8_t i=0;i<3;i++){
        result[i] = mSensitivity * (~((rawAcc[i])<<8  + (rawAcc[i+1])>>2) + 1);
    }
    return result;
}

float FXOS8700CQ::convertMagnetic(uint8_t *rawMag){
//TODO Needs to check whether we have 3 or 6 bytes to convert.
    float result[] = new float[3];
        for(uint8_t i=0;i<3;i++){
            result[i] = mSensitivity * (~((rawAcc[i])<<8  + (rawAcc[i+1])>>2) + 1);
        }
        return result;
}


int FXOS8700CQ::write(FXOS8700CQ_Address address, uint8_t *data, int length){
    uint8_t *bigData = new uint8_t[length+1];
    *bigData = address;
    for(int i = 0; i < length; i++){
        *(bigData+i+1) = data[i];
    }
    int result = mI2C.write(mAddress, (char*) bigData, length + 1);
    delete[] bigData;
    return result; 
}

void FXOS8700CQ::read(FXOS8700CQ_Address address, uint8_t *data, int length){
    uint8_t addressP[] = {address}; 
    mI2C.write(mAddress, (char*) addressP, 1, true);
    mI2C.read(mAddress, (char*) data, length);
}