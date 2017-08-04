#ifndef _MPL3115A2_driver_
#define _MPL3115A2_driver_
#include "MPL3115A2_enum.h"
#include "mbed.h"

class MPL3115A2{
public:
MPL3115A2(MPL3115A2_Mode mode = BAROMETER, MPL3115A2_Oversample_Ratio ratio = OS_6MS);
double getData();
int getOsR();
void setOsR(MPL3315A2_Oversample_Ratio ration);

private:

I2C mI2C(PTC11, PTC10);
uint8_t mAddress = 0xC0;
uint8_t read(MPL3115A2_address address, uint8_t *data, int length);
int write(MPL3115A2_address address, uint8_t *data);




}

#endif
