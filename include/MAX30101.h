#ifndef _MAX30101_
#define _MAX30101_

#include "mbed.h"
#include "MAX30101_enum.h"


#define MIN_HEART_FREQUENCY 50u
#define MAX_HEART_FREQUENCY 200u

class MAX30101{
    public:
        MAX30101(MAX30101_Mode mode = MAX_HR_MODE, MAX30101_Oversample oversample = MAX_OVERSAMPLE_1,
bool fifoRollover = false, uint8_t fifoThreshold = 0, MAX30101_Led slot1 = MAX_LED_RED, MAX30101_Led slot2 = MAX_LED_NONE,
MAX30101_Led slot3 = MAX_LED_NONE, MAX30101_Led slot4 = MAX_LED_NONE);

        ~MAX30101();
        
        typedef struct{
            MAX30101_Led ledType;
            uint32_t value;
        } ledSample_t;

        typedef struct{
            ledSample_t *ledSamples;
            uint8_t length;
            float temperature;
        } mail_t;

        Mail<mail_t, 32> mailBox;

        void standby();
        void powerUp();
        void reset();
        void powerDown();

        uint8_t getHR(uint32_t* values, uint16_t length);
        
        mail_t getSampleTemplate();
        mail_t* getData(uint8_t numberOfSamples = 0);

        void setOversample(MAX30101_Oversample oversample);
        void setFIFORollover(bool fifoRollover);
        void setFIFOThreshold(uint8_t fifoThreshold);
        void setMode(MAX30101_Mode mode, MAX30101_Led slot1 = MAX_LED_NONE, 
            MAX30101_Led slot2 = MAX_LED_NONE, MAX30101_Led slot3 = MAX_LED_NONE, 
            MAX30101_Led slot4 = MAX_LED_NONE);
        void setPulseAmplitude(uint8_t redAmplitude, uint8_t irAmplitude, 
                            uint8_t greenAmplitude = 0, uint8_t pilotAmplitude = 0);
        void setPulseWidth(MAX30101_Pulse_Width width);
        void setOxygenRate(MAX30101_Oxygen_Rate rate);
        void setOxygenRange(MAX30101_Oxygen_Range range);
        void setProximityDelay(uint8_t delay);
        void setMultiLedTiming(MAX30101_Led slot1, MAX30101_Led slot2, MAX30101_Led slot3, MAX30101_Led slot4);

        void setInterrupt(MAX30101_Interrupt interrupt, void (*function)(), uint8_t threshold = 0, bool fifoRollover = false);
        void removeInterrupt(MAX30101_Interrupt interrupt);


    private:
        DigitalOut mPower;
        I2C mI2C;
        InterruptIn mInterrupt;
        uint8_t mAddress;
        Thread mThread;
        uint8_t mResolution;
        float mTemperature;
        MAX30101_Oxygen_Rate mSampleRate;
        Ticker mTicker;
        mail_t mSampleTemplate;

        void startTemperatureMeasurement();
        float getTemperature();

        void setInterruptFunction(void (*function)());
        void interruptWrapper();

        void (*mInterruptFunction)();
        void clearInterrupt();
        void dispatchInterruptData();

        void clearFIFOCounters();
        void updateChannels(MAX30101_Mode mode, MAX30101_Led slot1 = MAX_LED_NONE, 
                            MAX30101_Led slot2 = MAX_LED_NONE, MAX30101_Led slot3 = MAX_LED_NONE, 
                            MAX30101_Led slot4 = MAX_LED_NONE);
        
        int write(MAX30101_Address address, uint8_t *data, int length = 1);
        void read(MAX30101_Address address, uint8_t *data, int length = 1);

};


#endif