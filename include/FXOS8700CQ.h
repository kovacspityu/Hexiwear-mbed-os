#ifndef _FXOS8700CQ_Driver_
#define _FXOS8700CQ_Driver_

#include "mbed.h"
#include "FXOS8700CQ_enum.h"

class FXOS8700CQ{

    public:
        FXOS8700CQ(FXOS8700CQ_Range range = FXO_RANGE2000, FXOS8700CQ_ODR dataRate = FXO_ODR100);

        typedef struct{
            FXOS8700CQ_Axis axis;
            float value;
        } mail_t;
        Mail<mail_t, 24> mailBox;

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

        float* getAngles();
        float* getRadians();
        int8_t* getTemperature();
        void setInterrupt(FXOS8700CQ_Interrupt_Pin pin, FXOS8700CQ_Interrupt name, void (*function)(), float threshold = 0, int count = 0, bool resetCount = false);
        void removeInterrupt(FXOS8700CQ_Interrupt name);
        

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
        void interruptWrapper(FXOS8700CQ_Interrupt_Pin pin);
        void interruptWrapperOne();
        void interruptWrapperTwo();
        void (*FXOS8700CQInterruptOne)();
        void (*FXOS8700CQInterruptTwo)();

        int16_t* getRawData();
        void dispatchInterruptData(FXOS8700CQ_Interrupt_Pin pin);
        void dispatchInterruptDataOne();
        void dispatchInterruptDataTwo();
        void setInterruptFunction(void (*function)(), FXOS8700CQ_Interrupt_Pin pin);
        FXOS8700CQ_Interrupt identifyInterrupt(FXOS8700CQ_Interrupt_Pin pin);

        float convertToAngle(int16_t rawAngle);
        float convertToRadian(int16_t rawAngle);

        void read(FXOS8700CQ_Address address, uint8_t *data, int length = 1);
        int write(FXOS8700CQ_Address address, uint8_t *data, int length = 1);

};



#endif