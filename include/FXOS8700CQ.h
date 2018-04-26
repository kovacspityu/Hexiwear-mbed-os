#ifndef _FXOS8700CQ_Driver_
#define _FXOS8700CQ_Driver_

#include "mbed.h"
#include "FXOS8700CQ_enum.h"

using namespace FXO;

class FXOS8700CQ{

    public:
        FXOS8700CQ(FXOS8700CQ_Mode mode = ACCELEROMETER, FXOS8700CQ_Range range = RANGE2000, FXOS8700CQ_ODR awakeODR = ODR400, FXOS8700CQ_ODR asleepODR = ODR400);

        static const float TEMPERATURE_SENSITIVITY;
        static const float MAGNETIC_SENSITIVITY;
        static const float BASE_ACC_SENSITIVITY;
        
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
        void setAwakeODR(FXOS8700CQ_ODR dataRate);
        void setAsleepODR(FXOS8700CQ_ODR dataRate);
        bool setAccLowNoise(bool activated=true);
        void setMagOversampleAwake(FXOS8700CQ_Mag_OSR oversample);
        void setMagOversampleAsleep(FXOS8700CQ_Mag_OSR oversample);
        void setAccOversampleAwake(FXOS8700CQ_Acc_OSR oversample);
        void setAccOversampleAsleep(FXOS8700CQ_Acc_OSR oversample);
        void setAccelerationMagnitude(FXOS8700CQ_Interrupt_Pin pin, void (*function)(), uint8_t count, bool resetCount, uint8_t config, float threshold, float* reference);
        void setFreefallMotion(FXOS8700CQ_Interrupt_Pin pin, void (*function)(), float count, bool resetCount, uint8_t config, float threshold, float xThreshold=0, float yThreshold=0, float zThreshold=0);
        void setPulse(FXOS8700CQ_Interrupt_Pin pin, void (*function)(), uint8_t config, float timing, float* threshold, float latency, float window);
        void setOrientation(FXOS8700CQ_Interrupt_Pin pin, void (*function)(), float count, bool resetCount, FXOS8700CQ_Lockout_Angle lockout, FXOS8700CQ_Orientation_Angle angle, FXOS8700CQ_Trip_Threshold threshold, FXOS8700CQ_Trip_Delta delta);
        void setTransient(FXOS8700CQ_Interrupt_Pin pin, void (*function)(), float count, bool resetCount, uint8_t config, float threshold);
        void setSleepWake(FXOS8700CQ_Interrupt_Pin pin, void (*function)(), float count, bool resetCount, uint8_t config, uint8_t interrupts);
        void setMagneticThreshold(FXOS8700CQ_Interrupt_Pin pin, void (*function)(), float count, bool resetCount, uint8_t config, float *threshold);
        void setMagneticMagnitude(FXOS8700CQ_Interrupt_Pin pin, void (*function)(), uint8_t count, bool resetCount, uint8_t config, float threshold, float* reference);


        float* getAcceleration();
        float* getMagnetic();
        float* getAllData();
        float getTemperature();
        void setInterrupt(FXOS8700CQ_Interrupt_Pin pin, FXOS8700CQ_Interrupt name, void (*function)());
        void setInterrupt(FXOS8700CQ_Interrupt_Pin pin, FXOS8700CQ_Interrupt name, void (*function)(), FXOS8700CQ_FIFO_Mode mode, uint8_t watermark = 0, uint8_t config = 0);
        void removeInterrupt(FXOS8700CQ_Interrupt name);
        

    private:
        I2C mI2C;
        uint8_t mAddress;
        uint16_t activeInterrupts;
        float mAccSensitivity;
        bool awake;
        FXOS8700CQ_ODR mODR;
        FXOS8700CQ_ODR mSleepODR;
        FXOS8700CQ_ODR mAwakeODR;
        FXOS8700CQ_Mode mMode;
        InterruptIn mInterruptOne;
        InterruptIn mInterruptTwo;
        DigitalIn mReset;
        Thread mThread;
        EventFlags mEvent;
        void interruptWrapper();
        void (*mInterrupts[])();

        uint8_t* getRawAcceleration();
        uint8_t* getRawMagnetic();
        uint8_t* getAllRawData();
        
        void dispatchInterruptData();
        void setInterruptFunction(void (*function)(), FXOS8700CQ_Interrupt name);
        FXOS8700CQ_Interrupt identifyInterrupt();

        float convertAcceleration(uint8_t *rawAcc);
        float convertMagnetic(uint8_t *rawMag);

        void read(FXOS8700CQ_Address address, uint8_t *data, int length = 1);
        int write(FXOS8700CQ_Address address, uint8_t *data, int length = 1);
};



#endif