#include "MPL3115A2_enum.h"
#include "MPL3115A2.h"
#include "mbed.h"

using namespace MPL;

bool MPL3115A2::visAltimeter = false;
bool MPL3115A2::visFIFO = false;
uint8_t MPL3115A2::activeInterrupts = 0;

MPL3115A2::MPL3115A2(Mode mode, Os_Ratio ratio, Time_Step timeStep) : 
mI2C(I2C_SDA, I2C_SCL), mAddress(0xC0), mInterruptOne(PTD12), mInterruptTwo(PTD10){
    fullReset();
    standby();
    uint8_t data = (1<<1) | (ratio<<3) | (mode<<7);
    write(CTRL_REG_1, &data);
    read(CTRL_REG_2, &data);
    data&=0b00110000;
    data|=timeStep;
    write(CTRL_REG_2, &data);
    if(mode==ALTIMETER){visAltimeter = true;}
    else{visAltimeter = false;}
    visFIFO = false;
    data = 0b00000111;
    write(EVENT_CONFIG, &data);
    setActive();
}

void MPL3115A2::setActive(){
    uint8_t data;
    read(CTRL_REG_1, &data);
    if(data&1){return;}
    else{
        data|=1;
        write(CTRL_REG_1, &data);
    }
}

void MPL3115A2::standby(){
    uint8_t data;
    read(CTRL_REG_1, &data);
    if(data&1){
        data&=(~1);
        write(CTRL_REG_1, &data);}
    else{return;}
}

void MPL3115A2::refresh(){
    standby();
    setActive();
}

void MPL3115A2::fullReset(){
    uint8_t data = 1<<2;
    write(CTRL_REG_1, &data);
    wait_ms(50);
    data = 0b00000111;
    write(EVENT_CONFIG, &data);
}

bool MPL3115A2::isDataAvailable(){
    uint8_t data;
    read(DR_STATUS, &data);
    return data&0b00001110;
}

uint8_t MPL3115A2::getStatus(){
    uint8_t data;
    read(DR_STATUS, &data);
    return data;
}

Os_Ratio MPL3115A2::getOsR(){
    uint8_t data;
    read(CTRL_REG_1, &data, 1);
    return (Os_Ratio)((data&0b00111000)>>3);
}

void MPL3115A2::setOsR(Os_Ratio ratio){
    standby();
    uint8_t data;
    read(CTRL_REG_1, &data, 1);
    data&=0b11000111;
    data|=(ratio<<3);
    write(CTRL_REG_1, &data);
    setActive();
}

Time_Step MPL3115A2::getTimeStep(){
    uint8_t data;
    read(CTRL_REG_2, &data);
    data&=0b00001111;
    return (Time_Step)(data);
}

void MPL3115A2::setTimeStep(Time_Step timeStep){
    uint8_t data;
    read(CTRL_REG_2, &data);
    data|=timeStep;
    write(CTRL_REG_2, &data);
}

bool MPL3115A2::isAltimeter(){
    return visAltimeter;
}
bool MPL3115A2::isFIFO(){
    return visFIFO;
}

float MPL3115A2::getData(){
    uint8_t dataLength;
    if(isFIFO()){dataLength = 5;}
    else{dataLength = 3;}
    uint8_t *data = new uint8_t[dataLength];
    read(PRESSURE_MSB, data, dataLength);
    float result;
    isAltimeter()?(result = convertAltitudeI2D(data)):(result = convertPressureI2D(data));
    delete[] data;
    return result;
}

float MPL3115A2::getTemperature(){
    uint8_t offset;
    Address address;
    if(isFIFO()){
        offset=3;
        address = PRESSURE_MSB;
    }
    else{
        offset = 0;
        address = TEMPERATURE_MSB; 
    }
    uint8_t *data = new uint8_t[2+offset];
    read(address, data, 2+offset);
    float result = convertTemperatureI2D(data+offset);
    delete[] data;
    return result;
}

float MPL3115A2::getDataDelta(){
    uint8_t data[3];
    read(PRESSURE_DELTA_MSB, data, 3);
    if (isAltimeter()){return convertAltitudeI2D(data);}
    else{return convertPressureI2D(data);}
}

float MPL3115A2::getTemperatureDelta(){
    uint8_t data[2];
    read(TEMPERATURE_DELTA_MSB, data, 2);
    return convertTemperatureI2D(data);
}

float MPL3115A2::getMaxData(){
    uint8_t data[3];
    read(MAX_PRESSURE_MSB, data, 3);
    if (isAltimeter()){return convertAltitudeI2D(data);}
    else{return convertPressureI2D(data);}
}
float MPL3115A2::getMaxTemperature(){
    uint8_t data[2];
    read(MAX_TEMPERATURE_MSB, data, 2);
    return convertTemperatureI2D(data);
}
float MPL3115A2::getMinData(){
    uint8_t data[3];
    read(MIN_PRESSURE_MSB, data, 3);
    if (isAltimeter()){return convertAltitudeI2D(data);}
    else{return convertPressureI2D(data);}
}
float MPL3115A2::getMinTemperature(){
    uint8_t data[2];
    read(MAX_TEMPERATURE_MSB, data, 2);
    return convertTemperatureI2D(data);
}

void MPL3115A2::setSeaLevelPressure(float pressure){
    uint8_t data[2];
    if(pressure){
        *data = (uint16_t) lround(pressure/2);
        write(PRESSURE_REFERENCE_MSB, data, 2);
    }
    else{
        if(isAltimeter()){
            uint8_t dummy;
            read(CTRL_REG_1, &dummy);
            dummy|=0b00000010;
            write(CTRL_REG_1, &dummy);
            setMode(BAROMETER);
            *data = (uint16_t) lround(getData()/2);
            write(PRESSURE_REFERENCE_MSB, data, 2);
        }
        else{
            *data = (uint16_t) lround(getData()/2);
            write(PRESSURE_REFERENCE_MSB, data, 2);
        }
    }
}

void MPL3115A2::setPressureOffset(int8_t pressure){
    pressure = ~(pressure*4) + 1;
//TODO Check if the underlying bits are actually left untouched, as they should.
    uint8_t upressure = (uint8_t)pressure;
    write(PRESSURE_OFFSET, &upressure); 
}

void MPL3115A2::setTemperatureOffset(float temperature){
    temperature = fmod(temperature, 8);
    temperature /=16;
    int8_t itemperature = (int8_t) lround(temperature);
//TODO Check if the underlying bits are actually left untouched, as they should.
    uint8_t utemperature = (uint8_t)itemperature;
    write(PRESSURE_OFFSET, &utemperature); 
}

void MPL3115A2::setAltitudeOffset(int8_t altitude){
//TODO Check if the underlying bits are actually left untouched, as they should.
    uint8_t ualtitude = (uint8_t)altitude;
    write(PRESSURE_OFFSET, &ualtitude); 
}

void MPL3115A2::setInterrupt(Interrupt_Pin pin, Interrupt name, void (*function)(), float target){
    if(name==I_NO_INTERRUPT){return;}
    standby();
    uint8_t data;
    read(CTRL_REG_5, &data);
    if(pin==PIN_ONE) {data |= name;}
    else    {data &= ~name;}
    write(CTRL_REG_5, &data);
    switch(name){
        case I_FIFO : {
            visFIFO = true;
            break;
        }
        case I_NEW_DATA : {
            //TODO Right now it lacks functionality, the hardware can interrupt on 
            //TODO just new pressure/altitude or new temperature
            break;
        }

        case I_PRESSURE_CHANGE : {
            if(activeInterrupts&I_PRESSURE_WINDOW){removeInterrupt(I_PRESSURE_WINDOW);}
            uint8_t data[2];
            if(isAltimeter()){
                if(target){
                    convertAltitudeD2I(target, data);
                    write(ALTITUDE_WINDOW_MSB, data, 2);
                }
                else{
                    uint8_t dummy;
                    read(CTRL_REG_2, &dummy);
                    dummy |= 0b00110000;
                    write(CTRL_REG_2, &dummy);
                }
                break;
            }
            else{
                if(target){
                    convertPressureD2I(target, data);
                    write(PRESSURE_WINDOW_MSB, data, 2);
                }
                else{
                    uint8_t dummy;
                    read(CTRL_REG_2, &dummy);
                    dummy |= 0b00110000;
                    write(CTRL_REG_2, &dummy);
                }
                break;
            }
        }

        case I_TEMPERATURE_CHANGE : {
            if(activeInterrupts&I_TEMPERATURE_WINDOW){removeInterrupt(I_TEMPERATURE_WINDOW);}
            uint8_t data;
            if(target){
                data = convertTemperatureD2I(target);
                write(TEMPERATURE_WINDOW, &data);
            }
            else{
                read(CTRL_REG_2, &data);
                data |= 0b00110000;
                write(CTRL_REG_2, &data);
            }
            break;
        }
        case I_ALTITUDE_THRESHOLD : {
            if(isAltimeter()){
                uint8_t data[2];
                if(target){
                    convertAltitudeD2I(target, data); 
                    write(ALTITUDE_TARGET_MSB, data, 2);
                    }
                else{
                    uint8_t dummy;
                    read(CTRL_REG_2, &dummy);
                    dummy |= 0b00110000;
                    write(CTRL_REG_2, &dummy);
                    }
                break;}
            else{
                if(target){
                    uint8_t data[2];
                    convertPressureD2I(target, data);
                    write(PRESSURE_TARGET_MSB, data, 2);}
                else{
                    read(CTRL_REG_2, &data);
                    data |= 0b00110000;
                    write(CTRL_REG_2, &data);
                    }
                break;
            }
        }
        case I_TEMPERATURE_THRESHOLD : {
            if(target){
                uint8_t data = convertTemperatureD2I(target);
                write(TEMPERATURE_TARGET, &data);}
            else{
                read(CTRL_REG_2, &data);
                data |= 0b00110000;
                write(CTRL_REG_2, &data);
                }
            break;
        }
        case I_ALTITUDE_WINDOW : {
            if(activeInterrupts&I_ALTITUDE_CHANGE){removeInterrupt(I_ALTITUDE_CHANGE);}
            if(isAltimeter()){
                if(target){
                    uint8_t data[2];
                    convertAltitudeD2I(target, data);
                    write(ALTITUDE_WINDOW_MSB, data, 2);
                    }
                else{
                    read(CTRL_REG_2, &data);
                    data |= 0b00110000;
                    write(CTRL_REG_2, &data);
                    }
            }
            else{
                if(target){
                    uint8_t data[2]; 
                    convertPressureD2I(target, data);
                    write(PRESSURE_WINDOW_MSB, data, 2);
                    }
                else{
                    read(CTRL_REG_2, &data);
                    data |= 0b00110000;
                    write(CTRL_REG_2, &data);
                    }
                }   
            break;
        }
        case I_TEMPERATURE_WINDOW : {
            if(activeInterrupts&1){removeInterrupt(I_TEMPERATURE_CHANGE);}
            if(target){
                uint8_t data = convertTemperatureD2I(target);
                write(TEMPERATURE_WINDOW, &data);
                }
            else{
                read(CTRL_REG_2, &data);
                data |= 0b00110000;
                write(CTRL_REG_2, &data);
                }
            break;
        }
        case I_NO_INTERRUPT: {break;}
    }
    read(CTRL_REG_4, &data);
    data |= name;
    write(CTRL_REG_4, &data);
    activeInterrupts |= name;
    if(pin==PIN_ONE){
        mInterruptOne.fall(callback(this, &MPL3115A2::dispatchInterruptData));
    }
    else{
        mInterruptTwo.fall(callback(this, &MPL3115A2::dispatchInterruptData));
    }
    setInterruptFunction(function);
    setActive();
}

void MPL3115A2::setInterrupt(Interrupt_Pin pin, Interrupt name, void (*function)(), bool overflow, uint8_t watermark){
    if(name==I_FIFO){
        standby();
        uint8_t data=0;
        write(FIFO_SETUP, &data);
        watermark%=64;
        data = watermark|1<<(7 - (uint8_t)overflow);
        write(FIFO_SETUP, &data);
        setInterrupt(pin, name, function);
    }
    //TODO Maybe send a warning if the user tries to setup an interrupt using unrelated parameters.
}


void MPL3115A2::removeInterrupt(Interrupt name){
    uint8_t data;
    if(name==I_FIFO){
        data = 0;
        write(FIFO_SETUP, &data);
        visFIFO = false;
    }
    read(CTRL_REG_4, &data);
    data &= ~name;
    write(CTRL_REG_4, &data);
    activeInterrupts &= ~name;
    if(!activeInterrupts){
        mInterruptOne.fall(NULL);
        mThread.terminate();
        MPL3115A2Interrupt = NULL;
        mInterruptTwo.fall(NULL);
    }
}

void MPL3115A2::setInterruptFunction(void (*function)()){
    MPL3115A2Interrupt = function;
    if(mThread.get_state()==Thread::Deleted){mThread.start(callback(this, &MPL3115A2::interruptWrapper));}

}

void MPL3115A2::interruptWrapper(){
    while(1){
        Thread::signal_wait(0x01);
        mail_t *mail = mailBox.alloc();
        uint8_t data = getStatus();
        Interrupt name = identifyInterrupt();
        while(name){
            switch(name){
                case I_ALTITUDE_CHANGE : ;
                case I_ALTITUDE_THRESHOLD : ;
                case I_ALTITUDE_WINDOW : {
                    if(isAltimeter()){
                        mail->type = TYPE_ALTITUDE;
                        }
                    else{
                        mail->type = TYPE_PRESSURE;
                        }
                    mail->value = getData();
                    mailBox.put(mail);
                    break;
                }
                case I_TEMPERATURE_CHANGE : ;
                case I_TEMPERATURE_THRESHOLD : ;
                case I_TEMPERATURE_WINDOW : {
                    mail->type = TYPE_TEMPERATURE;
                    mail->value = getTemperature();
                    mailBox.put(mail);
                    break;
                }
                case I_FIFO : {
                    mailBox.free(mail);
                    uint8_t samplesNumber;
                    read(FIFO_STATUS, &samplesNumber);
                    samplesNumber&=0x3F;
                    mail_t **mailArray = new mail_t*[2*samplesNumber];
                    for(int i=0;i<2*samplesNumber;i++){
                        *(mailArray+i)= mailBox.alloc();
                    }
                    uint8_t *samples = new uint8_t[5*samplesNumber];
                    read(FIFO_DATA, samples, 5*samplesNumber);
                    for(int i=0;i<samplesNumber-1;i++){
                        if(isAltimeter()){
                            (*(mailArray+2*i))->type = TYPE_ALTITUDE;
                            (*(mailArray+2*i))->value = convertAltitudeI2D(samples+5*i);
                        }
                        else{
                            (*(mailArray+2*i))->type = TYPE_PRESSURE;
                            (*(mailArray+2*i))->value = convertPressureI2D(samples+5*i);
                        }
                        (*(mailArray+2*i+1))->type = TYPE_TEMPERATURE;
                        (*(mailArray+2*i+1))->value = convertTemperatureI2D(samples+5*i+3);
                        mailBox.put(*(mailArray+2*i));
                        mailBox.put(*(mailArray+2*i+1));
                    }
                    delete[] samples;
                    break;
                }
                case I_NEW_DATA : {
                    switch(data&0xE){
                        case 0xE: {
                            mail_t *mail2 = mailBox.alloc();
                            mail2->type = TYPE_TEMPERATURE;
                            mail2->value = getTemperature();
                            mailBox.put(mail2);
                        }
                        case 0x8:{
                            if(isAltimeter()){
                                mail->type = TYPE_ALTITUDE;
                                mail->value = getData();
                                mailBox.put(mail);
                            }
                            else{
                                mail->type = TYPE_PRESSURE;
                                mail->value = getData();
                                mailBox.put(mail);
                            }
                            break;
                        }
                        case 0x2 : {
                            mail->type = TYPE_TEMPERATURE;
                            mail->value = getTemperature();
                            mailBox.put(mail); 
                        }
                    }
                    break;
                }
                case I_NO_INTERRUPT: {break;}
            }
            MPL3115A2Interrupt();
            name = identifyInterrupt();
        }
    }
}


void MPL3115A2::setMode(Mode mode){
    uint8_t data;
    read(CTRL_REG_1, &data);
    standby();
    data&=~(1);
    if(mode==ALTIMETER){
        data|=(1<<7);
        visAltimeter = true;
    }
    else{
        data&=~(1<<7);
        visAltimeter = false;
    }
    write(CTRL_REG_1, &data);
    setActive();
}


float MPL3115A2::convertAltitudeI2D(uint8_t *altitude){
    return  ( (float)
            (
            (((uint32_t) altitude[0]) << 24) | 
            (((uint32_t) altitude[1]) << 16) | 
            (((uint32_t) altitude[2]) << 8)
            )
            )   / 65536;
}

float MPL3115A2::convertPressureI2D(uint8_t *pressure){
    return  ( (float)
            (
            (((uint32_t) pressure[0]) << 16) | 
            (((uint32_t) pressure[1]) << 8) | 
            ((uint32_t) pressure[2])
            )
            ) / 64;
}

float MPL3115A2::convertTemperatureI2D(uint8_t *temperature){
    return  ( (float)
            ( 
            (((uint16_t) temperature[0]) << 8) |   
            ((uint16_t) temperature[1])
            )
            ) / 256;
}

void MPL3115A2::convertAltitudeD2I(float altitude, uint8_t* out){
    *out = (uint8_t) ((~lround(altitude)) + 1); 
}

void MPL3115A2::convertPressureD2I(float pressure, uint8_t* out){
    *out = (uint8_t) (~lround(pressure/2)) + 1;
}

uint8_t MPL3115A2::convertTemperatureD2I(float temperature){
    return (~lround(temperature)) + 1;
}

Interrupt MPL3115A2::identifyInterrupt(){
    uint8_t data;
    read(INTERRUPT_STATUS, &data);
    for(uint8_t i=0;i<8;i++){
        if(data&(1<<i)){return (Interrupt) (1<<i);}
    }
    return I_NO_INTERRUPT;
}

void MPL3115A2::dispatchInterruptData(){
    mThread.signal_set(0x01);
}

int MPL3115A2::write(Address address, uint8_t *data, int length){
    uint8_t *bigData = new uint8_t[length+1];
    *bigData = address;
    for(int i = 0; i < length; i++){
        *(bigData+i+1) = data[i];
    }
    int result = mI2C.write(mAddress, (char*) bigData, length + 1);
    delete[] bigData;
    return result; 
}

void MPL3115A2::read(Address address, uint8_t *data, int length){
    uint8_t addressP[] = {address}; 
    mI2C.write(mAddress, (char*) addressP, 1, true);
    mI2C.read(mAddress, (char*) data, length);
}


