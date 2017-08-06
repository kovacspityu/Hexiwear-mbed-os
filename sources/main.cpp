#include "mbeh.h"
#include "MPL3115A2.h"

int main(){
    MPL3115A2 sensor();
    Serial pc(USBTX, USBRX);
    double pressure;
    while(1){
        pressure = sensor.getData();
        pc.baud(19200);
        pc.prinf("Pressure =%f", pressure);
        Thread::wait(250;)
    }
}