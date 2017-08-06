#include "mbed.h"
#include "MPL3115A2.h"

int main(){
    MPL3115A2 sensor;
    Serial pc(USBTX, USBRX);
    double pressure;
    while(1){
        pressure = sensor.getData();
        pc.baud(19200);
        pc.printf("Pressure = %f\n", pressure);
        Thread::wait(500);
    }
}