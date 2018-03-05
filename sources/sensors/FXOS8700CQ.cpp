#include "FXOS8700CQ_enum.h"
#include "FXOS8700CQ.h"
#include "mbed.h"

using namespace FXO;

const float FXOS8700CQ::TEMPERATURE_SENSITIVITY = 0.96;

FXOS8700CQ::FXOS8700CQ(FXOS8700CQ_Mode mode, FXOS8700CQ_Range range, FXOS8700CQ_ODR dataRate) : mI2C(PTC11, PTC10), mAddress(0x1E),
 mInterruptOne(PTC1), mInterruptTwo(PTD13), mReset(PTD11){
     mI2C.frequency(400000);
     setMode(mode);
     setRange(range);
     setODR(dataRate);
}

void FXOS8700CQ::setMode(FXOS8700CQ_Mode mode){
    uint8_t data;
    if(mode==STANDBY){
        read(CTRL_REG_1, &data);
        data&=~1;
        write(CTRL_REG_1, &data);
        mMode = STANDBY;
    }
    else{
        read(MAG_CTRL_REG_1, &data);
        data&=~3;
        data|=mode;
        write(MAG_CTRL_REG_1, &data);
        read(CTRL_REG_1, &data);
        data&=~1;
        write(CTRL_REG_1, &data);
        if(mMode==HYBRID){mSensitivity*=2;}
        else if(mode==HYBRID){mSensitivity/=2;}
        mMode = mode;
    }
}

void FXOS8700CQ::standby(){
    setMode(STANDBY);
}

void FXOS8700CQ::setAccelerometer(){
    setMode(ACCELEROMETER);
}

void FXOS8700CQ::setMagnetometer(){
    setMode(MAGNETOMETER);
}

void FXOS8700CQ::setHybrid(){
    setMode(HYBRID);
}

FXOS8700CQ_Mode FXOS8700CQ::getStatus(){
    uint8_t data;
    read(CTRL_REG_1, &data);
    if((data&1)==0){return STANDBY;}
    read(MAG_CTRL_REG_1, &data);
    mMode = (FXOS8700CQ_Mode) (data&3);
    return (FXOS8700CQ_Mode) (data&3);
}

bool FXOS8700CQ::isDataAvailable(){
    //TODO
}


void FXOS8700CQ::setRange(FXOS8700CQ_Range range){
    // TODO Implement the low noise mode in CTRL_REG_1 and deactivate it when the user asks for max range, 
    // TODO as it doesn't work at max range
    uint8_t data;
    read(DATA_CFG, &data);
    data&=~3;
    data|=range;
    write(DATA_CFG, &data);
}

void FXOS8700CQ::setODR(FXOS8700CQ_ODR dataRate){
    uint8_t data;
    read(CTRL_REG_1, &data);
    data&=~ODR1;
    data|=dataRate;
    FXOS8700CQ_Mode tempMode = mMode;
    standby();
    write(CTRL_REG_1, &data);
    mODR = dataRate;
    setMode(tempMode);
}

void FXOS8700CQ::setMagOversample(FXOS8700CQ_OSR oversample){
    uint8_t data;
    read(MAG_CTRL_REG_1, &data);
    data&=~(OSR_7<<2);
    data|=(oversample<<2);
}

void FXOS8700CQ::setAccOversampleAwake(FXOS8700CQ_Acc_OSR oversample){
    uint8_t data;
    read(CTRL_REG_2, &data);
    data&=~(0b11);
    data|=oversample;
    write(CTRL_REG_2, &data);
}

void FXOS8700CQ::setAccOversampleAsleep(FXOS8700CQ_Acc_OSR oversample){
    uint8_t data;
    read(CTRL_REG_2, &data);
    data&=~(0b11<<3);
    data|=oversample<<3;
    write(CTRL_REG_2, &data);
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
    if(mMode!=ACCELEROMETER){
        uint8_t data;
        read(TEMPERATURE, &data);
        return TEMPERATURE_SENSITIVITY * (int)(~data + 1);
    }
    return -300.; // Not pretty, but a number under 0 K should be detected by well written code. 
}



void FXOS8700CQ::setInterrupt(FXOS8700CQ_Interrupt_Pin pin, FXOS8700CQ_Interrupt name, void (*function)(), float threshold, int count, bool resetCount){
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
    }
    read(CTRL_REG_2, &data);
    data|=(1<<(2*name));
    if(pin == PIN_ONE){data|=(1<<(2*name+1));}
    else{data&=~(1<<(2*name+1));}
    write(CTRL_REG_2, &data);
    if(pin==PIN_ONE){activeInterruptsOne |= name;
        mInterruptOne.fall(callback(this, &FXOS8700CQ::dispatchInterruptDataOne));
    }
    else{activeInterruptsTwo &= ~name;
        mInterruptTwo.fall(callback(this, &FXOS8700CQ::dispatchInterruptDataTwo));
    }
    setInterruptFunction(function, pin);
    setReady();
}

void FXOS8700CQ::removeInterrupt(FXOS8700CQ_Interrupt name){
    uint8_t data;
    if(name==I_FIFO){
//TODO
    }
    read(CTRL_REG_2, &data);
    data &= ~(1<<(2*name));
    write(CTRL_REG_2, &data);
    if(activeInterruptsOne&name){activeInterruptsOne &= ~name;}
    else{activeInterruptsTwo &=~ name;}
    if(!activeInterruptsOne){
        mInterruptOne.fall(NULL);
        mThreadOne.terminate();
        FXOS8700CQInterruptOne = NULL;
    }
    if(!activeInterruptsTwo){
        mInterruptTwo.fall(NULL);
        mThreadTwo.terminate();
        FXOS8700CQInterruptTwo = NULL;
    }
}

void FXOS8700CQ::setInterruptFunction(void (*function)(), FXOS8700CQ_Interrupt_Pin pin){
    if(pin){
        FXOS8700CQInterruptOne = function;
        mThreadOne.start(callback(this, &FXOS8700CQ::interruptWrapperOne));
    }
    else{
        FXOS8700CQInterruptTwo = function;
        mThreadTwo.start(callback(this, &FXOS8700CQ::interruptWrapperTwo));
    }
}

void FXOS8700CQ::interruptWrapper(FXOS8700CQ_Interrupt_Pin pin){
    while(1){
        //TODO
        Thread::signal_wait(0x01);
        switch(identifyInterrupt(pin)){
            case I_NEW_DATA: {
                uint8_t data;
                read(DR_STATUS, &data);
                float *samples = new float[3];
                samples=getAngles();
                for(int i=0;i<3;i++){
                    if(data&(1<<i)){
                        mail_t *mail = mailBox.alloc();
                        mail->axis = (FXOS8700CQ_Axis) i;
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
                    (*(mailArray+i))->axis = (FXOS8700CQ_Axis) (i%3);
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
        if(pin){FXOS8700CQInterruptOne();}
        else{FXOS8700CQInterruptTwo();}
    }
}

void FXOS8700CQ::dispatchInterruptDataOne(){
        dispatchInterruptData(PIN_ONE);
}
    
void FXOS8700CQ::dispatchInterruptDataTwo(){
        dispatchInterruptData(PIN_TWO);
}

FXOS8700CQ_Interrupt FXOS8700CQ::identifyInterrupt(FXOS8700CQ_Interrupt_Pin pin){
    uint8_t data;
    read(INTERRUPT_STATUS, &data);
    if(pin==PIN_ONE){return (FXOS8700CQ_Interrupt) (activeInterruptsOne & data);}
    else{return (FXOS8700CQ_Interrupt) (activeInterruptsTwo & data);}
    }

void FXOS8700CQ::dispatchInterruptData(FXOS8700CQ_Interrupt_Pin pin){
    if(pin){mThreadOne.signal_set(0x01);}
    else{mThreadTwo.signal_set(0x01);}
}



uint8_t* FXOS8700CQ::getRawAcceleration(){
    //TODO Determine if the data is 3 or 6 bytes depending on the current precision
    uint8_t *data = new uint8_t[6];
    read(ACC_X_MSB, data, 6);
    return data;
}

uint8_t* FXOS8700CQ::getRawMagnetic(){
    //TODO Determine if the data is 3 or 6 bytes depending on the current precision
    uint8_t *data = new uint8_t[6];
    read(MAG_X_MSB, data, 6);
    return data;
}


float* FXOS8700CQ::convertAcceleration(uint8_t *rawAcc){
//TODO Needs to check whether we have 3 or 6 bytes to convert.
    float *result = new float[3];
    for(uint8_t i=0;i<3;i++){
        result[i] = mSensitivity * (~(((rawAcc[i])<<8)  + ((rawAcc[i+1])>>2)) + 1);
    }
    return result;
}

float* FXOS8700CQ::convertMagnetic(uint8_t *rawMag){
//TODO Needs to check whether we have 3 or 6 bytes to convert.
    float *result = new float[3];
        for(uint8_t i=0;i<3;i++){
            result[i] = mSensitivity * (~(((rawMag[i])<<8)  + ((rawMag[i+1])>>2)) + 1);
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