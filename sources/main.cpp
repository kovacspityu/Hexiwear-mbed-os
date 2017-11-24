#include "mbed.h"
#include "MPL3115A2.h"
#include "TSL2561.h"
#include "HTU21D.h"
#include "FXAS21002C.h"
#include "MAX30101.h"

#define FFT_SAMPLE_SIZE 512

Serial pc(USBTX, USBRX);
Mutex stdio_mutex;
MPL3115A2 sensorM(ALTIMETER);
TSL2561 sensorL;
HTU21D sensorH;
MAX30101 sensorHR(MAX_MULTI_MODE);
uint32_t *bufferHR = new uint32_t[FFT_SAMPLE_SIZE];
uint32_t sampleNumber = 0;

void interruptDummyM(){
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
}

void interruptDummyHR(){
    while(1){    
        osEvent evt = sensorHR.mailBox.get(10);
        if (evt.status == osEventMail){
            MAX30101::mail_t *mail = (MAX30101::mail_t*)evt.value.p;
            stdio_mutex.lock();
            for(uint i=0;i<mail->length && i<FFT_SAMPLE_SIZE-sampleNumber;i++){
                //pc.printf("\nPeaks = %u, number of samples collected thus far = %u", mail->ledSamples[i].value, sampleNumber+i+1);
                bufferHR[sampleNumber+i] = mail->ledSamples[i].value;
            }
            sampleNumber+=mail->length;
            if(sampleNumber>=FFT_SAMPLE_SIZE){
                uint8_t rawHR = sensorHR.getHR(bufferHR, FFT_SAMPLE_SIZE);
                pc.printf("\nHeart rate = %u", rawHR);
                for(uint i= sampleNumber-FFT_SAMPLE_SIZE;i<mail->length;i++){
                    //pc.printf("\nPeaks = %u, number of samples collected thus far = %u", mail->ledSamples[i].value, i-FFT_SAMPLE_SIZE+sampleNumber+1);
                    bufferHR[i+FFT_SAMPLE_SIZE-sampleNumber] = mail->ledSamples[i].value;
                }
                sampleNumber = mail->length + sampleNumber - FFT_SAMPLE_SIZE;//TODO sampleNumber needs to be equal to the number of samples added during the second for cycle, not 0
            }
            stdio_mutex.unlock();
            delete[] mail->ledSamples;
            mail->ledSamples = 0;
            sensorHR.mailBox.free(mail);
        }
        else{return;}
    }
}




int main(){
    pc.baud(19200);
    //sensorM.setInterrupt(PIN_ONE, I_FIFO, &interruptDummyM, true);
    //sensorL.setInterrupt(80, 120, ONE_CYCLE, &interruptDummyL);
    sensorHR.setPulseAmplitude(0xCF, 0, 0xCF, 0);
    sensorHR.setProximityDelay(0);
    sensorHR.setMultiLedTiming(MAX_LED_RED, MAX_LED_GREEN, MAX_LED_NONE, MAX_LED_NONE);
    sensorHR.setInterrupt(I_FIFO_FULL_MAX, &interruptDummyHR, 0x0F);
    //float barometer;
    //float temperature;
    //float light;
    //float humidity;
    while(1){   
        //light = sensorL.getLux();
        //humidity = sensorH.waitForHumidity();
        //temperature = sensorH.waitForTemperature();
        //stdio_mutex.lock();
        //pc.printf("Current humidity is %f %\n", humidity);
        //pc.printf("Current temperature is %fC\n", temperature);
        //stdio_mutex.unlock();
        /*wait(1);
        uint8_t fifoWrite, fifoRead;
        uint8_t *data =  new uint8_t[2];
        sensorHR.read(MAX_FIFO_WRITE_PTR, &fifoWrite);
        sensorHR.read(MAX_FIFO_READ_PTR, &fifoRead);
        sensorHR.read(MAX_INTERRUPT_STATUS, data, 2);
        uint32_t sampleNumber = (32 + fifoWrite - fifoRead) % 32;
        delete[] data;
        /*
        sensorM.setMode(ALTIMETER); 
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