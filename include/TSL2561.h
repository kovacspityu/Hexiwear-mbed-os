#include "TSL2561_enum.h"
#include "mbed.h"
#include "rtos.h"

class TSL2561{
    public:
        TSL2561(TSL2561_GAIN gain = LOW_GAIN, TSL2561_OS_RATE rate = OS_400MS);
        
        Mail<double, 16> mailBox;

        bool isActive();
        double getLux();
        void powerUp();
        void powerDown();
        void reset();
        void setGain(TSL2561_GAIN gain);
        void setOSRate(TSL2561_OS_RATE rate);
        //void setInterrupt(float lowThreshold, float highThreshold, TSL2561_Interrupt_Length persistence);
        void setInterrupt(int lowPercentage, int highPercentage, TSL2561_Interrupt_Length persistance, void (*function)());
        void removeInterrupt();

        void setDebugInterrupt(void (*function)());

        
    private:
        I2C mI2C;
        InterruptIn mInterrupt;
        uint8_t mAddress;
        TSL2561_GAIN mGain;
        TSL2561_OS_RATE mRate;
        Thread mThread;
        uint8_t mFix;

        void getRawLux(uint8_t *rawLight);
        double formatLux(uint8_t *light);
        uint8_t (*convertLuxD2I)(double lux);

        void (*mInterruptFunction)();
        void clearInterrupt();
        void dispatchInterruptData();
        void dispatchWrongGain();
        void autoAdjustGain();


        void read(TSL2561_Address address, uint8_t *data, int length = 1);
        bool write(TSL2561_Address address, uint8_t *data, int length = 1);
};