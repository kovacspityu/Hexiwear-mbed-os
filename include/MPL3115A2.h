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
        float value;
    } mail_t;
    Mail<mail_t, 16> mailBox;
    
    void setActive();
    void standby();
    void refresh();
    void fullReset();
    bool isDataAvailable();
    float getData();
    float getTemperature();
    float getDataDelta();
    float getTemperatureDelta();
    float getMaxData();
    float getMaxTemperature();
    float getMinData();
    float getMinTemperature();
    uint8_t getActiveInterruptsOne();
    uint8_t getActiveInterruptsTwo();
    uint8_t getStatus();
    int getOsR();
    void setOsR(MPL3315A2_Os_Ratio ratio);
    int getTimeStep();
    void setTimeStep(MPL3115A2_Time_Step timeStep);
    void setInterrupt(MPL3115A2_Interrupt_Pin pin, MPL3115A2_Interrupt name, void (*function)(), float target = 0);    
    void removeInterrupt(MPL3115A2_Interrupt name);
    bool isAltimeter();
    void setMode(MPL3315A2_Mode mode);
    void setSeaLevelPressure(float pressure = 0);
    void setPressureOffset(uint8_t pressure);
    void setTemperatureOffset(uint8_t temperature);
    void setAltitudeOffset(uint8_t altitude);
    void setInterruptFunction(void (*function)(), MPL3115A2_Interrupt_Pin pin);

    float convertAltitudeI2D(uint8_t* altitude);
    float convertPressureI2D(uint8_t* pressure);

    MPL3115A2_Interrupt identifyInterrupt(MPL3115A2_Interrupt_Pin pin);
    void dispatchInterruptDataOne();
    void dispatchInterruptDataTwo();

    private:

    I2C mI2C;
    uint8_t mAddress;
    Thread threadOne;
    Thread threadTwo;
    InterruptIn mInterruptOne, mInterruptTwo;
    uint8_t activeInterruptsOne, activeInterruptsTwo;
    void interruptWrapper(MPL3115A2_Interrupt_Pin pin);
    void interruptWrapperOne();
    void interruptWrapperTwo();
    void (*MPL3115A2InterruptOne)();
    void (*MPL3115A2InterruptTwo)();
    
    void read(MPL3115A2_Address address, uint8_t *data, int length = 1);
    int write(MPL3115A2_Address address, uint8_t *data, int length = 1);
    void dispatchInterruptData(MPL3115A2_Interrupt_Pin pin);

    void convertPressureD2I(float pressure, uint8_t* out);
    void convertAltitudeD2I(float altitude, uint8_t* out);
    uint8_t convertTemperatureD2I(float temperature);
    float convertTemperatureI2D(uint8_t* temperature);
    
    
};

#endif
