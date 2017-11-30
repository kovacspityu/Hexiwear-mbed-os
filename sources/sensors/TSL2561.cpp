#include "TSL2561_enum.h"
#include "TSL2561.h"
#include "mbed.h"
#include "rtos.h"


TSL2561::TSL2561(TSL2561_Gain gain, TSL2561_Os_Rate rate) : mPower(PTB12),
mI2C(PTB1, PTB0), mInterrupt(PTC0), mAddress(0x52), mGain(gain), mRate(rate)
{
    mPower = 0;
    mI2C.frequency(400000);
    powerUp();
    uint8_t data = (gain<<4) | rate;
    write(TSL_TIMING, &data);
    wait();
}

bool TSL2561::isActive(){
    uint8_t data;
    read(TSL_CONTROL, &data);
    return data;
}

float TSL2561::getLux(){
    uint8_t light[4];
    getRawLux(light);
    return formatLux(light);
}

void TSL2561::powerUp(){
    uint8_t data = 3;
    write(TSL_CONTROL, &data);
}

void TSL2561::powerDown(){
    uint8_t data = 0;
    write(TSL_CONTROL, &data);
}

void TSL2561::reset(){
    if(!isActive()){
        powerUp();
        wait_ms(5);
    }
    powerDown();
    powerUp();
}

void TSL2561::setGain(TSL2561_Gain gain){
    uint8_t data;
    read(TSL_TIMING, &data);
    if(gain){data |= (gain<<4);}
    else{ data &= ~(gain<<4);}
    write(TSL_TIMING, &data);
    mGain = gain;
}

void TSL2561::setOSRate(TSL2561_Os_Rate rate){
    uint8_t data;
    read(TSL_TIMING, &data);
    data &= ~(3);
    data |= rate;
    write(TSL_TIMING, &data);
    mRate = rate;
    wait();
}

/* TODO
void TSL2561::setInterrupt(float lowThreshold, float highThreshold, TSL2561_Interrupt_Length persistence){

}*/

void TSL2561::setInterrupt(int lowPercentage, int highPercentage, TSL2561_Interrupt_Length persistance, void (*function)()){
    uint8_t data[4];
    getRawLux(data);
    data[2] = uint8_t(lround(((data[0] / 100) * highPercentage)));
    data[3] = uint8_t(lround(((data[1] / 100) * highPercentage)));
    data[0] = uint8_t(lround(((data[0] / 100) * lowPercentage)));
    data[1] = uint8_t(lround(((data[1] / 100) * lowPercentage)));
    write(TSL_LOW_THRESHOLD_LSB, data, 4);
    uint8_t dummy = persistance | (1<<4);
    mInterrupt.rise(callback(this, &TSL2561::dispatchInterruptData));
    write(TSL_INTERRUPT, &dummy);
    setInterruptFunction(function);
}

void TSL2561::removeInterrupt(){
    uint8_t dummy = 0;
    mThread.terminate();
    write(TSL_INTERRUPT, &dummy);
    mInterruptFunction = NULL;
}

void TSL2561::clearInterrupt(){
    uint8_t commandByte = 0b11000000;
    mI2C.write(mAddress, (char*) &commandByte, 1);
}

void TSL2561::setDebugInterrupt(void (*function)()){
    mInterrupt.rise(callback(this, &TSL2561::dispatchInterruptData));
    uint8_t data = 3<<4;
    write(TSL_INTERRUPT, &data);
    setInterruptFunction(function);
}

void TSL2561::setInterruptFunction(void(*function)()){
    mInterruptFunction = function;
    mThread.start(callback(this, &TSL2561::interruptWrapper));
}

void TSL2561::interruptWrapper(){
    while(1){
        Thread::signal_wait(0x01);
        float *mail = mailBox.alloc();
        *mail = getLux();
        mailBox.put(mail);
        clearInterrupt();
        dispatchWrongSensitivity(*mail);
        mInterruptFunction();
    }
}


void TSL2561::getRawLux(uint8_t *rawLight){
    read(TSL_WHOLE_DATA_LSB, rawLight, 4);
}

float TSL2561::formatLux(uint8_t *light){
    uint16_t allLight = (light[1]<<8) + light[0];
    uint16_t irLight = (light[3]<<8) + light[2];
    float allLightF, irLightF;
    if(mGain){
        allLight<<=4;
        irLight<<=4;
    }
    switch(mRate){
        case TSL_OS_14MS :{
            allLightF = ((float) allLight) * 0.034;
            irLightF = ((float) irLight) * 0.034;
            break;
        }
        case TSL_OS_100MS :{
            allLightF = ((float) allLight) * 0.252;
            irLightF = ((float ) irLight) * 0.252;
            break;
        }
        case TSL_OS_400MS :{
            allLightF = (float) allLight;
            irLightF = (float) irLight;
            break;
        }
    }
    if(!allLight){return 0.0f;}
    float ratio = irLightF/allLightF;
    if(ratio >= 0 && ratio <= 0.5){
        return 0.0304 * allLightF - 0.062 * allLightF * pow(ratio, 1.4);
    }
    else if(ratio > 0.5 && ratio <= 0.61){
        return 0.0224 * allLightF - 0.031 * irLightF;
    }
    else if(ratio > 0.61 && ratio <= 0.8){
        return 0.0128 * allLightF - 0.0153 * irLightF;
    }
    else if(ratio > 0.8 && ratio <= 1.3){
        return 0.00146 * allLightF - 0.00112 * irLightF;
    }
    else{
        return 0.0f;
    }
}

void TSL2561::dispatchInterruptData(){
    mThread.signal_set(0x01);
}

void TSL2561::dispatchWrongSensitivity(float lux){
    if(lux<0.1f){
        if(mGain == TSL_LOW_GAIN){setGain(TSL_HIGH_GAIN);}
        else{if(mRate!=TSL_OS_400MS){setOSRate((TSL2561_Os_Rate)(mRate+1));}}
        return;
    }
    if(lux>=20500){
            if((mRate==TSL_OS_14MS)&&(mGain==TSL_HIGH_GAIN)){setGain(TSL_LOW_GAIN);}
            else if(mRate!=TSL_OS_14MS){setOSRate((TSL2561_Os_Rate)(mRate-1));}
    }
}

void TSL2561::wait(){
    int waitingTime;
    switch(mRate){
        case TSL_OS_14MS: {
            waitingTime = 14;
            break;
        }
        case TSL_OS_100MS: {
            waitingTime = 101;
            break;
        }
        case TSL_OS_400MS: {
            waitingTime = 402;
            break;
        }
    }
    wait_us(waitingTime);
}

void TSL2561::read(TSL2561_Address address, uint8_t *data, int length){
    mI2C.write(mAddress, (char*) &address, 1);
    mI2C.read(mAddress, (char*) data, length);
}

int TSL2561::write(TSL2561_Address address, uint8_t *data, int length){
    uint8_t *bigData = new uint8_t[length+1];
    *bigData = address;
    for(int i = 0; i < length; i++){
        *(bigData+i+1) = data[i];
    }
    int result = mI2C.write(mAddress, (char*) bigData, length + 1);
    delete[] bigData;
    return result; 
}