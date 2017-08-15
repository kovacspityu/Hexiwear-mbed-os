#ifndef _MPL3115A2_driver_
#define _MPL3115A2_driver_
#include "MPL3115A2_enum.h"
#include "mbed.h"

class MPL3115A2
{
    public:
    MPL3115A2(MPL3315A2_Mode mode = BAROMETER, MPL3315A2_Os_Ratio ratio = OS_512_MS);

    typedef struct {
        MPL3115A2_Data_Type type;
        double value;
    } mail_t;
    Mail<mail_t, 16> mailBox;
    
    double getData();
    double getTemperature();
    double getDataDelta();
    double getTemperatureDelta();
    double getMaxData();
    double getMaxTemperature();
    double getMinData();
    double getMinTemperature();
    uint8_t getActiveInterruptsOne();
    uint8_t getActiveInterruptsTwo();
    uint8_t getStatus();
    int getOsR();
    void setOsR(MPL3315A2_Os_Ratio ratio);
    int getTimeStep();
    void setTimeStep(MPL3115A2_Time_Step timeStep);
    void setInterrupt(MPL3115A2_Interrupt_Pin pin, MPL3115A2_Interrupt name, double target = 0);
    void setInterrupt(MPL3115A2_Interrupt_Pin pin, MPL3115A2_Interrupt name, void (*function)(), double target = 0);    
    void removeInterrupt(MPL3115A2_Interrupt name);
    bool isAltimeter();
    void setMode(MPL3315A2_Mode mode);
    void setSeaLevelPressure(double pressure = 0);
    void setPressureOffset(uint8_t pressure);
    void setTemperatureOffset(uint8_t temperature);
    void setAltitudeOffset(uint8_t altitude);
    void setInterruptFunction(void (*function)());

    double  convertAltitudeI2D(uint8_t* altitude);
    double  convertPressureI2D(uint8_t* pressure);

    MPL3115A2_Interrupt identifyInterrupt(MPL3115A2_Interrupt_Pin pin);
    void dispatchInterruptDataOne();
    void dispatchInterruptDataTwo();

    private:

    I2C mI2C;
    uint8_t mAddress;
    Thread thread;
    InterruptIn mInterruptOne, mInterruptTwo;
    uint8_t activeInterruptsOne, activeInterruptsTwo;
    void (*interruptFunction)();
    
    void read(MPL3115A2_Address address, uint8_t *data, int length = 1);
    int write(MPL3115A2_Address address, uint8_t *data, int length = 1);
    void dispatchInterruptData(MPL3115A2_Interrupt_Pin pin);

    void convertPressureD2I(double pressure, uint8_t* out);
    void convertAltitudeD2I(double altitude, uint8_t* out);
    uint8_t convertTemperatureD2I(double temperature);
    double convertTemperatureI2D(uint8_t* temperature);
    
    
};

#endif
