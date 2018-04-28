#include "mbed.h"
#include "FXOS8700CQ_enum.h"
#include "FXOS8700CQ.h"


using namespace FXO;

const float FXOS8700CQ::TEMPERATURE_SENSITIVITY = 0.96;
const float FXOS8700CQ::MAGNETIC_SENSITIVITY = 0.1;
const float FXOS8700CQ::BASE_ACC_SENSITIVITY = 0.244;


FXOS8700CQ::FXOS8700CQ(Mode mode, Range range, 
ODR awakeODR, ODR asleepODR) : 
mI2C(PTC11, PTC10), mAddress(0x1E<<1), mInterruptOne(PTC1), mInterruptTwo(PTD13), mReset(PTD11){
     mI2C.frequency(400000);
     awake=true;
     for(uint8_t i=0;i<10;i++){
         mInterrupts[i] = NULL;
     }
     mAwakeODR = awakeODR;
     mSleepODR = asleepODR;
     mMode = STANDBY;
     setMode(mode);
     setRange(range);
     setAwakeODR(awakeODR);
     setAsleepODR(asleepODR);
     setInterrupt(PIN_TWO, I_SLEEP_WAKE, NULL);
     // Sets up the autoincrement mode for data.
     uint8_t data = 32;
     write(MAG_CTRL_REG_2, &data);
}

void FXOS8700CQ::setMode(Mode mode){
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
        data|=1;
        write(CTRL_REG_1, &data);
        // The ODR is halved in HYBRID mode.
        if(mMode==HYBRID && mode!=HYBRID){
            mODR = (ODR) (mODR/2);
        }
        // Go back to normal ODR if coming out of HYBRID mode.
        else if(mode==HYBRID){
            mODR = (ODR) (mODR*2);
        }
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

Mode FXOS8700CQ::getStatus(){
    uint8_t data;
    read(CTRL_REG_1, &data);
    if((data&1)==0){return STANDBY;}
    read(MAG_CTRL_REG_1, &data);
    mMode = (Mode) (data&3);
    return (Mode) (data&3);
}

bool FXOS8700CQ::isDataAvailable(){
    uint8_t data;
    read(STATUS, &data);
    return data ? true : false;
}


void FXOS8700CQ::setRange(Range range){
    mAccSensitivity = BASE_ACC_SENSITIVITY * (1<<range);
    uint8_t data;
    read(DATA_CFG, &data);
    data&=~3;
    data|=range;
    write(DATA_CFG, &data);
    if(range==RANGE2000){
        read(CTRL_REG_1, &data);
        data&=251;
        write(CTRL_REG_1, &data);
    }
}

void FXOS8700CQ::setAwakeODR(ODR dataRate){
    // ODR1_2 is the ODR when the requested ODR==ODR1 and the system is in hybrid mode,
    // otherwise it's not available
    if(dataRate==ODR1_2){dataRate=ODR1;}
    uint8_t data;
    read(CTRL_REG_1, &data);
    data&=0b11000111;
    data|=dataRate<<3;
    Mode tempMode = mMode;
    standby();
    write(CTRL_REG_1, &data);
    if(tempMode==HYBRID){mODR= (ODR) (dataRate+1);}
    mAwakeODR = dataRate;
    setMode(tempMode);
}

void FXOS8700CQ::setAsleepODR(ODR dataRate){
    if(dataRate==ODR1_2){dataRate=ODR1;}
    if(dataRate<ODR50){dataRate=ODR50;}
    mSleepODR=dataRate;
    uint8_t data;
    read(CTRL_REG_1, &data);
    data&=0b00111111;
    data|=(dataRate<<6);
    Mode tempMode = mMode;
    standby();
    write(CTRL_REG_1, &data);
    if(tempMode==HYBRID){mODR= (ODR) (dataRate+1);}
    setMode(tempMode);
}

bool FXOS8700CQ::setAccLowNoise(bool activated){
    if(mAccSensitivity!=0.976){
        uint8_t data;
        read(CTRL_REG_1, &data);
        if(activated){data|=4;}
        else{data&=~4;}
        write(CTRL_REG_1, &data);
        return true;
    }
    return false;
}

void FXOS8700CQ::setMagOversampleAwake(Mag_OSR oversample){
    uint8_t data;
    read(MAG_CTRL_REG_1, &data);
    data&=~(OSR_7<<2);
    data|=(oversample<<2);
    write(MAG_CTRL_REG_1, &data);
}

void FXOS8700CQ::setMagOversampleAsleep(Mag_OSR oversample){
    uint8_t data;
    read(MAG_CTRL_REG_3, &data);
    data&=~(OSR_7<<4);
    data|=(oversample<<4);
    write(MAG_CTRL_REG_3, &data);
}

void FXOS8700CQ::setAccOversampleAwake(Acc_OSR oversample){
    uint8_t data;
    read(CTRL_REG_2, &data);
    data&=~(0b11);
    data|=oversample;
    write(CTRL_REG_2, &data);
}

void FXOS8700CQ::setAccOversampleAsleep(Acc_OSR oversample){
    uint8_t data;
    read(CTRL_REG_2, &data);
    data&=~(0b11<<3);
    data|=oversample<<3;
    write(CTRL_REG_2, &data);
}

void FXOS8700CQ::setAccelerationMagnitude(Interrupt_Pin pin, void (*function)(), uint8_t count, bool resetCount, uint8_t config, float threshold, float* reference){
    uint16_t dummy = lround(fabs(threshold/mAccSensitivity));
    dummy&=0b0001111111111111;
    if(resetCount){dummy|=1<<15;}
    write(ACC_MAGNITUDE_MSB, (uint8_t*) &dummy, 2);
    for(uint i=0;i<3;i++){
        dummy = lround(fabs(reference[i]/mAccSensitivity));
        dummy&=0b0011111111111111;
        write((Address)(ACC_REF_X_MSB+2*i), (uint8_t*) &dummy, 2);
    }
    uint8_t data=lround(fabs(count/mODR));
    write(ACC_MAGNITUDE_COUNT, &data);
    config&=0b0111000;
    write(ACC_MAGNITUDE_CFG, &config);
    setInterrupt(pin, I_ACC_MAGNITUDE, function);
}

void FXOS8700CQ::setFreefallMotion(Interrupt_Pin pin, void (*function)(), float count, bool resetCount, uint8_t config, float threshold, float xThreshold, float yThreshold, float zThreshold){
    config&=0b11111000;
    write(MOTION_CFG, &config);
    uint8_t data=lround(fabs(count/mODR));
    write(MOTION_COUNT, &data);
    threshold=fabs(threshold);
    xThreshold=fabs(xThreshold);
    yThreshold=fabs(yThreshold);
    zThreshold=fabs(zThreshold);
    data = lround(xThreshold/63);
    data&=0b01111111;
    data|=resetCount<<7;
    write(MOTION_THRESHOLD, &data);
    uint16_t dummy = lround(xThreshold/63);
    dummy<<=2;
    if(xThreshold){
        dummy|=1<<15;                
    }
    else{dummy&=~(1<<15);}
    write(MOTION_X_MSB, (uint8_t*) &dummy, 2);
    dummy = lround(yThreshold/63);
    dummy<<=2;
    if(yThreshold){
        dummy|=1<<15;                
    }
    else{dummy&=~(1<<15);}
    write(MOTION_Y_MSB, (uint8_t*) &dummy, 2);
    dummy = lround(zThreshold/63);
    dummy<<=2;
    if(zThreshold){
        dummy|=1<<15;                
    }
    else{dummy&=~(1<<15);}
    write(MOTION_Z_MSB, (uint8_t*) &dummy, 2);
    setInterrupt(pin, I_FREEFALL, function);
}

void FXOS8700CQ::setPulse(Interrupt_Pin pin, void (*function)(), uint8_t config, float timing, float* threshold, float latency, float window){
    write(PULSE_CFG, &config);
    uint8_t data;
    for(uint8_t i=0;i<3;i++){
        data=lround(fabs(threshold[i]/63));
        write(PULSE_X_THRESHOLD, &data);
    }
    data=lround(fabs(latency/mODR));
    data&=127;
    write(PULSE_LATENCY, &data);
    data=lround(fabs(timing/mODR));
    write(PULSE_TIME_LIMIT, &data);
    data=lround(fabs(window/mODR));
    write(PULSE_WINDOW, &data);
    setInterrupt(pin, I_PULSE, function);
}

void FXOS8700CQ::setOrientation(Interrupt_Pin pin, void (*function)(), float count, bool resetCount, Lockout_Angle lockout, Orientation_Angle angle, Trip_Threshold threshold, Trip_Delta delta){
    uint8_t data = lround(fabs(count/mODR));
    write(ORIENTATION_COUNT, &data);
    data=lockout|angle;
    write(LOCKING_ANGLES_CFG, &data);
    data=threshold|delta;
    write(ORIENTATION_THRESHOLD, &data);
    data=(resetCount<<7)|64;
    setInterrupt(pin, I_ORIENTATION, function);
}

void FXOS8700CQ::setTransient(Interrupt_Pin pin, void (*function)(), float count, bool resetCount, uint8_t config, float threshold){
    uint8_t data;
    config&=31;
    write(TRANSIENT_CFG, &config);
    data=lround(fabs(threshold/63));
    data&=127;
    data|=(resetCount<<7);
    write(TRANSIENT_THRESHOLD, &data);
    data=lround(fabs(count/mODR));
    write(TRANSIENT_COUNT, &data);
    setInterrupt(pin, I_TRANSIENT, function);
}

void FXOS8700CQ::setSleepWake(Interrupt_Pin pin, void (*function)(), float count, bool resetCount, uint8_t config, uint8_t interrupts){
    uint8_t data;
    if(mAwakeODR==ODR1){
        if(count>63){count=63;}
        data=lround(count*1000/640);
    }
    else{
        if(count>81){count=81;}
        data=lround(count*1000/320);
    }
    write(SLEEP_COUNTER, &data);
    // CTRL_REG_3 holds which interrupts can wake the sensor AND 
    // the polarity and resistance of the interrupt pins AND a FIFO option, 
    // so we need to make sure not to change bits 0, 1 and 7.
    read(SLEEP_INT_CONFIG, &data);
    data&=131;
    interrupts<<=1;
    interrupts&=124;
    data|=interrupts;
    write(SLEEP_INT_CONFIG, &data);
    read(CTRL_REG_2, &data);
    data|=2;
    write(CTRL_REG_2, &data); 
    setInterrupt(pin, I_SLEEP_WAKE, function);
}

void FXOS8700CQ::setMagneticThreshold(Interrupt_Pin pin, void (*function)(), float count, bool resetCount, uint8_t config, float *threshold){
    uint16_t dummy;
    for(uint8_t i=0;i<3;i++){
        read((Address)(MAG_X_OFFSET_MSB+2*i), (uint8_t*) &dummy, 2);
        dummy=lround(fabs(threshold[i]*10))-dummy;
        dummy&=~(1<<15);
        if(i==0){dummy|=resetCount;}
        write((Address)(MAG_THRESHOLD_X_MSB+2*i), (uint8_t*) &dummy, 2);
    }
    uint8_t data = lround(fabs(count/mODR));
    write(MAG_THRESHOLD_COUNT, &data);
    config&=pin;
    write(MAG_THRESHOLD_CFG, &config);
    setInterrupt(pin, I_MAG_THRESHOLD, function);
}

void FXOS8700CQ::setMagneticMagnitude(Interrupt_Pin pin, void (*function)(), uint8_t count, bool resetCount, uint8_t config, float threshold, float* reference){
    uint16_t dummy = lround(fabs(threshold*10));
    dummy&=0b0111111111111111;
    if(resetCount){dummy|=1<<15;}
    write(MAG_MAGNITUDE_MSB, (uint8_t*) &dummy, 2);
    for(uint i=0;i<3;i++){
        dummy = lround(reference[i]/MAGNETIC_SENSITIVITY);
        write((Address)(MAG_REF_X_MSB+2*i), (uint8_t*) &dummy, 2);
    }
    uint8_t data=lround(fabs(count/mODR));
    write(MAG_MAGNITUDE_COUNT, &data);
    config|=pin;
    write(MAG_MAGNITUDE_CFG, &config);
    setInterrupt(pin, I_MAG_MAGNITUDE, function);
}



float* FXOS8700CQ::getAcceleration(){
    uint8_t *data = getRawAcceleration();
    float *result = new float[3];
    for(uint i=0;i<3;i++){ 
        result[i] = convertAcceleration(data + 2*i);
    }
    delete[] data;
    return result;
}

float* FXOS8700CQ::getMagnetic(){
    uint8_t *data = getRawMagnetic();
    float *result = new float[3];
    for(uint i=0;i<3;i++){ 
        result[i] = convertMagnetic(data + 2*i);
    }
    delete[] data;
    return result;
}

float* FXOS8700CQ::getAllData(){
    if(mMode==HYBRID){
        uint8_t *data = getAllRawData();
        float *result = new float[6];
        for(uint8_t i=0;i<3;i++){ 
            result[i] = convertAcceleration(data + 2*i);
        }
        for(uint8_t i=3;i<6;i++){
            result[i] = convertMagnetic(data + 2*i);
        }
        delete[] data;
        return result;
    }
    return NULL;
}

float FXOS8700CQ::getTemperature(){
    if(mMode!=ACCELEROMETER){
        uint8_t data;
        read(TEMPERATURE, &data);
        return TEMPERATURE_SENSITIVITY * (int)(~data + 1);
    }
    return -300.; // Not pretty, but a number under 0 K should be detected by well written code. 
}



void FXOS8700CQ::setInterrupt(Interrupt_Pin pin, Interrupt name, void (*function)()){
    uint8_t data;
    if(name<=I_SLEEP_WAKE){
        read(INT_CONFIG, &data);
        data|=name;
        write(INT_CONFIG, &data);
        read(INT_PIN_CONFIG, &data);
        data&=~name;
        data|=(name*pin);
        write(INT_PIN_CONFIG, &data);
    }
    activeInterrupts |= name;
    if(pin==PIN_ONE){
        mInterruptOne.fall(callback(this, &FXOS8700CQ::dispatchInterruptData));
    }
    else{
        mInterruptTwo.fall(callback(this, &FXOS8700CQ::dispatchInterruptData));
    }
    setInterruptFunction(function, name);  
}

void FXOS8700CQ::setInterrupt(Interrupt_Pin pin, Interrupt name, void (*function)(), FIFO_Mode mode, uint8_t watermark, uint8_t config){
    
    if(name!=I_FIFO){return;}
    
    uint8_t data; 
    read(INT_CONFIG, &data);
    data|=name;
    write(INT_CONFIG, &data);
    read(INT_PIN_CONFIG, &data);
    data&=~name;
    data|=(name*pin);
    write(INT_PIN_CONFIG, &data);
    data = watermark | (mode<<5);
    write(FIFO_SETUP, &data);
    activeInterrupts |= name;
    if(mode==TRIGGER){
        config&=62;
        write(FIFO_TRIGGER, &config);
    }
    if(pin==PIN_ONE){
        mInterruptOne.fall(callback(this, &FXOS8700CQ::dispatchInterruptData));
    }
    else{
        mInterruptTwo.fall(callback(this, &FXOS8700CQ::dispatchInterruptData));
    }
    setInterruptFunction(function, name);
}


void FXOS8700CQ::removeInterrupt(Interrupt name){
    uint8_t data;
    if(name<=I_SLEEP_WAKE){    
        read(INT_CONFIG, &data);
        data &= ~(name);
        write(INT_CONFIG, &data);
    }
    else if(name==I_MAG_THRESHOLD){
        read(MAG_THRESHOLD_CFG, &data);
        data&=253;
        write(MAG_THRESHOLD_CFG, &data);
    }
    else{
        read(MAG_MAGNITUDE_CFG, &data);
        data&=253;
        write(MAG_MAGNITUDE_CFG, &data);
    }
    if(activeInterrupts&name){activeInterrupts &= ~name;}
    if(!activeInterrupts){
        mInterruptOne.fall(NULL);
        mInterruptTwo.fall(NULL);
        mThread.terminate();
        mInterrupts[lround(log2(name))] = NULL;
    }
}

void FXOS8700CQ::setInterruptFunction(void (*function)(), Interrupt name){
    if(function==NULL){return;}
    mInterrupts[lround(log2(name))] = function;
}

void FXOS8700CQ::interruptWrapper(){
    //TODO
    while(1){
        Interrupt name = (Interrupt) (1<<mEvent.wait_any());
        switch(name){
            case I_NEW_DATA: {
                uint8_t data;
                read(STATUS, &data);
                float *samples = getAcceleration();
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
            case I_ACC_MAGNITUDE: {
                break;
            }
            case I_FREEFALL     : {
            break;
            }
            case I_PULSE        : {
                break;
            }
            case I_ORIENTATION  : {
                break;
            }
            case I_TRANSIENT    : {

            }
            case I_FIFO         : {
                uint8_t samplesNumber;
                read(STATUS, &samplesNumber);
                samplesNumber&=63;
                mail_t **mailArray = new mail_t*[3*samplesNumber];
                for(int i=0;i<2*samplesNumber;i++){
                    *(mailArray+i)= mailBox.alloc();
                }
                uint8_t *samples = new uint8_t[6*samplesNumber];
                read(X_MSB, samples, 6*samplesNumber);
                for(uint8_t i=0;i<3*samplesNumber;i++){
                    (*(mailArray+i))->axis = (Axis) (i%3);
                    (*(mailArray+i))->value = convertAcceleration(samples + 2*i);

                    mailBox.put(*(mailArray+i));
                }
                delete[] samples;
                break;
            }
            case I_SLEEP_WAKE   : {
                if(awake){mODR=mAwakeODR;}
                else{mODR=mSleepODR;}
                awake=!awake;
                break;
            }
            case I_MAG_THRESHOLD: {
                break;
            }
            case I_MAG_MAGNITUDE: {
                break;
            }
        }
        if(mInterrupts[lround(log2(name))]){
            (*mInterrupts[lround(log2(name))])();
        }
    }
}


Interrupt FXOS8700CQ::identifyInterrupt(){
    uint8_t data;
    read(INT_STATUS, &data);
    if(activeInterrupts & data){return (Interrupt) (activeInterrupts & data);}
    read(MAG_INT_STATUS, &data);
    return (Interrupt) (activeInterrupts & data<<8);
}

void FXOS8700CQ::dispatchInterruptData(){
    mEvent.set(lround(log2(identifyInterrupt())));
}



uint8_t* FXOS8700CQ::getRawAcceleration(){
    uint8_t *data = new uint8_t[6];
    read(ACC_X_MSB, data, 6);
    return data;
}

uint8_t* FXOS8700CQ::getRawMagnetic(){
    uint8_t *data = new uint8_t[6];
    read(MAG_X_MSB, data, 6);
    return data;
}

uint8_t* FXOS8700CQ::getAllRawData(){
    uint8_t *data = new uint8_t[12];
    read(ACC_X_MSB, data, 12);
    return data;
}


float FXOS8700CQ::convertAcceleration(uint8_t *rawAcc){
    float result = mAccSensitivity * (~(((rawAcc[0])<<8)  + ((rawAcc[1])>>2)) + 1);
    return result;
}

float FXOS8700CQ::convertMagnetic(uint8_t *rawMag){
    float result = MAGNETIC_SENSITIVITY * (~(((rawMag[0])<<8)  + ((rawMag[1])>>2)) + 1);;
    return result;
}


int FXOS8700CQ::write(Address address, uint8_t *data, int length){
    uint8_t *bigData = new uint8_t[length+1];
    *bigData = address;
    for(int i = 0; i < length; i++){
        *(bigData+i+1) = data[i];
    }
    int result = mI2C.write(mAddress, (char*) bigData, length + 1);
    delete[] bigData;
    return result; 
}

void FXOS8700CQ::read(Address address, uint8_t *data, int length){
    uint8_t addressP[] = {address}; 
    mI2C.write(mAddress, (char*) addressP, 1, true);
    mI2C.read(mAddress, (char*) data, length);
}