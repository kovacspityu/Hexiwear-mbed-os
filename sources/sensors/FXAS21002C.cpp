#include "FXAS21002C_enum.h"
#include "FXAS21002C.h"
#include "mbed.h"

using namespace FXA;

FXAS21002C::FXAS21002C(Range range, ODR dataRate): 
mI2C(I2C_SDA, I2C_SCL), mAddress(0x40), mInterruptOne(PTD1), mInterruptTwo(PTC18){
    standby();
    setRange(range);
    setODR(dataRate);
    mSensitivity = 125/(range&0b11);
    if(range&4){mSensitivity*=2;}
    setReady();
}

void FXAS21002C::setMode(Mode mode){
    switch(mode){
        case STANDBY : {
            standby();
            break;
        }
        case READY : {
            setReady();
            break;
        }
        case ACTIVE : {
            setActive();
            break;
        }
    }
}

void FXAS21002C::standby(){
    uint8_t data;
    read(CTRL_REG_1, &data);
    data&=~3;
    write(CTRL_REG_1, &data);
    mMode = STANDBY;
}

void FXAS21002C::setActive(){
    uint8_t data;
    read(CTRL_REG_1, &data);
    int waitingTime;
    switch(data&=0b11){
        case ACTIVE: {
            waitingTime = 0;
            break;
        }
        case READY: {
            waitingTime = 1/(float)mODR + 5;
            break;
        }
        case STANDBY: {
            waitingTime = 1/(float)mODR + 60;
            break;
        }
    }
    if(waitingTime){
        data|=3;
        write(CTRL_REG_1, &data);
        wait_ms(waitingTime);
    }
    mMode = ACTIVE;
}

void FXAS21002C::setReady(){
    uint8_t data;
    read(CTRL_REG_1, &data);
    data&=~3;
    data|=1;
    write(CTRL_REG_1, &data);
    mMode = READY;
}

Mode FXAS21002C::getStatus(){
    uint8_t data;
    read(CTRL_REG_1, &data);
    data&=0b11;
    mMode = (Mode) data;
    return mMode;
}

bool FXAS21002C::isDataAvailable(){
    uint8_t data;
    read(DR_STATUS, &data);
    return data&0b00001000;
}

void FXAS21002C::setRange(Range range){
    Mode tempMode = mMode;
    setReady();
    uint8_t data;
    if(range==RANGE4000){
        read(CTRL_REG_3, &data);
        if(!(data&1)){
            data|=1;
            mSensitivity*=2;
            write(CTRL_REG_3, &data);
            range=(Range)(range&~4);
        }
    }
    read(CTRL_REG_0, &data);
    mSensitivity*=(1<<(data&0b11));
    mSensitivity/=(1<<range);
    data&=~0b11;
    data|=range;
    write(CTRL_REG_0, &data); 
    setMode(tempMode);
}

void FXAS21002C::setLowPass(Low threshold){
    Mode tempMode = mMode;
    setReady();
    uint8_t data;
    read(CTRL_REG_0, &data);
    data&=~0xC0;
    data|=threshold;
    write(CTRL_REG_0, &data);
    setMode(tempMode);
}

void FXAS21002C::setHighPass(High threshold){
    Mode tempMode = mMode;
    setReady();
    uint8_t data;
    read(CTRL_REG_0, &data);
    data&=~0x18;
    data|=threshold;
    write(CTRL_REG_0, &data);
    setMode(tempMode); 
}

void FXAS21002C::setODR(ODR dataRate){
    Mode tempMode = mMode;
    setReady();
    uint8_t data;
    read(CTRL_REG_1, &data);
    data&=~0x1C;
    data|=dataRate;
    write(CTRL_REG_1, &data);
    mODR = 800/(1<<(dataRate>>2));
    setMode(tempMode);
}

float *FXAS21002C::getAngles(){
    int16_t *data = new int16_t[3];
    data = getRawData();
    float *result = new float[3];
    for(int i=0;i<3;i++){
        result[i] = convertToAngle(data[i]);
    }
    delete[] data;
    return result;
}

float *FXAS21002C::getRadians(){
    int16_t *data;
    data = getRawData();
    float *result = new float[3];
    for(int i=0;i<3;i++){
        result[i] = convertToRadian(data[i]);
    }
    delete[] data;
    return result;
}

int8_t *FXAS21002C::getTemperature(){
    uint8_t *data = new uint8_t[1];
    read(TEMPERATURE, data);
    return (int8_t*)data;
}

int16_t *FXAS21002C::getRawData(){
    uint8_t *data = new uint8_t[6];
    read(X_ANGLE_MSB, data, 6);
    int16_t *result = new int16_t[3];
    for(int i=0; i<3;i++){
        result[i] = (int16_t) (((uint16_t)(data[2*i])<<8) + data[2*i+1]);
    }
    delete[] data;
    return result;
}

void FXAS21002C::setInterrupt(Interrupt_Pin pin, Interrupt name, void (*function)(), uint8_t count, bool resetCount, float threshold, uint8_t config){
    if(name==I_NO_INTERRUPT){return;}
    Mode tempMode = mMode;
    standby();
    uint8_t data;
    switch(name){
        case I_NEW_DATA: {
            break;
        }
        case I_FIFO: {
            // FIFO needs to be deactivated before changing its mode, as per docs.
            data = 0;
            write(FIFO_SETUP, &data);
            data = (count&63) | (1<<(6 + ( (uint8_t) (!resetCount) ) ));
            write(FIFO_SETUP, &data);
            read(CTRL_REG_3, &data);
            data|=1<<3;
            write(CTRL_REG_3, &data);
            break;
        }
        case I_THRESHOLD: {
            write(THRESHOLD_COUNTER, &count);
            threshold = fabs(threshold);
            data = (threshold/(256 * mSensitivity))-1;
            if(data>127){data=127;}
            data|=resetCount<<7;
            write(THRESHOLD_CONFIG, &data);
            data = config&15; 
            write(RT_INT_CONFIG, &data);
            break;
        }
        case I_NO_INTERRUPT: {break;}
    }
    read(CTRL_REG_2, &data);
    data|=(1<<(2*name));
    if(pin == PIN_ONE){data|=(1<<(2*name+1));}
    else{data&=~(1<<(2*name+1));}
    write(CTRL_REG_2, &data);
    activeInterrupts |= name;
    if(pin==PIN_ONE){
        mInterruptOne.fall(callback(this, &FXAS21002C::dispatchInterruptData));
    }
    else{
        mInterruptTwo.fall(callback(this, &FXAS21002C::dispatchInterruptData));
    }
    setInterruptFunction(function);
    setMode(tempMode);
}

void FXAS21002C::removeInterrupt(Interrupt name){
    Mode tempMode = mMode;
    standby();
    uint8_t data;
    if(name==I_FIFO){
        data = 0;
        write(FIFO_SETUP, &data);
    }
    read(CTRL_REG_2, &data);
    data &= ~(1<<(2*name));
    write(CTRL_REG_2, &data);
    activeInterrupts &= ~name;
    if(!activeInterrupts){
        mInterruptOne.fall(NULL);
        mThread.terminate();
        FXAS21002CInterrupt = NULL;
        mInterruptTwo.fall(NULL);
    }
    setMode(tempMode);
}

void FXAS21002C::setInterruptFunction(void (*function)()){
    FXAS21002CInterrupt = function;
    if(mThread.get_state()==Thread::Deleted){mThread.start(callback(this, &FXAS21002C::interruptWrapper));}
}

void FXAS21002C::interruptWrapper(){
    while(1){
        //TODO
        Thread::signal_wait(0x01);
        uint8_t name = identifyInterrupt();
        while(name){
            switch(name){
                case I_NEW_DATA: {
                    uint8_t data;
                    read(DR_STATUS, &data);
                    float *samples;
                    samples=getAngles();
                    for(int i=0;i<3;i++){
                        if(data&(1<<i)){
                            mail_t *mail = mailBox.alloc();
                            mail->axis = (Axis) i;
                            mail->value = samples[i];
                        }
                    }
                    delete[] samples;
                    break;
                }
                case I_FIFO: {
                    uint8_t samplesNumber;
                    read(FIFO_STATUS, &samplesNumber);
                    samplesNumber&=0x3F;
                    mail_t **mailArray = new mail_t*[3*samplesNumber];
                    for(int i=0;i<2*samplesNumber;i++){
                        *(mailArray+i)= mailBox.alloc();
                    }
                    uint8_t *samples = new uint8_t[6*samplesNumber];
                    read(X_ANGLE_MSB, samples, 6*samplesNumber);
                    for(int i=0;i<3*samplesNumber;i++){
                        (*(mailArray+i))->axis = (Axis) (i%3);
                        (*(mailArray+i))->value = convertToAngle(*samples+2*i);
                        
                        mailBox.put(*(mailArray+i));
                    }
                    delete[] samples;
                    break;
                }
                case I_THRESHOLD: {
                    //TODO
                    break;
                }
            }
            FXAS21002CInterrupt();
        }
        name = identifyInterrupt();
    }
}

Interrupt FXAS21002C::identifyInterrupt(){
    uint8_t data;
    read(INTERRUPT_STATUS, &data);
    for(uint8_t i=0;i<3;i++){
        if(data&(1<<i)){return (Interrupt) (1<<i);}
    }
    return I_NO_INTERRUPT;
}

void FXAS21002C::dispatchInterruptData(){
    mThread.signal_set(0x01);
}


float FXAS21002C::convertToAngle(int16_t rawAngle){
    return mSensitivity * rawAngle;
}

float FXAS21002C::convertToRadian(int16_t rawAngle){
    return mSensitivity * rawAngle * M_PI / 180;
}

int FXAS21002C::write(Address address, uint8_t *data, int length){
    uint8_t *bigData = new uint8_t[length+1];
    *bigData = address;
    for(int i = 0; i < length; i++){
        *(bigData+i+1) = data[i];
    }
    int result = mI2C.write(mAddress, (char*) bigData, length + 1);
    delete[] bigData;
    return result; 
}

void FXAS21002C::read(Address address, uint8_t *data, int length){
    uint8_t addressP[] = {address}; 
    mI2C.write(mAddress, (char*) addressP, 1, true);
    mI2C.read(mAddress, (char*) data, length);
}