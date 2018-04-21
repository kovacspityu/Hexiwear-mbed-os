#ifndef _FXOS8700CQ_Driver_
#define _FXOS8700CQ_Driver_

#include "mbed.h"
#include "FXOS8700CQ_enum.h"

using namespace FXO;

class FXOS8700CQ{

    public:
        FXOS8700CQ(FXOS8700CQ_Mode mode = ACCELEROMETER, FXOS8700CQ_Range range = RANGE2000, FXOS8700CQ_ODR dataRate = ODR100);

        static const float TEMPERATURE_SENSITIVITY;
        
        typedef struct{
            FXOS8700CQ_Axis axis;
            float value;
        } mail_t;
        Mail<mail_t, 24> mailBox;

        void setMode(FXOS8700CQ_Mode mode);
        void standby();
        void setAccelerometer();
        void setMagnetometer();
        void setHybrid();
        FXOS8700CQ_Mode getStatus();
        bool isDataAvailable();

        void setRange(FXOS8700CQ_Range range);
        void setLowPass(FXOS8700CQ_Low threshold);
        void setHighPass(FXOS8700CQ_High threshold);
        void setODR(FXOS8700CQ_ODR dataRate);
        void setAsleepODR(FXOS8700CQ_ODR dataRate);
        void setMagOversample(FXOS8700CQ_Mag_OSR oversample);
        void setAccOversampleAwake(FXOS8700CQ_Acc_OSR oversample);
        void setAccOversampleAsleep(FXOS8700CQ_Acc_OSR oversample);
        void setAccelerationMagnitude(float count, bool resetCount, uint8_t config, float threshold, float* reference);
        void setFreefallMotion(float count, bool resetCount, uint8_t config, float threshold, float xThreshold=0, float yThreshold=0, float zThreshold=0);
        void setPulse(uint8_t config, float timing, float* threshold, float latency, float window);


        float* getAcceleration();
        float* getMagnetic();
        float getTemperature();
        void setInterrupt(FXOS8700CQ_Interrupt_Pin pin, FXOS8700CQ_Interrupt name, void (*function)(), float threshold = 0, float count = 0, bool resetCount = false, uint8_t config = 0);
        void setInterrupt(FXOS8700CQ_Interrupt_Pin pin, FXOS8700CQ_Interrupt name, void (*function)(), FXOS8700CQ_FIFO_Mode mode, uint8_t watermark = 0);
        void removeInterrupt(FXOS8700CQ_Interrupt name);
        

    private:
        I2C mI2C;
        uint8_t mAddress;
        uint16_t activeInterruptsOne;
        uint16_t activeInterruptsTwo;
        float mSensitivity;
        bool awake;
        FXOS8700CQ_ODR mODR;
        FXOS8700CQ_ODR mSleepODR;
        FXOS8700CQ_ODR mAwakeODR;
        FXOS8700CQ_Mode mMode;
        InterruptIn mInterruptOne;
        InterruptIn mInterruptTwo;
        DigitalIn mReset;
        Thread mThreadOne;
        Thread mThreadTwo;
        void interruptWrapper(FXOS8700CQ_Interrupt_Pin pin);
        void interruptWrapperOne();
        void interruptWrapperTwo();
        void (*FXOS8700CQInterruptOne)();
        void (*FXOS8700CQInterruptTwo)();

        uint8_t* getRawAcceleration();
        uint8_t* getRawMagnetic();
        
        void dispatchInterruptData(FXOS8700CQ_Interrupt_Pin pin);
        void dispatchInterruptDataOne();
        void dispatchInterruptDataTwo();
        void setInterruptFunction(void (*function)(), FXOS8700CQ_Interrupt_Pin pin);
        FXOS8700CQ_Interrupt identifyInterrupt(FXOS8700CQ_Interrupt_Pin pin);

        float convertAcceleration(uint8_t *rawAcc);
        float convertMagnetic(uint8_t *rawMag);

        void read(FXOS8700CQ_Address address, uint8_t *data, int length = 1);
        int write(FXOS8700CQ_Address address, uint8_t *data, int length = 1);
};



#endif