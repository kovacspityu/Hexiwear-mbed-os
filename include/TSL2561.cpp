#include "TSL2561_enum.h"
#include "TSL2561.h"
#include "mbed.h"
#include "rtos.h"


TSL2561::TSL2561(TSL2561_GAIN gain, TSL2561_OS_RATE rate) : 
mI2C(I2C_SDA, I2C_SCL), mInterrupt(PTC0), mAddress(0x29 << 1), mGain(gain), mRate(rate)
{
    powerUp();
    uint8_t data = (gain<<4) | rate;
    write(TIMING, &data);
    mInterrupt.rise(callback(this, &TSL2561::dispatchInterruptData));
}

bool TSL2561::isActive(){
    uint8_t data;
    read(CONTROL, &data);
    return data;
}

double TSL2561::getLux(){
    uint8_t light[4];
    getRawLux(light);
    return formatLux(light);
}

void TSL2561::powerUp(){
    uint8_t data = 3;
    write(CONTROL, &data);
}

void TSL2561::powerDown(){
    uint8_t data = 0;
    write(CONTROL, &data);
}

void TSL2561::reset(){
    if(!isActive()){
        powerUp();
        wait_ms(5);
    }
    powerDown();
    powerUp();
}

void TSL2561::setGain(TSL2561_GAIN gain){
    uint8_t data;
    read(TIMING, &data);
    if(gain){data |= (gain<<4);}
    else{ data &= ~(gain<<4);}
    write(TIMING, &data);
    mGain = gain;
}

void TSL2561::setOSRate(TSL2561_OS_RATE rate){
    uint8_t data;
    read(TIMING, &data);
    data &= ~(3);
    data |= rate;
    write(TIMING, &data);
    mRate = rate;
}

/*void TSL2561::setInterrupt(float lowThreshold, float highThreshold, TSL2561_Interrupt_Length persistence){

}*/

void TSL2561::setInterrupt(int lowPercentage, int highPercentage, TSL2561_Interrupt_Length persistance, void (*function)()){
    uint8_t data[4];
    getRawLux(data);
    data[2] = uint8_t(lround(((data[0] / 100) * highPercentage)));
    data[3] = uint8_t(lround(((data[1] / 100) * highPercentage)));
    data[0] = uint8_t(lround(((data[0] / 100) * lowPercentage)));
    data[1] = uint8_t(lround(((data[1] / 100) * lowPercentage)));
    write(LOW_THRESHOLD_LSB, data, 4);
    uint8_t dummy = persistance | (1<<4);
    mThread.start(*function);
    write(INTERRUPT, &dummy);
    mInterruptFunction = function;
}

void TSL2561::removeInterrupt(){
    uint8_t dummy = 0;
    mThread.terminate();
    write(INTERRUPT, &dummy);
    mInterruptFunction = NULL;
}

void TSL2561::clearInterrupt(){
    uint8_t commandByte = 0b11000000;
    mI2C.write(mAddress, (char*) &commandByte, 1);
}

void TSL2561::setDebugInterrupt(void (*function)()){
    mThread.start(*function);
    uint8_t data = 3<<4;
    write(INTERRUPT, &data);
    mInterruptFunction = function;
}


void TSL2561::getRawLux(uint8_t *rawLight){
    read(WHOLE_DATA_LSB, rawLight, 4);
}

double TSL2561::formatLux(uint8_t *light){
    uint16_t allLight = (light[1]<<8) + light[0];
    uint16_t irLight = (light[3]<<8) + light[2];
    delete[] light;
    if(mGain){
        allLight<<=4;
        irLight<<=4;
    }
    switch(mRate){
        case OS_14MS :{
            allLight*=0.034;
            irLight*=0.034;
            break;
        }
        case OS_100MS :{
            allLight*=0.252;
            irLight*=0.252;
            break;
        }
        case OS_400MS :{
            break;
        }
    }
    if(!allLight){return 0.0f;}
    double ratio = irLight/allLight;
    if(ratio >= 0 && ratio <= 0.5){
        return 0.0304 * allLight - 0.062 * allLight * pow(ratio, 1.4);
    }
    else if(ratio > 0.5 && ratio <= 0.61){
        return 0.0224 * allLight - 0.031 * irLight;
    }
    else if(ratio > 0.61 && ratio <= 0.8){
        return 0.0128 * allLight - 0.0153 * irLight;
    }
    else if(ratio > 0.8 && ratio <= 1.3){
        return 0.00146 * allLight - 0.00112 * irLight;
    }
    else{
        return 0.0f;
    }
}

void TSL2561::dispatchInterruptData(){
    double *mail = mailBox.alloc();
    *mail = getLux();
    mailBox.put(mail);
    clearInterrupt();
    mThread.signal_set(0x01);
}

/* TODO Find how to attach autoAdjustGain to the interrupt,
as it is a TSL2561::void* instead of void*
void TSL2561::dispatchWrongSensitivity(){
    uint8_t data[4];
    getRawLux(data);
    //TODO Actually implement it using the OS intervals too.
    if(data[0] && data[1]){mFix = 0;}
    else{mFix = 1;}
}

void TSL2561::autoAdjustGain(){
    if(mFix){setGain(LOW_GAIN);}
    else{setGain(HIGH_GAIN);}
}*/

void TSL2561::read(TSL2561_Address address, uint8_t *data, int length){
    uint8_t commandByte = 0b10010000 | address;
    mI2C.write(mAddress, (char*) &commandByte, 1);
    mI2C.read(mAddress, (char*) data, length);
}

bool TSL2561::write(TSL2561_Address address, uint8_t *data, int length){
    uint8_t bigData[length+1];
    bigData[0] = 0b10100000 | address;
    for(int i = 0; i < length; i++){
        bigData[i+1] = data[i];
    }
    return mI2C.write(mAddress, (char*) bigData, length+1);
}