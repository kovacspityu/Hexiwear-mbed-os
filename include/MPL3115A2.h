#ifndef _MPL3115A2_driver_
#define _MPL3115A2_driver_
#include "MPL3115A2_enum.h"
#include "mbed.h"

class MPL3115A2
{
    public:
    MPL3115A2(MPL3315A2_Mode mode = BAROMETER, MPL3315A2_Os_Ratio ratio = OS_512_MS);

    InterruptIn interruptOne, interruptTwo; 

    double getData();
    double getTemperature();
    double getDataDelta();
    double getTemperatureDelta();
    double getMaxData();
    double getMaxTemperature();
    double getMinData();
    double getMinTemperature();
    int getOsR();
    void setOsR(MPL3315A2_Os_Ratio ratio);
    int getTimeStep();
    void setTimeStep(MPL3115A2_Time_Step timeStep);
    void setInterrupt(MPL3115A2_Interrupt_Pin pin, MPL3115A2_Interrupt name, uint8_t[2] target = {0,0});
    bool isAltimeter();
    void setMode(MPL3315A2_Mode mode);

    private:

    I2C mI2C;
    uint8_t mAddress;
    void read(MPL3115A2_Address address, uint8_t *data, int length);
    int write(MPL3115A2_Address address, uint8_t *data);
};

#endif
