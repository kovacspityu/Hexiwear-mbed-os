#include "FXOS8700CQ_enum.h"
#include "FXOS8700CQ.h"
#include "mbed.h"

void FXOS8700CQ::standby(){
    uint8_t data;
    read(FXO_CTRL_REG_1, &data);
    data&=~1;
    write(FXO_CTRL_REG_1, &data);
}

void FXOS8700CQ::setAccelerometer(){
    uint8_t data;
    read(FXO_MAG_CTRL_REG_1, &data);
    data&=~3;
    write(FXO_MAG_CTRL_REG_1, &data);
    read(FXO_CTRL_REG_1, &data);
    data&=~1;
    write(FXO_CTRL_REG_1, &data);
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
}

void FXOS8700CQ::setHybrid(){
    uint8_t data;
    read(FXO_MAG_CTRL_REG_1, &data);
    data|=3;
    write(FXO_MAG_CTRL_REG_1, &data);
    read(FXO_CTRL_REG_1, &data);
    data&=~1;
    write(FXO_CTRL_REG_1, &data);
}

FXOS8700CQ_Mode FXOS8700CQ::getStatus(){
    uint8_t data;
    read(FXO_CTRL_REG_1, &data);
    if(data&1==0){return FXO_STANDBY;}
    read(FXO_MAG_CTRL_REG_1, &data);
    return static_cast<FXOS8700CQ_Mode> data&3;
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