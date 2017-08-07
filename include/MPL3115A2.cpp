#include "MPL3115A2_enum.h"
#include "MPL3115A2.h"
#include "mbed.h"

MPL3115A2::MPL3115A2(MPL3315A2_Mode mode, MPL3315A2_Os_Ratio ratio) : 
mI2C(PTC11, PTC10), mAddress(0xC0), interruptOne(PTD12), interruptTwo(PTD10)
{
    uint8_t data = 1  + (ratio<<3) + (mode<<7);
    write(CTRL_REG_1, &data);
}

int MPL3115A2::write(MPL3115A2_Address address, uint8_t *data){
    const char bigData[2] = {(char) address, (char) *data};
    return mI2C.write(mAddress, bigData, 2); 
}

void MPL3115A2::read(MPL3115A2_Address address, uint8_t *data, int length){
    const char cAddress =  address;
    mI2C.write(mAddress, &cAddress, 1, true);
    mI2C.read(mAddress, (char*) data, length);
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
    write(CTRL_REG_2, &data, 1);
}

bool MPL3115A2::isAltimeter(){
    uint8_t data;
    read(CTRL_REG_1, &data, 1);
    return (data&(1<<7));
}

double MPL3115A2::getData(){
    uint8_t data[3];
    double returnValue;
    read(PRESSURE_MSB, data, 3);
    if (isAltimeter()){
        returnValue =    (
                            (((uint32_t) data[0]) << 24) | 
                            (((uint32_t) data[1]) << 16) | 
                            (((uint32_t) data[2]) << 8)
                            ) / 65536;
    }
    else{
        returnValue =    (
                            (((uint32_t) data[0]) << 16) | 
                            (((uint32_t) data[1]) << 8) | 
                            ((uint32_t) data[2])
                            ) / 64;
    }
    return returnValue;
}

double MPL3115A2::getTemperature(){
    uint8_t data[2];
    double returnValue;
    read(TEMPERATURE_MSB, data, 2);
    returnValue =        (
                            (((uint16_t) data[0]) << 8) |
                            ((uint16_t) data[1])
                            ) / 256;
    return returnValue;
}

double MPL3115A2::getDataDelta(){
    uint8_t data[3];
    double returnValue;
    read(PRESSURE_DELTA_MSB, data, 3);
    if (isAltimeter()){
        returnValue =    (
                            (((uint32_t) data[0]) << 24) | 
                            (((uint32_t) data[1]) << 16) | 
                            (((uint32_t) data[2]) << 8)
                            ) / 65536;
    }
    else{
        returnValue =    (
                            (((uint32_t) data[0]) << 16) | 
                            (((uint32_t) data[1]) << 8) | 
                            ((uint32_t) data[2])
                            ) / 64;
    }
    return returnValue;
}

double MPL3115A2::getTemperatureDelta(){
    uint8_t data[2];
    double returnValue;
    read(TEMPERATURE_DELTA_MSB, data, 2);
    returnValue =        (
                            (((uint16_t) data[0]) << 8) |
                            ((uint16_t) data[1])
                            ) / 256;
    return returnValue;
}

double MPL3115A2::getMaxData(){
    uint8_t data[3];
    double returnValue;
    read(MAX_PRESSURE_MSB, data, 3);
    if (isAltimeter()){
        returnValue =    (
                            (((uint32_t) data[0]) << 24) | 
                            (((uint32_t) data[1]) << 16) | 
                            (((uint32_t) data[2]) << 8)
                            ) / 65536;
    }
    else{
        returnValue =    (
                            (((uint32_t) data[0]) << 16) | 
                            (((uint32_t) data[1]) << 8) | 
                            ((uint32_t) data[2])
                            ) / 64;
    }
    return returnValue;
}
double MPL3115A2::getMaxTemperature(){
    uint8_t data[2];
    double returnValue;
    read(MAX_TEMPERATURE_MSB, data, 2);
    returnValue =        (
                            (((uint16_t) data[0]) << 8) |
                            ((uint16_t) data[1])
                            ) / 256;
    return returnValue;
}
double MPL3115A2::getMinData(){
    uint8_t data[3];
    double returnValue;
    read(MIN_PRESSURE_MSB, data, 3);
    if (isAltimeter()){
        returnValue =    (
                            (((uint32_t) data[0]) << 24) | 
                            (((uint32_t) data[1]) << 16) | 
                            (((uint32_t) data[2]) << 8)
                            ) / 65536;
    }
    else{
        returnValue =    (
                            (((uint32_t) data[0]) << 16) | 
                            (((uint32_t) data[1]) << 8) | 
                            ((uint32_t) data[2])
                            ) / 64;
    }
    return returnValue;
}
double MPL3115A2::getMinTemperature(){
    uint8_t data[2];
    double returnValue;
    read(MAX_TEMPERATURE_MSB, data, 2);
    returnValue =        (
                            (((uint16_t) data[0]) << 8) |
                            ((uint16_t) data[1])
                            ) / 256;
    return returnValue;
}

void MPL3115A2::setInterrupt(MPL3115A2_Interrupt_Pin pin, MPL3115A2_Interrupt name){
    uint8_t data;
    read(CTRL_REG_5, &data);
    if(pin) {data |= (pin<<name);}
    else    {data &= ~(1<<name);}
    write(CTRL_REG_5, &data, 1);
    switch(name){
        case I_FIFO : {
//Need to correctly change the execution mode to FIFO, maybe with user provided options.
//The associated register is called FIFO_SETUP
            break;
        }
        case I_NEW_DATA : {
//Need to look at what the EVENT_CONFIG register does exactly
            break;
        }
        case I_ALTITUDE_THRESHOLD: ;
        case I_PRESSURE_THRESHOLD: {
//CTRL_REG_2 allows to select either the current pressure/altitude as target,
//or a provided one to load into PRESSURE_TARGET
            break;
        }
        case I_TEMPERATURE_THRESHOLD : {
//CTRL_REG_2 allows to select either the current temperature as target,
//or a provided one to load into TEMPERATURE_TARGET
            break;
        }
        case I_ALTITUDE_WINDOW : ;
        case I_PRESSURE_WINDOW : {
//CTRL_REG_2 allows to select either the current pressure/altitude as window (bad idea),
//or a provided one to load into PRESSURE_WINDOW. In that case Interrupt is generated for
//Current altitude = PRESSURE_TARGET ± PRESSURE_WINDOW
            break;
        }
        case I_TEMPERATURE_WINDOW : {
//CTRL_REG_2 allows to select either the current temperature as window (bad idea),
//or a provided one to load into TEMPERATURE_WINDOW. In that case Interrupt is generated for
//Current temperature = TEMPERATURE_TARGET ± TEMPERATURE_WINDOW
        }
    }
    read(CTRL_REG_4, &data);
    data &= name;

}