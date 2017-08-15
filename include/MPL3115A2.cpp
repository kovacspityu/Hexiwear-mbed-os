#include "MPL3115A2_enum.h"
#include "MPL3115A2.h"
#include "mbed.h"
#include "rtos.h"
#include <cmath>
#include <cstdlib>

MPL3115A2::MPL3115A2(MPL3315A2_Mode mode, MPL3315A2_Os_Ratio ratio) : 
mI2C(I2C_SDA, I2C_SCL), mAddress(0xC0), mInterruptOne(PTD12), mInterruptTwo(PTD10),
activeInterruptsOne(0), activeInterruptsTwo(0)
{
    uint8_t data = 1  + (ratio<<3) + (mode<<7);
    write(CTRL_REG_1, &data);
    mInterruptOne.fall(callback(this, &MPL3115A2::dispatchInterruptDataOne));
    mInterruptTwo.fall(callback(this, &MPL3115A2::dispatchInterruptDataTwo));
}

int MPL3115A2::write(MPL3115A2_Address address, uint8_t *data, int length){
    uint8_t bigData[length+1];
    bigData[0] = address;
    for(int i = 0; i < length; i++){
        bigData[i+1] = data[i];
    }
    return mI2C.write(mAddress, (char*) bigData, 1 + length); 
}

void MPL3115A2::read(MPL3115A2_Address address, uint8_t *data, int length){
    mI2C.write(mAddress, (char*) address, 1, true);
    mI2C.read(mAddress, (char*) data, length);
}

uint8_t MPL3115A2::getActiveInterruptsOne(){
    return activeInterruptsOne;
}

uint8_t MPL3115A2::getActiveInterruptsTwo(){
    return activeInterruptsTwo;
}

uint8_t MPL3115A2::getStatus(){
    uint8_t data;
    read(DR_STATUS, &data);
    return data;
}

int MPL3115A2::getOsR(){
    uint8_t data;
    read(CTRL_REG_1, &data, 1);
    return (data&0b00111000)>>3;
}

void MPL3115A2::setOsR(MPL3315A2_Os_Ratio ratio){
    uint8_t data;
    read(CTRL_REG_1, &data, 1);
    data&=~1;
    write(CTRL_REG_1, &data);
    data&=0b11000111;
    data|=((ratio<<3) + 1);
    write(CTRL_REG_1, &data);
}

int MPL3115A2::getTimeStep(){
    uint8_t data;
    read(CTRL_REG_2, &data);
    data&=0b00001111;
    return data;
}

void MPL3115A2::setTimeStep(MPL3115A2_Time_Step timeStep){
    uint8_t data;
    read(CTRL_REG_2, &data);
    data|=timeStep;
    write(CTRL_REG_2, &data);
}

bool MPL3115A2::isAltimeter(){
    uint8_t data;
    read(CTRL_REG_1, &data, 1);
    return (data&(1<<7));
}

double MPL3115A2::getData(){
    uint8_t data[3];
    read(PRESSURE_MSB, data, 3);
    if (isAltimeter()){return convertAltitudeI2D(data);}
    else{return convertPressureI2D(data);}
}

double MPL3115A2::getTemperature(){
    uint8_t data[2];
    read(TEMPERATURE_MSB, data, 2);
    return convertTemperatureI2D(data);
}

double MPL3115A2::getDataDelta(){
    uint8_t data[3];
    read(PRESSURE_DELTA_MSB, data, 3);
    if (isAltimeter()){return convertAltitudeI2D(data);}
    else{return convertPressureI2D(data);}
}

double MPL3115A2::getTemperatureDelta(){
    uint8_t data[2];
    read(TEMPERATURE_DELTA_MSB, data, 2);
    return convertTemperatureI2D(data);
}

double MPL3115A2::getMaxData(){
    uint8_t data[3];
    read(MAX_PRESSURE_MSB, data, 3);
    if (isAltimeter()){return convertAltitudeI2D(data);}
    else{return convertPressureI2D(data);}
}
double MPL3115A2::getMaxTemperature(){
    uint8_t data[2];
    read(MAX_TEMPERATURE_MSB, data, 2);
    return convertTemperatureI2D(data);
}
double MPL3115A2::getMinData(){
    uint8_t data[3];
    read(MIN_PRESSURE_MSB, data, 3);
    if (isAltimeter()){return convertAltitudeI2D(data);}
    else{return convertPressureI2D(data);}
}
double MPL3115A2::getMinTemperature(){
    uint8_t data[2];
    read(MAX_TEMPERATURE_MSB, data, 2);
    return convertTemperatureI2D(data);
}

void MPL3115A2::setSeaLevelPressure(double pressure){
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

void MPL3115A2::setPressureOffset(uint8_t pressure){
    pressure = ~(pressure*4) + 1;
    write(PRESSURE_OFFSET, &pressure); 
}

void MPL3115A2::setTemperatureOffset(uint8_t temperature){
    temperature = (~div(temperature, 16).quot) + 1;
    write(PRESSURE_OFFSET, &temperature); 
}

void MPL3115A2::setAltitudeOffset(uint8_t altitude){
    altitude = (~altitude) + 1;
    write(PRESSURE_OFFSET, &altitude); 
}

void MPL3115A2::setInterrupt(MPL3115A2_Interrupt_Pin pin, MPL3115A2_Interrupt name, double target){
    uint8_t data;
    read(CTRL_REG_5, &data);
    if(pin) {data |= name;}
    else    {data &= ~name;}
    write(CTRL_REG_5, &data);
    switch(name){
        case I_FIFO : {
//TODO Need to correctly change the execution mode to FIFO, maybe with user provided options.
//The associated register is called FIFO_SETUP
            break;
        }
        case I_NEW_DATA : {
//TODO Right now it lacks functionality, the hardware can interrupt on just new pressure/altitude or new temperature
            uint8_t data = 0b00000111; 
            write(EVENT_CONFIG, &data);
            break;
        }

        case I_PRESSURE_CHANGE : {
//TODO Check that this interrupt isn't active together with the window one, 
//as they use the same register for the window value
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
//TODO Ensure that this interrupt and temperature window are not active at the same time, 
//as they use the same register for the window.
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
        case I_ALTITUDE_THRESHOLD: {
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
    }
    read(CTRL_REG_4, &data);
    data &= name;
    write(CTRL_REG_4, &data);
    if(pin){activeInterruptsOne |= name;}
    else{activeInterruptsTwo &= ~name;}
}

void MPL3115A2::setInterrupt(MPL3115A2_Interrupt_Pin pin, MPL3115A2_Interrupt name, void (*function)(), double target){
    setInterrupt(pin, name, target);
    setInterruptFunction(function);
}    


void MPL3115A2::removeInterrupt(MPL3115A2_Interrupt name){
    uint8_t data;
    //TODO Specific code for FIFO interrupt
    if(name==I_NEW_DATA){
        uint8_t data = 0;
        write(EVENT_CONFIG, &data);
    }
    read(CTRL_REG_4, &data);
    data &= ~name;
    write(CTRL_REG_4, &data);
    if(activeInterruptsOne&name){activeInterruptsOne &= ~name;}
    else{activeInterruptsTwo &=~ name;}
    if(!(activeInterruptsOne || activeInterruptsTwo)){thread.terminate();}
}

void MPL3115A2::setInterruptFunction(void (*function)()){
    thread.start(*function);
    interruptFunction = function;
}

void MPL3115A2::dispatchInterruptDataOne(){
    dispatchInterruptData(PIN_ONE);
}

void MPL3115A2::dispatchInterruptDataTwo(){
    dispatchInterruptData(PIN_TWO);
}


double MPL3115A2::convertAltitudeI2D(uint8_t *altitude){
    return  (
            (((uint32_t) altitude[0]) << 24) | 
            (((uint32_t) altitude[1]) << 16) | 
            (((uint32_t) altitude[2]) << 8)
            ) / 65536;
}

double MPL3115A2::convertPressureI2D(uint8_t *pressure){
    return  (
            (((uint32_t) pressure[0]) << 16) | 
            (((uint32_t) pressure[1]) << 8) | 
            ((uint32_t) pressure[2])
            ) / 64;
}

double MPL3115A2::convertTemperatureI2D(uint8_t *temperature){
    return  (
            (((uint16_t) temperature[0]) << 8) |   
             ((uint16_t) temperature[1])
            ) / 256;
}

void MPL3115A2::convertAltitudeD2I(double altitude, uint8_t* out){
    *out = (uint8_t)((~lround(altitude)) + 1); 
}

void MPL3115A2::convertPressureD2I(double pressure, uint8_t* out){
    *out = (uint8_t) (~lround(pressure/2)) + 1;
}

uint8_t MPL3115A2::convertTemperatureD2I(double temperature){
    return (~lround(temperature)) + 1;
}

MPL3115A2_Interrupt MPL3115A2::identifyInterrupt(MPL3115A2_Interrupt_Pin pin){
    uint8_t data;
    read(INTERRUPT_STATUS, &data);
    if(pin){return static_cast<MPL3115A2_Interrupt> (lround(activeInterruptsOne & data));}
    else{return static_cast<MPL3115A2_Interrupt> (lround(activeInterruptsTwo & data));}
    }

void MPL3115A2::dispatchInterruptData(MPL3115A2_Interrupt_Pin pin){
    mail_t *mail = mailBox.alloc();
    switch(identifyInterrupt(pin)){
        case I_ALTITUDE_CHANGE : ;
        case I_ALTITUDE_THRESHOLD : ;
        case I_ALTITUDE_WINDOW : {
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
        case I_TEMPERATURE_CHANGE : ;
        case I_TEMPERATURE_THRESHOLD : ;
        case I_TEMPERATURE_WINDOW : {
            mail->type = TYPE_TEMPERATURE;
            mail->value = getTemperature();
            mailBox.put(mail);
            break;
        }
        case I_FIFO : {
            //TODO
            break;
        }
        case I_NEW_DATA : {
            uint8_t data = getStatus();
            if(data&2){
                mail->type = TYPE_TEMPERATURE;
                mail->value = getTemperature();
                mailBox.put(mail);
            }
            else{
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
            }
            break;
        }
    }
    getStatus();
    thread.signal_set(0x01);
}


