#ifndef _TSL2561_DRIVER_
#define _TSL2561_DRIVER_
#include "TSL2561_enum.h"
#include "mbed.h"
#include "rtos.h"


class TSL2561{
    public:
        TSL2561(TSL::Gain gain = TSL::LOW_GAIN, TSL::Os_Rate rate = TSL::OS_400MS);
        
        Mail<float, 16> mailBox;

        bool isActive();
        float getLux();
        void powerUp();
        void powerDown();
        void reset();
        void setGain(TSL::Gain gain);
        void setOSRate(TSL::Os_Rate rate);
        //void setInterrupt(float lowThreshold, float highThreshold, TSL::Interrupt_Length persistence);
        void setInterrupt(int lowPercentage, int highPercentage, TSL::Interrupt_Length persistance, void (*function)());
        void removeInterrupt();

        void setDebugInterrupt(void (*function)());

        
    private:
        DigitalOut mPower;
        I2C mI2C;
        InterruptIn mInterrupt;
        uint8_t mAddress;
        TSL::Gain mGain;
        TSL::Os_Rate mRate;
        Thread mThread;
        uint8_t mFix;

        void setInterruptFunction(void (*function)());
        void interruptWrapper();

        void getRawLux(uint8_t *rawLight);
        float formatLux(uint8_t *light);
        uint8_t (*convertLuxD2I)(float lux);

        void (*mInterruptFunction)();
        void clearInterrupt();
        void dispatchInterruptData();
        void dispatchWrongSensitivity(float lux);
        void wait();


        void read(TSL::Address address, uint8_t *data, int length = 1);
        int write(TSL::Address address, uint8_t *data, int length = 1);
};

#endif