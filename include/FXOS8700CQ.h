#ifndef _FXOS8700CQ_Driver_
#define _FXOS8700CQ_Driver_

#include "mbed.h"
#include "FXOS8700CQ_enum.h"


class FXOS8700CQ{

    public:
        FXOS8700CQ(FXO::Mode mode = FXO::ACCELEROMETER, FXO::Range range = FXO::RANGE2000, FXO::ODR awakeODR = FXO::ODR400, FXO::ODR asleepODR = FXO::ODR400);

        static const float TEMPERATURE_SENSITIVITY;
        static const float MAGNETIC_SENSITIVITY;
        static const float BASE_ACC_SENSITIVITY;
        
        typedef struct{
            FXO::Axis axis;
            float value;
        } mail_t;
        Mail<mail_t, 135> mailBox;

        void setMode(FXO::Mode mode);
        void standby();
        void setAccelerometer();
        void setMagnetometer();
        void setHybrid();
        FXO::Mode getStatus();
        bool isDataAvailable();
        void softReset();
        void hardReset();

        void setRange(FXO::Range range);
        void setLowPass(FXO::Low threshold);
        void setHighPass(FXO::High threshold);
        void setAwakeODR(FXO::ODR dataRate);
        void setAsleepODR(FXO::ODR dataRate);
        bool setAccLowNoise(bool activated=true);
        void setMagOversampleAwake(FXO::Mag_OSR oversample);
        void setMagOversampleAsleep(FXO::Mag_OSR oversample);
        void setAccOversampleAwake(FXO::Acc_OSR oversample);
        void setAccOversampleAsleep(FXO::Acc_OSR oversample);
        void setNewData(FXO::Interrupt_Pin pin, void (*function)());
        void setAccelerationMagnitude(FXO::Interrupt_Pin pin, void (*function)(), uint8_t count, bool resetCount, uint8_t config, float threshold, float* reference);
        void setFreefallMotion(FXO::Interrupt_Pin pin, void (*function)(), float count, bool resetCount, uint8_t config, float threshold, float xThreshold=0, float yThreshold=0, float zThreshold=0);
        void setPulse(FXO::Interrupt_Pin pin, void (*function)(), uint16_t config, float timing, float* threshold, float latency, float window);
        void setOrientation(FXO::Interrupt_Pin pin, void (*function)(), float count, bool resetCount, FXO::Lockout_Angle lockout, FXO::Orientation_Angle angle, FXO::Trip_Threshold threshold, FXO::Trip_Delta delta);
        void setTransient(FXO::Interrupt_Pin pin, void (*function)(), float count, bool resetCount, uint8_t config, float threshold);
        void setFIFO(FXO::Interrupt_Pin pin, void (*function)(), FXO::FIFO_Mode mode, uint8_t watermark = 0, uint8_t config = 0);
        void setSleepWake(FXO::Interrupt_Pin pin, void (*function)(), float count, bool resetCount, uint8_t interrupts);
        void setMagneticThreshold(FXO::Interrupt_Pin pin, void (*function)(), float count, bool resetCount, uint8_t config, float *threshold);
        void setMagneticMagnitude(FXO::Interrupt_Pin pin, void (*function)(), uint8_t count, bool resetCount, uint8_t config, float threshold, float* reference);

        void setAccelerationOffset(float* offset);
        void setMagneticOffset(float* offset);

        float* getAcceleration();
        float* getMagnetic();
        float* getAllData();
        float* getMaxMagnetic();
        float* getMinMagnetic();
        float getTemperature();
        void removeInterrupt(FXO::Interrupt name);
        

    private:
        I2C mI2C;
        uint8_t mAddress;
        uint16_t activeInterrupts;
        FXO::Range mRange;
        bool awake;
        FXO::ODR mODR;
        FXO::ODR mSleepODR;
        FXO::ODR mAwakeODR;
        FXO::Mode mMode;
        InterruptIn mInterruptOne;
        InterruptIn mInterruptTwo;
        DigitalOut mReset;
        Thread mThread;
        void interruptWrapper();
        void (*mInterrupts[10])();

        uint8_t* getRawAcceleration();
        uint8_t* getRawMagnetic();
        uint8_t* getAllRawData();
        
        void dispatchInterruptData();
        void setInterrupt(FXO::Interrupt_Pin pin, FXO::Interrupt name, void (*function)());
        void setInterruptFunction(void (*function)(), FXO::Interrupt name);
        FXO::Interrupt identifyInterrupt();

        float convertFIFOAcceleration(uint8_t *rawAcc);
        float convertAcceleration(uint8_t *rawAcc);
        float convertMagnetic(uint8_t *rawMag);

        void read(FXO::Address address, uint8_t *data, int length = 1);
        int write(FXO::Address address, uint8_t *data, int length = 1);
};



#endif