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

void FXAS21002C::standby(){
    uint8_t data;
    read(CTRL_REG_1, &data);
    data&=~3;
    write(CTRL_REG_1, &data);
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
}

void FXAS21002C::setReady(){
    uint8_t data;
    read(CTRL_REG_1, &data);
    data&=~3;
    data|=1;
    write(CTRL_REG_1, &data);
}

Mode FXAS21002C::getStatus(){
    uint8_t data;
    read(CTRL_REG_1, &data);
    data&=0b11;
    return (Mode)data;
}

bool FXAS21002C::isDataAvailable(){
    uint8_t data;
    read(DR_STATUS, &data);
    return data&0b00001000;
}

void FXAS21002C::setRange(Range range){
    bool mActive = false;
    if(getStatus()==ACTIVE){
        setReady();
        mActive = true;}
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
    if(mActive){
        setActive();
    }
}

void FXAS21002C::setLowPass(Low threshold){
    bool mActive = false;
    if(getStatus()==ACTIVE){
        setReady();
        mActive = true;}
    uint8_t data;
    read(CTRL_REG_0, &data);
    data&=~0xC0;
    data|=threshold;
    write(CTRL_REG_0, &data);
    if(mActive){
        setActive();
    }
}

void FXAS21002C::setHighPass(High threshold){
    bool mActive = false;
    if(getStatus()==ACTIVE){
        setReady();
        mActive = true;}
    uint8_t data;
    read(CTRL_REG_0, &data);
    data&=~0x18;
    data|=threshold;
    write(CTRL_REG_0, &data);
    if(mActive){
        setActive();
    }
}

void FXAS21002C::setODR(ODR dataRate){
    bool mActive = false;
    if(getStatus()==ACTIVE){
        setReady();
        mActive = true;
    }
    uint8_t data;
    read(CTRL_REG_1, &data);
    data&=~0x1C;
    data|=dataRate;
    write(CTRL_REG_1, &data);
    mODR = 800/(1<<(dataRate>>2));
    if(mActive){
        setActive();
    }
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
    int16_t *data = new int16_t[3];
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
    return result;
}

void FXAS21002C::setInterrupt(Interrupt_Pin pin, Interrupt name, void (*function)(), float threshold, int count, bool resetCount){
    if(name==I_NO_INTERRUPT){return;}
    standby();
    uint8_t data;
    switch(name){
        case I_NEW_DATA: {
            break;
        }
        case I_FIFO: {
            //TODO
            read(CTRL_REG_3, &data);
            data|=1<<3;
            write(CTRL_REG_3, &data);
            break;
        }
        case I_THRESHOLD: {
            uint8_t data = (count<256?count:255); 
            write(THRESHOLD_COUNTER, &data);
            if(count>127){count=127;}
            data = (count/256)-1;
            data|=resetCount<<7;
            write(THRESHOLD_CONFIG, &data);
            data=7;
            //TODO This register allows to select only certain axes for the interrupt, and to "latch" it.
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
    setReady();
}

void FXAS21002C::removeInterrupt(Interrupt name){
    uint8_t data;
    if(name==I_FIFO){
//TODO
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
}

void FXAS21002C::setInterruptFunction(void (*function)()){
    FXAS21002CInterrupt = function;
    if(mThread.get_state()==Thread::Deleted)mThread.start(callback(this, &FXAS21002C::interruptWrapper));
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
                    float *samples = new float[3];
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