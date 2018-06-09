#ifndef _FXAS21002C_Driver_
#define _FXAS21002C_Driver_

#include "mbed.h"
#include "FXAS21002C_enum.h"


class FXAS21002C{

    public:
        FXAS21002C(FXA::Range range = FXA::RANGE2000, FXA::ODR dataRate = FXA::ODR100);

        typedef struct{
            FXA::Axis axis;
            float value;
        } mail_t;
        Mail<mail_t, 24> mailBox;

        void setMode(FXA::Mode mode);
        void standby();
        void setActive();
        void setReady();
        FXA::Mode getStatus();
        bool isDataAvailable();

        void setRange(FXA::Range range);
        void setLowPass(FXA::Low threshold);
        void setHighPass(FXA::High threshold);
        void setODR(FXA::ODR dataRate);

        float* getAngles();
        float* getRadians();
        int8_t* getTemperature();
        void setInterrupt(FXA::Interrupt_Pin pin, FXA::Interrupt name, void (*function)(), uint8_t count=0, bool resetCount=false, float threshold=0, uint8_t config=0);
        void removeInterrupt(FXA::Interrupt name);
        

    private:
        I2C mI2C;
        uint8_t mAddress;
        static uint8_t activeInterrupts;
        FXA::Mode mMode;
        float mSensitivity;
        float mODR;
        InterruptIn mInterruptOne;
        InterruptIn mInterruptTwo;
        Thread mThread;
        void interruptWrapper();
        void (*FXAS21002CInterrupt)();

        int16_t* getRawData();
        void dispatchInterruptData();
        void setInterruptFunction(void (*function)());
        FXA::Interrupt identifyInterrupt();

        float convertToAngle(int16_t rawAngle);
        float convertToRadian(int16_t rawAngle);

        void read(FXA::Address address, uint8_t *data, int length = 1);
        int write(FXA::Address address, uint8_t *data, int length = 1);

};


#endif