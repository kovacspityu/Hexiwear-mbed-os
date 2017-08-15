#include "mbed.h"
#include "MPL3115A2.h"
#include "TSL2561.h"

Serial pc(USBTX, USBRX);
Mutex stdio_mutex;
MPL3115A2 sensorM;
TSL2561 sensorL;

void interruptDummyM(){
    while(true){
        Thread::signal_wait(0x01);
        osEvent evt = sensorM.mailBox.get();
        if (evt.status == osEventMail) {
            MPL3115A2::mail_t *mail = (MPL3115A2::mail_t*)evt.value.p;
            stdio_mutex.lock();
            switch(mail->type){
                case TYPE_ALTITUDE : pc.printf("Current altitude = %f m\n", mail->value);
                case TYPE_PRESSURE : pc.printf("Current pressure = %f Pa\n", mail->value);
                case TYPE_TEMPERATURE: pc.printf("Current temperature = %f°C\n", mail->value);
            }
            stdio_mutex.unlock();
            sensorM.mailBox.free(mail);
        }
    }
}

void interruptDummyL(){
    while(true){
        Thread::signal_wait(0x01);
        osEvent evt = sensorL.mailBox.get();
        if (evt.status == osEventMail) {
            double *mail = (double*)evt.value.p;
            stdio_mutex.lock();
            pc.printf("Current Light = %f Lux\n", *mail);
            stdio_mutex.unlock();
            sensorL.mailBox.free(mail);
        }
    }
}



int main(){
    pc.baud(19200);
    sensorM.setInterrupt(PIN_ONE, I_NEW_DATA, &interruptDummyM);
    sensorL.setInterrupt(80, 120, ONE_CYCLE, &interruptDummyL);
    wait(2);
    double barometer = sensorM.getData();
    pc.printf("Current pressure, directly requested, is %f Pa\n", barometer);
    while(1){    
        wait(5);
    }
}