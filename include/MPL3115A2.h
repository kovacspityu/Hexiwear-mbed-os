#ifndef _MPL3115A2_driver_
#define _MPL3115A2_driver_
#include "MPL3115A2_enum.h"
#include "mbed.h"

class MPL3115A2{
public:
MPL3115A2(MPL3315A2_Mode mode = BAROMETER, MPL3315A2_Oversample_Ratio ratio = OS_6MS);
double getData();
double getTemperature();
double getDataDelta();
double getTemperatureDelta();
double getMaxData();
double getMaxTemperature();
double getMinData();
double getMinTemperature();
int getOsR();
void setOsR(MPL3315A2_Oversample_Ratio ratio);
bool isAltimeter();
void setMode(MPL3315A2_Mode mode);

private:

I2C mI2C;
uint8_t mAddress;
uint8_t read(MPL3115A2_register address, uint8_t *data, int length);
int write(MPL3115A2_register address, uint8_t *data);




}

#endif
