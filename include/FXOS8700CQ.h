#ifndef _FXOS21002C_Driver_
#define _FXOS21002C_Driver_

#include "mbed.h"
#include "FXOS8700CQ_enum.h"

class FXOS21002C{

    public:
        FXOS21002C(FXOS21002C_Range range = FXO_RANGE2000, FXOS21002C_ODR dataRate = FXO_ODR100);

        typedef struct{
            FXOS21002C_Axis axis;
            float value;
        } mail_t;
        Mail<mail_t, 24> mailBox;

        void standby();
        void setActive();
        void setReady();
        FXOS21002C_Mode getStatus();
        bool isDataAvailable();

        void setRange(FXOS21002C_Range range);
        void setLowPass(FXOS21002C_Low threshold);
        void setHighPass(FXOS21002C_High threshold);
        void setODR(FXOS21002C_ODR dataRate);

        float* getAngles();
        float* getRadians();
        int8_t* getTemperature();
        void setInterrupt(FXOS21002C_Interrupt_Pin pin, FXOS21002C_Interrupt name, void (*function)(), float threshold = 0, int count = 0, bool resetCount = false);
        void removeInterrupt(FXOS21002C_Interrupt name);
        

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
        void interruptWrapper(FXOS21002C_Interrupt_Pin pin);
        void interruptWrapperOne();
        void interruptWrapperTwo();
        void (*FXOS21002CInterruptOne)();
        void (*FXOS21002CInterruptTwo)();

        int16_t* getRawData();
        void dispatchInterruptData(FXOS21002C_Interrupt_Pin pin);
        void dispatchInterruptDataOne();
        void dispatchInterruptDataTwo();
        void setInterruptFunction(void (*function)(), FXOS21002C_Interrupt_Pin pin);
        FXOS21002C_Interrupt identifyInterrupt(FXOS21002C_Interrupt_Pin pin);

        float convertToAngle(int16_t rawAngle);
        float convertToRadian(int16_t rawAngle);

        void read(FXOS21002C_Address address, uint8_t *data, int length = 1);
        int write(FXOS21002C_Address address, uint8_t *data, int length = 1);

};



#endif