#include "MAX30101_enum.h"
#include "MAX30101.h"
#include "mbed.h"
#include "rtos.h"
#include <algorithm>
#include "arm_math.h"

MAX30101::MAX30101(MAX30101_Mode mode, MAX30101_Oversample oversample,
    bool fifoRollover, uint8_t fifoThreshold, MAX30101_Led slot1, MAX30101_Led slot2,
    MAX30101_Led slot3, MAX30101_Led slot4) : mPower(PTA29), mI2C(PTB1, PTB0),
    mInterrupt(PTB18), mAddress(0xAE), mResolution(3), mTemperature(25){
    mPower = 1;
    mI2C.frequency(400000);
    reset();
    updateChannels(mode, slot1, slot2, slot3, slot4);
    uint8_t data = mode;
    write(MAX_MODE_CONFIG, &data);
    data = fifoThreshold + (((uint) fifoRollover)<<4) + (oversample<<5);
    write(MAX_FIFO_CONFIG, &data);
    clearFIFOCounters();
    if(mode==MAX_OX_MODE){
        mTicker.attach(callback(this, &MAX30101::startTemperatureMeasurement), 1);
        startTemperatureMeasurement();
    }
    uint8_t *dummy = new uint8_t[2];
    read(MAX_INTERRUPT_STATUS, dummy, 2);
    delete[] dummy;
    dummy=0;
}

MAX30101::~MAX30101(){
    powerDown();
}

void MAX30101::standby(){
    uint8_t data;
    read(MAX_MODE_CONFIG, &data);
    data|=1<<7;
    write(MAX_MODE_CONFIG, &data);
}

void MAX30101::powerUp(){
    mPower = 1;
    uint8_t data;
    read(MAX_MODE_CONFIG, &data);
    data&=127;
    write(MAX_MODE_CONFIG, &data);
}

void MAX30101::reset(){
    uint8_t data;
    read(MAX_MODE_CONFIG, &data);
    data|=1<<6;
    write(MAX_MODE_CONFIG, &data);
}

void MAX30101::powerDown(){
    standby();
    mPower = 0;
}


MAX30101::uint8AndFloat MAX30101::getHR(uint32_t *values, uint16_t length){
    float32_t *fValues = new float32_t[length];
    for(uint i=0;i<length;i++){
        fValues[i] = values[i];
    }
    arm_rfft_fast_instance_f32 transformStructure;
    arm_rfft_fast_init_f32(&transformStructure, length);
    float32_t *frequencies = new float32_t[2*length];
    arm_rfft_fast_f32(&transformStructure, fValues, frequencies, 0);
    arm_cmplx_mag_squared_f32(frequencies + lround(((float) (MIN_HEART_FREQUENCY * length))/(60 * 50 * 1<<mSampleRate)),
                    fValues, lround((( (float) (MAX_HEART_FREQUENCY - MIN_HEART_FREQUENCY)) * length)/(60 * 50 * 1<<mSampleRate)));
    // Only selecting the frequencies that have physical meaning, as defined in the header.
    uint32_t index = 0;
    delete[] frequencies;
    frequencies=0;
    float32_t variance;
    arm_max_f32(fValues, length, &variance, &index);
    arm_var_f32(fValues, lround((( (float) (MAX_HEART_FREQUENCY - MIN_HEART_FREQUENCY)) * length)/(60 * 50 * 1<<mSampleRate)), &variance);
    uint8AndFloat result;
    result.hr = ((uint8_t) (60 * (float)(index) * (50*1<<mSampleRate) / length)) + MIN_HEART_FREQUENCY;
    // 60 is the number of seconds in a minute, length is needed because the FFTs are not normalized
    result.weight = length * abs(fValues[index] - index>MIN_HEART_FREQUENCY ? fValues[index-1] : fValues[index+1])/variance; 
    // As for the hr, the variance needs to be scaled by a factor of length, and divided by the derivative of fValues due to uncertainty propagation.
    // We should also multiply it by the same factor as hr, but we will only use it as a weight, so that would be a common factor to all terms.
    delete[] fValues;
    fValues=0;
    return result;
}


MAX30101::mail_t MAX30101::getSampleTemplate(){
    return mSampleTemplate;
}

MAX30101::mail_t *MAX30101::getData(uint8_t numberOfSamples){
    if(numberOfSamples){
        numberOfSamples=min(numberOfSamples, (uint8_t) 32);
    // Sanitizing the input to avoid reading outside of the FIFO buffer
    //TODO the 32 should be changed to a variable that depends on the mode of operation
    }
    else{
        uint8_t fifoWrite, fifoRead, fifoOverflow;
        read(MAX_FIFO_OVERFLOW_CTR, &fifoOverflow);
        if(fifoOverflow){
            numberOfSamples = 32;
        }
        else{
            read(MAX_FIFO_WRITE_PTR, &fifoWrite);
            read(MAX_FIFO_READ_PTR, &fifoRead);
            numberOfSamples = (32 + fifoWrite - fifoRead) % 32;
        }
    }
    if(numberOfSamples){
        uint8_t *data= new uint8_t[3 * mSampleTemplate.length * numberOfSamples];
        read(MAX_FIFO_DATA, data, 3 * mSampleTemplate.length * numberOfSamples);
        mail_t* samples = new mail_t[numberOfSamples];
        for(int i=0;i<numberOfSamples;i++){
            samples[i].ledSamples = new ledSample_t[mSampleTemplate.length];
            samples[i].length = mSampleTemplate.length;
            samples[i].temperature = mTemperature;
            for(int j=0; j<mSampleTemplate.length;j++){
                samples[i].ledSamples[j].ledType = mSampleTemplate.ledSamples[j].ledType;
                samples[i].ledSamples[j].value = (data[3 * (j + mSampleTemplate.length * i) +2] + (data[3 * (j + mSampleTemplate.length * i) + 1]<<8) + ((data[3 * (j + mSampleTemplate.length * i)]&&3)<<16))>>mResolution;
            }
        }
        delete[] data;
        data=0;
        return samples;
    }
    else{
        return NULL; // TODO Might need to define some exceptions if there's no data
    }
}

void MAX30101::setOversample(MAX30101_Oversample oversample){
    uint8_t data;
    read(MAX_FIFO_CONFIG, &data);
    data&=0x1F;
    data|=oversample;
    write(MAX_FIFO_CONFIG, &data);
}

void MAX30101::setFIFORollover(bool fifoRollover){
    uint8_t data;
    read(MAX_FIFO_CONFIG, &data);
    data&=0xEF;
    if(fifoRollover){data|=1<<4;}
    write(MAX_FIFO_CONFIG, &data);
}

void MAX30101::setFIFOThreshold(uint8_t fifoThreshold){
    fifoThreshold=min(fifoThreshold, (uint8_t) 15);
    uint8_t data;
    read(MAX_FIFO_CONFIG, &data);
    data&=0xF0;
    data|=fifoThreshold;
    write(MAX_FIFO_CONFIG, &data);
}

void MAX30101::setMode(MAX30101_Mode mode, MAX30101_Led slot1, 
    MAX30101_Led slot2, MAX30101_Led slot3, MAX30101_Led slot4){
    uint8_t data;
    read(MAX_MODE_CONFIG, &data);
    data&=0xF8;
    data|=mode;
    write(MAX_MODE_CONFIG, &data);
    clearFIFOCounters();
    if(mode == MAX_MULTI_MODE && (slot1||slot2||slot3||slot4) ){
        setMultiLedTiming(slot1, slot2, slot3, slot4);
    }
    else{updateChannels(mode, slot1, slot2, slot3, slot4);}
}

void MAX30101::setPulseAmplitude(uint8_t redAmplitude, uint8_t irAmplitude, 
                                uint8_t greenAmplitude, uint8_t pilotAmplitude){
    uint8_t *data = new uint8_t[3];
    data[0] = redAmplitude;
    data[1] = irAmplitude;
    data[2] = greenAmplitude;
    write(MAX_LED_CONFIG, data, 3);
    write(MAX_P_LED_CONFIG, &pilotAmplitude);
    delete[] data;
    data = 0;
}

void MAX30101::setPulseWidth(MAX30101_Pulse_Width width){
    uint8_t data;
    read(MAX_OXYGEN_CONFIG, &data);
    data&=0xFC;
    data|=width;
    write(MAX_OXYGEN_CONFIG, &data); 
}

void MAX30101::setOxygenRate(MAX30101_Oxygen_Rate rate){
    uint8_t data;
    read(MAX_OXYGEN_CONFIG, &data);
    data&=0xE3;
    data|=rate<<3;
    write(MAX_OXYGEN_CONFIG, &data);
    mSampleRate = rate;
}

void MAX30101::setOxygenRange(MAX30101_Oxygen_Range range){
    uint8_t data;
    read(MAX_OXYGEN_CONFIG, &data);
    data&=0x9F;
    data|=range<<5;
    write(MAX_OXYGEN_CONFIG, &data);
}

void MAX30101::setProximityDelay(uint8_t delay){
    write(MAX_LED_TIMING, &delay);
}

void MAX30101::setMultiLedTiming(MAX30101_Led slot1, MAX30101_Led slot2, MAX30101_Led slot3, MAX30101_Led slot4){
    setMode(MAX_MULTI_MODE);
    uint8_t *data = new uint8_t[2];
    data[0] = slot1 + (slot2<<4);
    data[1] = slot3 + (slot4<<4);
    write(MAX_LED_TIMING, data, 2);
    delete[] data;
    data = 0;
    updateChannels(MAX_MULTI_MODE, slot1, slot2, slot3, slot4);
}



void MAX30101::setInterrupt(MAX30101_Interrupt interrupt, void (*function)(), uint8_t threshold, bool fifoRollover){
    if(interrupt!=1){
        uint8_t *data = new uint8_t[2];
        read(MAX_INTERRUPT_CONFIG, data, 2);
        if(interrupt == 2){
            data[1]|=interrupt;
        }
        else{
            data[0]|=interrupt;
        }
        write(MAX_INTERRUPT_CONFIG, data, 2);
        switch(interrupt){
            case I_FIFO_FULL_MAX: {
                setFIFOThreshold(threshold);
                setFIFORollover(fifoRollover);
                break;
            }
            case I_START_MAX: {
                setProximityDelay(threshold);
                break;
            }
            default:{
                break;
            }
        }
        delete[] data;
        data=0;
    }
    mInterrupt.fall(callback(this, &MAX30101::dispatchInterruptData));
    setInterruptFunction(function);
}

void MAX30101::removeInterrupt(MAX30101_Interrupt interrupt){
    if(interrupt==1){return;}
    uint8_t *data = new uint8_t[2];
    read(MAX_INTERRUPT_CONFIG, data, 2);
    if(interrupt == 2){
        data[1]&=~interrupt;
    }
    else{
        data[0]&=~interrupt;
    }
    write(MAX_INTERRUPT_CONFIG, data, 2);
    if(data[0]==0 && data[1]==2){
        mInterrupt.fall(NULL);
        mInterruptFunction = NULL;
        mThread.terminate();
    }
    delete[] data;
    data=0;
}

uint8_t MAX30101::combineLeds(uint8AndFloat* leds, uint8_t length){
    float meanSum = 0;
    float weightSum = 0;
    for(int i=0; i<length; i++){
        meanSum+=leds[i].hr*leds[i].weight;
        weightSum+=leds[i].weight;
    }
    return roundl(meanSum/weightSum);
}


float MAX30101::getTemperature(){
    uint8_t *data = new uint8_t[2];
    read(MAX_TEMPERATURE, data, 2);
    float result = (int8_t) data[0] + data[1] * 0.0625; 
    //TODO This measures the average temperature, still need to add the correction
    //due to the red led duty cycle.
    //There is a component that is linear in (50<<(SPo2_rate))*pulse_width/10000      
    //No idea about (1<<red_intensity)/10
    delete[] data;
    data = 0;
    return result; 
}

void MAX30101::startTemperatureMeasurement(){
    uint8_t data = 1;
    write(MAX_TEMPERATURE_START, &data);
}


void MAX30101::setInterruptFunction(void (*function)()){
    mInterruptFunction = function;
    mThread.start(callback(this, &MAX30101::interruptWrapper));
}

void MAX30101::interruptWrapper(){
    //TODO
    while(1){
        Thread::signal_wait(0x01);
        uint8_t *data = new uint8_t[2];
        read(MAX_INTERRUPT_STATUS, data, 2);
        switch(data[0]){
            case I_FIFO_FULL_MAX:{//Falls into the next case
            }
            case I_NEW_DATA_MAX:{
                uint8_t fifoOverflow, sampleNumber;
                read(MAX_FIFO_OVERFLOW_CTR, &fifoOverflow);
                if(fifoOverflow){
                    sampleNumber = 32;
                }
                else{
                    uint8_t fifoWrite, fifoRead;
                    read(MAX_FIFO_WRITE_PTR, &fifoWrite);
                    read(MAX_FIFO_READ_PTR, &fifoRead);
                    sampleNumber = (32 + fifoWrite - fifoRead) % 32;
                }
                if(sampleNumber){
                    mail_t *mailArray = getData(sampleNumber);
                    for(uint i=0;i<sampleNumber;i++){
                        mail_t *mail = mailBox.alloc();
                        mail->ledSamples = new ledSample_t[mailArray[i].length];
                        mail->length = mailArray[i].length;
                        mail->temperature = mailArray[i].temperature;
                        for(uint j=0;j<mailArray[i].length;j++){
                            mail->ledSamples[j]=mailArray[i].ledSamples[j];
                        } 
                        mailBox.put(mail);
                        delete[] mailArray[i].ledSamples;
                    }
                    delete[] mailArray;
                }
            break;
            }
            case I_LIGHT_MAX:{
                //TODO
                break;
            }
            case I_POWER_UP_MAX:{
                //TODO
                break;
            }
            case I_START_MAX:{
                //TODO 
                break;
            }
            case 0:{
                mTemperature = getTemperature();
            }
        }
        delete[] data;
        data = 0;
        mInterruptFunction();
    }
}

void MAX30101::dispatchInterruptData(){
    mThread.signal_set(0x01);
}


void MAX30101::updateChannels(MAX30101_Mode mode, MAX30101_Led slot1, 
    MAX30101_Led slot2, MAX30101_Led slot3, MAX30101_Led slot4){
    //TODO The docs are vague on whether the MAX_HR_MODE can use 
    //other leds than the red one.
    if(mSampleTemplate.ledSamples!=NULL){
        delete[] mSampleTemplate.ledSamples;
        mSampleTemplate.ledSamples = 0;
    }
    switch(mode){
        case MAX_HR_MODE:{
            mSampleTemplate.length = 1;
            mSampleTemplate.ledSamples = new ledSample_t[1];
            mSampleTemplate.ledSamples[0].ledType = MAX_LED_RED;
            break;
        }
        case MAX_OX_MODE:{
            mSampleTemplate.length = 2;
            mSampleTemplate.ledSamples = new ledSample_t[2];
            mSampleTemplate.ledSamples[0].ledType = MAX_LED_RED;
            mSampleTemplate.ledSamples[1].ledType = MAX_LED_IR;
            break;
        }
        case MAX_MULTI_MODE:{
            //TODO The worst of spaghetti code, need to find some way to make a slot array and give it default values
            mSampleTemplate.length = 0;
            if(slot1){
                mSampleTemplate.length++;
                if(slot2){
                    mSampleTemplate.length++;
                    if(slot3){
                        mSampleTemplate.length++;
                        if(slot4){
                            mSampleTemplate.length++;
                        }
                    }
                    else{
                        if(slot4){
                            slot3=slot4;
                            slot4=MAX_LED_NONE;
                            mSampleTemplate.length++;
                        }
                    }
                }
                else {
                    if(slot3){
                        slot2=slot3;
                        slot3=MAX_LED_NONE;
                        mSampleTemplate.length++;
                        if(slot4){
                            slot3=slot4;
                            slot4=MAX_LED_NONE;
                            mSampleTemplate.length++;
                            break;
                        }
                    }
                    else {
                        if(slot4){
                            slot2=slot4;
                            slot4=MAX_LED_NONE;
                            mSampleTemplate.length++;
                        }
                    }
                }
            }
            else{
                if(slot2){
                    slot1=slot2;
                    slot2=MAX_LED_NONE;
                    mSampleTemplate.length++;
                    if(slot3){
                        slot2=slot3;
                        slot3=MAX_LED_NONE;
                        if(slot4){
                            slot3=slot4;
                            slot4=MAX_LED_NONE;
                            mSampleTemplate.length++;
                        }
                    }
                    else{
                        if(slot4){
                            slot2=slot4;
                            slot4=MAX_LED_NONE;
                            mSampleTemplate.length++;
                        }
                    }
                }
                else {
                    if(slot3){
                        slot1=slot3;
                        slot3=MAX_LED_NONE;
                        mSampleTemplate.length++;
                        if(slot4){
                            slot2=slot4;
                            slot4=MAX_LED_NONE;
                            mSampleTemplate.length++;
                            break;
                        }
                    }
                    else {
                        if(slot4){
                            slot1=slot4;
                            slot4=MAX_LED_NONE;
                            mSampleTemplate.length++;
                        }
                    }
                }
            }
            mSampleTemplate.ledSamples = new ledSample_t[mSampleTemplate.length];
            switch(mSampleTemplate.length){
                //They all fall through, so from the active case to 1 gets executed 
                case 4:{
                    mSampleTemplate.ledSamples[3].ledType = slot4;
                }   
                case 3:{
                    mSampleTemplate.ledSamples[2].ledType = slot3;
                }
                case 2:{
                    mSampleTemplate.ledSamples[1].ledType = slot2;
                }
                case 1:{
                    mSampleTemplate.ledSamples[0].ledType = slot1;
                }
                default:{break;}
            }
        }
    }
}

void MAX30101::clearFIFOCounters(){
    uint8_t data = 0;
    write(MAX_FIFO_READ_PTR, &data);
    write(MAX_FIFO_OVERFLOW_CTR, &data);
    write(MAX_FIFO_WRITE_PTR, &data);
}

void MAX30101::read(MAX30101_Address address, uint8_t* data, int length){
    mI2C.write(mAddress, (char*) &address, 1);
    mI2C.read(mAddress, (char*) data, length);
}

int MAX30101::write(MAX30101_Address address, uint8_t* data, int length){
    uint8_t *bigData = new uint8_t[length+1];
    *bigData = address;
    for(int i = 0; i < length; i++){
        *(bigData+i+1) = data[i];
    }
    int result = mI2C.write(mAddress, (char*) bigData, length + 1);
    delete[] bigData;
    bigData=0;
    return result;
}