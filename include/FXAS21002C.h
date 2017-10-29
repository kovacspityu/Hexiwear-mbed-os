#ifndef _FXAS21002C_Driver_
#define _FXAS21002C_Driver_

#include "mbed.h"
#include "FXAS21002C_enum.h"

class FXAS21002C{

    public:
        FXAS21002C(FXAS21002C_Range range = FXA_RANGE2000, FXAS21002C_ODR dataRate = FXA_ODR100);

        typedef struct{
            FXAS21002C_Axis axis;
            float value;
        } mail_t;
        Mail<mail_t, 24> mailBox;

        void standby();
        void setActive();
        void setReady();
        FXAS21002C_Mode getStatus();
        bool isDataAvailable();

        void setRange(FXAS21002C_Range range);
        void setLowPass(FXAS21002C_Low threshold);
        void setHighPass(FXAS21002C_High threshold);
        void setODR(FXAS21002C_ODR dataRate);

        float* getAngles();
        float* getRadians();
        int8_t* getTemperature();
        void setInterrupt(FXAS21002C_Interrupt_Pin pin, FXAS21002C_Interrupt name, void (*function)(), float threshold = 0, int count = 0, bool resetCount = false);
        void removeInterrupt(FXAS21002C_Interrupt name);
        

    private:
        I2C mI2C;
        uint8_t mAddress;
        uint8_t activeInterruptsOne;
        uint8_t activeInterruptsTwo;
        float mSensitivity;
        float mODR;
        InterruptIn mInterruptOne;
        InterruptIn mInterruptTwo;
        Thread mThreadOne;
        Thread mThreadTwo;
        void interruptWrapper(FXAS21002C_Interrupt_Pin pin);
        void interruptWrapperOne();
        void interruptWrapperTwo();
        void (*FXAS21002CInterruptOne)();
        void (*FXAS21002CInterruptTwo)();

        int16_t* getRawData();
        void dispatchInterruptData(FXAS21002C_Interrupt_Pin pin);
        void dispatchInterruptDataOne();
        void dispatchInterruptDataTwo();
        void setInterruptFunction(void (*function)(), FXAS21002C_Interrupt_Pin pin);
        FXAS21002C_Interrupt identifyInterrupt(FXAS21002C_Interrupt_Pin pin);

        float convertToAngle(int16_t rawAngle);
        float convertToRadian(int16_t rawAngle);

        void read(FXAS21002C_Address address, uint8_t *data, int length = 1);
        int write(FXAS21002C_Address address, uint8_t *data, int length = 1);

};



#endif