#include "MAX30101_enum.h"
#include "MAX30101.h"
#include "mbed.h"
#include "rtos.h"
#include <algorithm>
#include "arm_math.h"

using namespace MAX;

const float MAX30101::temperatureDeltaCoeff1 = 9373.0f/99600;
const float MAX30101::temperatureDeltaCoeff2 = 71.0f/39840;

MAX30101::MAX30101(Mode mode, Oversample oversample,
    bool fifoRollover, uint8_t fifoThreshold, Led slot1, Led slot2,Led slot3, Led slot4) : 
    mPower(PTA29), mI2C(PTB1, PTB0), mInterrupt(PTB18), mAddress(0xAE), mResolution(3), 
    mTemperature(-300), mSampleRate(OX_RATE_50), mPulseWidth(PULSE_WDT_69){
    mPower = 1;
    mI2C.frequency(400000);
    reset();
    updateChannels(mode, slot1, slot2, slot3, slot4);
    uint8_t data = mode;
    write(MODE_CONFIG, &data);
    data = fifoThreshold | (((uint8_t) fifoRollover)<<4) | (oversample<<5);
    write(FIFO_CONFIG, &data);
    clearFIFOCounters();
    mTicker.attach(callback(this, &MAX30101::requestTemperatureMeasure), 1);
    tThread.start(callback(this, &MAX30101::updateTemperature));
    uint8_t dummy[2];
    read(INTERRUPT_STATUS, dummy, 2);
    for(uint8_t i=0;i<4;i++){
        current[i]=0;
    }
    startTemperatureMeasurement();
}

MAX30101::~MAX30101(){
    if(mThread.get_state()!=Thread::Deleted){mThread.terminate();}
    if(tThread.get_state()!=Thread::Deleted){tThread.terminate();}
    powerDown();
}

void MAX30101::standby(){
    uint8_t data;
    read(MODE_CONFIG, &data);
    data|=1<<7;
    write(MODE_CONFIG, &data);
}

void MAX30101::powerUp(){
    mPower = 1;
    uint8_t data;
    read(MODE_CONFIG, &data);
    data&=127;
    write(MODE_CONFIG, &data);
}

void MAX30101::reset(){
    uint8_t data;
    read(MODE_CONFIG, &data);
    data|=1<<6;
    write(MODE_CONFIG, &data);
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
    float32_t dcComponent;
    arm_mean_f32(fValues, length, &dcComponent);
    arm_offset_f32(fValues, dcComponent, fValues, length);
    arm_rfft_fast_instance_f32 transformStructure;
    arm_rfft_fast_init_f32(&transformStructure, length);
    float32_t *frequencies = new float32_t[2*length];
    arm_rfft_fast_f32(&transformStructure, fValues, frequencies, 0);
    arm_cmplx_mag_squared_f32(frequencies + lround(((float) (MIN_HEART_FREQUENCY * length))/(60 * 50 * 1<<mSampleRate)),
                    fValues, lround((( (float) (MAX_HEART_FREQUENCY - MIN_HEART_FREQUENCY)) * length)/(60 * 50 * 1<<mSampleRate)));
    // Only selecting the frequencies that have physical meaning, as defined in the header.
    uint32_t index = 0;
    delete[] frequencies;
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
        read(FIFO_OVERFLOW_CTR, &fifoOverflow);
        if(fifoOverflow){
            numberOfSamples = 32;
        }
        else{
            read(FIFO_WRITE_PTR, &fifoWrite);
            read(FIFO_READ_PTR, &fifoRead);
            numberOfSamples = (32 + fifoWrite - fifoRead) % 32;
        }
    }
    if(numberOfSamples){
        uint8_t *data= new uint8_t[3 * mSampleTemplate.length * numberOfSamples];
        read(FIFO_DATA, data, 3 * mSampleTemplate.length * numberOfSamples);
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
        return samples;
    }
    else{
        return NULL;
    }
}

void MAX30101::setOversample(Oversample oversample){
    uint8_t data;
    read(FIFO_CONFIG, &data);
    data&=0x1F;
    data|=oversample;
    write(FIFO_CONFIG, &data);
}

void MAX30101::setFIFORollover(bool fifoRollover){
    uint8_t data;
    read(FIFO_CONFIG, &data);
    data&=0xEF;
    if(fifoRollover){data|=1<<4;}
    write(FIFO_CONFIG, &data);
}

void MAX30101::setFIFOThreshold(uint8_t fifoThreshold){
    fifoThreshold=min(fifoThreshold, (uint8_t) 15);
    uint8_t data;
    read(FIFO_CONFIG, &data);
    data&=0xF0;
    data|=fifoThreshold;
    write(FIFO_CONFIG, &data);
}

void MAX30101::setMode(Mode mode, Led slot1, Led slot2, Led slot3, Led slot4){
    uint8_t data;
    read(MODE_CONFIG, &data);
    data&=0xF8;
    data|=mode;
    write(MODE_CONFIG, &data);
    clearFIFOCounters();
    if(mode == MULTI_MODE && (slot1||slot2||slot3||slot4) ){
        setMultiLedTiming(slot1, slot2, slot3, slot4);
    }
    else{updateChannels(mode, slot1, slot2, slot3, slot4);}
}

void MAX30101::setPulseAmplitude(uint8_t redAmplitude, uint8_t irAmplitude, 
                                uint8_t greenAmplitude, uint8_t pilotAmplitude){
    uint8_t data[3];
    data[0] = redAmplitude;
    data[1] = irAmplitude;
    data[2] = greenAmplitude;
    write(LED_CONFIG, data, 3);
    write(P_LED_CONFIG, &pilotAmplitude);
    current[0] = redAmplitude/5;
    current[1] = greenAmplitude/5;
    current[2] = irAmplitude/5;
    current[3] = pilotAmplitude/5;
}

void MAX30101::setPulseWidth(Pulse_Width width){
    uint8_t data;
    read(OXYGEN_CONFIG, &data);
    data&=0xFC;
    data|=width;
    write(OXYGEN_CONFIG, &data);
    mPulseWidth = width; 
}

void MAX30101::setOxygenRate(Oxygen_Rate rate){
    uint8_t data;
    read(OXYGEN_CONFIG, &data);
    data&=0xE3;
    data|=rate<<3;
    write(OXYGEN_CONFIG, &data);
    mSampleRate = rate;
}

void MAX30101::setOxygenRange(Oxygen_Range range){
    uint8_t data;
    read(OXYGEN_CONFIG, &data);
    data&=0x9F;
    data|=range<<5;
    write(OXYGEN_CONFIG, &data);
}

void MAX30101::setProximityDelay(uint8_t delay){
    write(LED_TIMING, &delay);
}

void MAX30101::setMultiLedTiming(Led slot1, Led slot2, Led slot3, Led slot4){
    setMode(MULTI_MODE);
    uint8_t data[2];
    data[0] = slot1 + (slot2<<4);
    data[1] = slot3 + (slot4<<4);
    write(LED_TIMING, data, 2);
    updateChannels(MULTI_MODE, slot1, slot2, slot3, slot4);
}



void MAX30101::setInterrupt(Interrupt name, void (*function)(), uint8_t threshold, bool fifoRollover){
    if(name==I_NO_INTERRUPT){return;}
    uint8_t data[2];
    read(INTERRUPT_CONFIG, data, 2);
    if(name == I_TEMPERATURE){
        data[1]|=name;
    }
    else{
        data[0]|=name;
    }
    write(INTERRUPT_CONFIG, data, 2);
    switch(name){
        case I_FIFO_FULL: {
            setFIFOThreshold(threshold);
            setFIFORollover(fifoRollover);
            break;
        }
        case I_START: {
            setProximityDelay(threshold);
            break;
        }
        default:{
            break;
        }
    }
    mInterrupt.fall(callback(this, &MAX30101::dispatchInterruptData));
    setInterruptFunction(function);
}

void MAX30101::removeInterrupt(Interrupt name){
    uint8_t data[2];
    read(INTERRUPT_CONFIG, data, 2);
    if(name == I_TEMPERATURE){
        data[1]&=~name;
    }
    else{
        data[0]&=~name;
    }
    write(INTERRUPT_CONFIG, data, 2);
    if(data[0]==0 && data[1]==2){
        mInterrupt.fall(NULL);
        mInterruptFunction = NULL;
        mThread.terminate();
    }
}

uint8_t MAX30101::combineLeds(uint8AndFloat* leds, uint8_t length){
    float meanSum = 0;
    float weightSum = 0;
    for(int i=0; i<length; i++){
        meanSum+=leds[i].hr*leds[i].weight;
        weightSum+=leds[i].weight;
    }
    return lround(meanSum/weightSum);
}

void MAX30101::startTemperatureMeasurement(){
    uint8_t data = 1;
    write(TEMPERATURE_START, &data);
}

float MAX30101::getTemperature(){
    uint8_t data[2];
    read(TEMPERATURE, data, 2);
    float result = (int8_t) data[0] + data[1] * 0.0625; 
    //TODO This measures the average temperature, still need to add the correction
    //due to the red led duty cycle.
    //There is a component that is linear in (50<<(SPo2_rate))*pulse_width/10000      
    //No idea about (1<<red_intensity)/10
    //Possibly ∆T = current * duty_cycle_percentage * parameter + constant
    float pulseWidth;
    switch(mPulseWidth){
        case PULSE_WDT_69 : {pulseWidth = 68.95;}
        case PULSE_WDT_118: {pulseWidth = 117.78;}
        case PULSE_WDT_215: {pulseWidth = 215.44;}
        case PULSE_WDT_411: {pulseWidth = 410.75;}
    }
    float sampleRate = 50*(1<<mSampleRate);
    if(mSampleRate>OX_RATE_800){
        if(mSampleRate==OX_RATE_1000){sampleRate=1000;}
        else{sampleRate=50*(1<<(mSampleRate-1));}
    }
    mTemperature = result + sampleRate*pulseWidth/10000 * current[0]*(temperatureDeltaCoeff1 - current[0]*temperatureDeltaCoeff2);
    startTemperatureMeasurement();
    return mTemperature; 
}

float MAX30101::getTemperatureDelta(){
    //TODO This measures the average temperature, still need to add the correction
    //due to the red led duty cycle.
    //There is a component that is linear in (50<<(SPo2_rate))*pulse_width/10000      
    //No idea about (1<<red_intensity)/10
    //Possibly ∆T = current * duty_cycle_percentage * parameter + constant
    float pulseWidth;
    switch(mPulseWidth){
        case PULSE_WDT_69 : {pulseWidth = 68.95;}
        case PULSE_WDT_118: {pulseWidth = 117.78;}
        case PULSE_WDT_215: {pulseWidth = 215.44;}
        case PULSE_WDT_411: {pulseWidth = 410.75;}
    }
    float sampleRate = 50*(1<<mSampleRate);
    if(mSampleRate>OX_RATE_800){
        if(mSampleRate==OX_RATE_1000){sampleRate=1000;}
        else{sampleRate=50*(1<<(mSampleRate-1));}
    }
    float totalCurrent=0;
    for(uint8_t i=0;i<mSampleTemplate.length;i++){
        //Reads from mSampleTemplate what leds light up during each measure and sums up their heat contribution.
        totalCurrent+=current[(mSampleTemplate.ledSamples[i]).ledType-1];
    }
    return sampleRate*pulseWidth/10000 * totalCurrent*(temperatureDeltaCoeff1 - totalCurrent*temperatureDeltaCoeff2);
}


void MAX30101::requestTemperatureMeasure(){
    tThread.signal_set(0x01);
}

void MAX30101::updateTemperature(){
    while(1){
        Thread::signal_wait(0x01);
        getTemperature();
    }
}


void MAX30101::setInterruptFunction(void (*function)()){
    mInterruptFunction = function;
    if(mThread.get_state()==Thread::Deleted){
        mThread.start(callback(this, &MAX30101::interruptWrapper));
        }
}

void MAX30101::interruptWrapper(){
    //TODO
    while(1){
        Thread::signal_wait(0x01);
        Interrupt name = identifyInterrupt();
        while(name){
            switch(name){
                case I_FIFO_FULL:{//Falls into the next case
                }
                case I_NEW_DATA:{
                    uint8_t fifoOverflow, sampleNumber;
                    read(FIFO_OVERFLOW_CTR, &fifoOverflow);
                    if(fifoOverflow){
                        sampleNumber = 32;
                    }
                    else{
                        uint8_t fifoWrite, fifoRead;
                        read(FIFO_WRITE_PTR, &fifoWrite);
                        read(FIFO_READ_PTR, &fifoRead);
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
                case I_LIGHT:{
                    //TODO
                    break;
                }
                case I_START:{
                    //TODO 
                    break;
                }
                case I_TEMPERATURE:{
                    getTemperature();
                    break;
                }
                case I_NO_INTERRUPT:{
                    //Will never get here as I_NO_INTERRUPT==0
                    break;
                }
            }
            mInterruptFunction();
            name = identifyInterrupt();
        }
    }
}

void MAX30101::dispatchInterruptData(){
    mThread.signal_set(0x01);
}


Interrupt MAX30101::identifyInterrupt(){
    uint8_t data[2];
    read(INTERRUPT_STATUS, data, 2);
    if(data[1]&I_TEMPERATURE){
        return I_TEMPERATURE;
    }
    else{
        for(uint8_t i=4;i<8;i++){
            if(data[0]&(1<<i)){
                return (Interrupt) (1<<i);
            }
        }
    }
    return I_NO_INTERRUPT;
}



void MAX30101::updateChannels(Mode mode, Led slot1, 
    Led slot2, Led slot3, Led slot4){
    //TODO The docs are vague on whether the HR_MODE can use 
    //other leds than the red one.
    if(mSampleTemplate.ledSamples!=NULL){
        delete[] mSampleTemplate.ledSamples;
    }
    switch(mode){
        case HR_MODE:{
            mSampleTemplate.length = 1;
            mSampleTemplate.ledSamples = new ledSample_t[1];
            mSampleTemplate.ledSamples[0].ledType = RED_LED;
            break;
        }
        case OX_MODE:{
            mSampleTemplate.length = 2;
            mSampleTemplate.ledSamples = new ledSample_t[2];
            mSampleTemplate.ledSamples[0].ledType = RED_LED;
            mSampleTemplate.ledSamples[1].ledType = IR_LED;
            break;
        }
        case MULTI_MODE:{
            Led slot[4] = {slot1, slot2, slot3, slot4};
            mSampleTemplate.length = 0;
            uint8_t cycleEnd = 4;
            for(uint8_t i=0;i<cycleEnd;){
                if(slot[i]!=NONE_LED){
                    mSampleTemplate.length++;
                    i++;
                }
                else{
                    for(uint8_t j=i;j<cycleEnd-1;j++){
                        slot[j]=slot[j+1];
                        cycleEnd--;
                    }
                }
            }
            mSampleTemplate.ledSamples = new ledSample_t[mSampleTemplate.length];
            for(uint8_t i=0;i<mSampleTemplate.length;i++){
                mSampleTemplate.ledSamples[i].ledType = slot[i];
            }
        }
    }
}

void MAX30101::clearFIFOCounters(){
    uint8_t data = 0;
    write(FIFO_READ_PTR, &data);
    write(FIFO_OVERFLOW_CTR, &data);
    write(FIFO_WRITE_PTR, &data);
}

void MAX30101::read(Address address, uint8_t* data, int length){
    mI2C.write(mAddress, (char*) &address, 1);
    mI2C.read(mAddress, (char*) data, length);
}

int MAX30101::write(Address address, uint8_t* data, int length){
    uint8_t *bigData = new uint8_t[length+1];
    *bigData = address;
    for(int i = 0; i < length; i++){
        *(bigData+i+1) = data[i];
    }
    int result = mI2C.write(mAddress, (char*) bigData, length + 1);
    delete[] bigData;
    return result;
}