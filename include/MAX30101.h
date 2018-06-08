#ifndef _MAX30101_
#define _MAX30101_

#include "mbed.h"
#include "MAX30101_enum.h"


#define MIN_HEART_FREQUENCY 50u
#define MAX_HEART_FREQUENCY 200u


class MAX30101{
    public:
        MAX30101(MAX::Mode mode = MAX::HR_MODE, MAX::Oversample oversample = MAX::OVERSAMPLE_1,
bool fifoRollover = false, uint8_t fifoThreshold = 0, MAX::Led slot1 = MAX::RED_LED, MAX::Led slot2 = MAX::NONE_LED,
MAX::Led slot3 = MAX::NONE_LED, MAX::Led slot4 = MAX::NONE_LED);

        ~MAX30101();
        
        typedef struct{
            MAX::Led ledType;
            uint32_t value;
        } ledSample_t;

        typedef struct{
            ledSample_t *ledSamples;
            uint8_t length;
            float temperature;
        } mail_t;

        typedef struct{
            uint8_t hr;
            float weight;
        } uint8AndFloat;

        Mail<mail_t, 32> mailBox;

        void standby();
        void powerUp();
        void reset();
        void powerDown();

        uint8AndFloat getHR(uint32_t* values, uint16_t length);
        
        mail_t getSampleTemplate();
        mail_t* getData(uint8_t numberOfSamples = 0);

        void setOversample(MAX::Oversample oversample);
        void setFIFORollover(bool fifoRollover);
        void setFIFOThreshold(uint8_t fifoThreshold);
        void setMode(   MAX::Mode mode, MAX::Led slot1 = MAX::NONE_LED, 
                        MAX::Led slot2 = MAX::NONE_LED, MAX::Led slot3 = MAX::NONE_LED, 
                        MAX::Led slot4 = MAX::NONE_LED);
        void setPulseAmplitude(uint8_t redAmplitude, uint8_t irAmplitude, 
                            uint8_t greenAmplitude = 0, uint8_t pilotAmplitude = 0);
        void setPulseWidth(MAX::Pulse_Width width);
        void setOxygenRate(MAX::Oxygen_Rate rate);
        void setOxygenRange(MAX::Oxygen_Range range);
        void setProximityDelay(uint8_t delay);
        void setMultiLedTiming(MAX::Led slot1, MAX::Led slot2, MAX::Led slot3, MAX::Led slot4);

        void setInterrupt(MAX::Interrupt name, void (*function)(), uint8_t threshold = 0, bool fifoRollover = false);
        void removeInterrupt(MAX::Interrupt name);

        uint8_t combineLeds(uint8AndFloat* leds, uint8_t length);


    private:
        DigitalOut mPower;
        I2C mI2C;
        InterruptIn mInterrupt;
        uint8_t mAddress;
        Thread mThread, tThread;
        uint8_t mResolution;
        float mTemperature;
        MAX::Oxygen_Rate mSampleRate;
        Ticker mTicker;
        mail_t mSampleTemplate;

        void startTemperatureMeasurement();
        float getTemperature();
        void requestTemperatureMeasure();
        void updateTemperature();

        void setInterruptFunction(void (*function)());
        void interruptWrapper();

        void (*mInterruptFunction)();
        void clearInterrupt();
        void dispatchInterruptData();

        MAX::Interrupt identifyInterrupt();

        void clearFIFOCounters();
        void updateChannels(MAX::Mode mode, MAX::Led slot1 = MAX::NONE_LED, 
                            MAX::Led slot2 = MAX::NONE_LED, MAX::Led slot3 = MAX::NONE_LED, 
                            MAX::Led slot4 = MAX::NONE_LED);
        
        int write(MAX::Address address, uint8_t *data, int length = 1);
        void read(MAX::Address address, uint8_t *data, int length = 1);

};


#endif