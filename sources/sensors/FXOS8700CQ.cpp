#include "FXOS8700CQ_enum.h"
#include "FXOS8700CQ.h"
#include "mbed.h"

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
    standby();
    write(FXO_CTRL_REG_1, &data);
    mODR = dataRate;
}

float* FXOS8700CQ::getAcceleration(){
    int16_t *data = getRawAcceleration();
    float *result = convertAcceleration(data);
    delete[] data;
    return result;
}

float* FXOS8700CQ::getAcceleration(){
    int16_t *data = getRawMagnetic();
    float *result = convertMagnetic(data);
    delete[] data;
    return result;
}


int16_t* FXOS8700CQ::getRawAcceleration(){
    uint8_t *data = new uint8_t[6];
    read(FXO_ACC_X_MSB, data, 6);
    int16_t result = new int16_t[3];
    //TODO Convert to int16_t
    delete[] data;
    return result;
}

float* FXOS8700CQ::getRawMagnetic(){
    uint8_t *data = new uint8_t[6];
    read(FXO_MAG_X_MSB, data, 6);
    int16_t result = new int16_t[3];
    //TODO Convert to int16_t
    delete[] data;
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