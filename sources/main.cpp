#include "mbed.h"
#include "MPL3115A2.h"
#include "TSL2561.h"
#include "HTU21D.h"
#include "FXAS21002C.h"

Serial pc(USBTX, USBRX);
Mutex stdio_mutex;
//MPL3115A2 sensorM(ALTIMETER);
//TSL2561 sensorL;
HTU21D sensorH;

/*void interruptDummyM(){
    pc.printf("Currently in context %p\n", Thread::gettid());
    for(int i=0;i<32;i++){
        osEvent evt = sensorM.mailBox.get();
        if (evt.status == osEventMail) {
            MPL3115A2::mail_t *mail = (MPL3115A2::mail_t*)evt.value.p;
            stdio_mutex.lock();
            switch(mail->type){
                case TYPE_ALTITUDE : {
                    pc.printf("Current altitude = %f m\n", mail->value);
                    break;
                }
                case TYPE_PRESSURE : {
                    pc.printf("Current pressure = %f Pa\n", mail->value);
                    break;
                }
                case TYPE_TEMPERATURE: {
                    pc.printf("Current temperature = %f°C\n", mail->value);
                    break;
                }
            }
            stdio_mutex.unlock();
            sensorM.mailBox.free(mail);
        }
    }
}

void interruptDummyL(){
        osEvent evt = sensorL.mailBox.get();
        if (evt.status == osEventMail) {
            float *mail = (float*)evt.value.p;
            stdio_mutex.lock();
            pc.printf("Current Light = %f Lux\n", *mail);
            stdio_mutex.unlock();
            sensorL.mailBox.free(mail);
        }
}*/



int main(){
    pc.baud(19200);
    //sensorM.setInterrupt(PIN_ONE, I_FIFO, &interruptDummyM, true);
    //sensorL.setInterrupt(80, 120, ONE_CYCLE, &interruptDummyL);
    float barometer;
    float temperature;
    float light;
    float humidity;
    while(1){   
        //light = sensorL.getLux();
        humidity = sensorH.waitForHumidity();
        temperature = sensorH.waitForTemperature();
        //stdio_mutex.lock();
        pc.printf("Current humidity is %f %\n", humidity);
        pc.printf("Current temperature is %fC\n", temperature);
        //stdio_mutex.unlock();
        wait(3);
        /*sensorM.setMode(ALTIMETER); 
        wait(3);
        if(sensorM.isDataAvailable()){
            barometer = sensorM.getData();
            temperature = sensorM.getTemperature();
            light = sensorL.getLux();
            pc.printf("Current altitude, directly requested, is %f m\nCurrent light, directly requested, is %f Lux\nCurrent temperature, directly requested, is %f C\n",
            barometer, light, temperature);
        }
        
        sensorM.setMode(BAROMETER);
        wait(3);
        if(sensorM.isDataAvailable()){
            barometer = sensorM.getData();
            light = sensorL.getLux();
            temperature = sensorM.getTemperature();
            pc.printf("Current pressure, directly requested, is %f Pa\nCurrent light, directly requested, is %f Lux\nCurrent temperature, directly requested, is %f C\n",
            barometer, light, temperature);
        }*/
    }
}