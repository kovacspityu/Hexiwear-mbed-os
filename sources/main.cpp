#include "mbed.h"
#include "MPL3115A2.h"
#include "TSL2561.h"
#include "HTU21D.h"
#include "FXAS21002C.h"
#include "MAX30101.h"
#include "SSD1351.h"
#include <queue>
#include <string>

#define FFT_SAMPLE_SIZE 512

//Ticker ticker;
//Thread pulseUpdaterThread, dumpThread;
//EventQueue dumpQueue, pulseUpdaterQueue;
Serial pc(USBTX, USBRX);
//Mutex stdio_mutex;
//MPL3115A2 sensorM(MPL_ALTIMETER);
//TSL2561 sensorL;
//HTU21D sensorH;
//MAX30101 sensorHR(MAX_MULTI_MODE);
SSD1351 screen;
//InterruptIn button1(BUTTON1);
//DigitalOut redLed(LED_RED, 1);
//DigitalOut greenLed(LED_GREEN, 1);
//DigitalOut blueLed(LED_BLUE, 1);
//uint32_t *bufferHRRed = new uint32_t[FFT_SAMPLE_SIZE];
//uint32_t *bufferHRGreen = new uint32_t[FFT_SAMPLE_SIZE];
//uint32_t *bufferHRIR = new uint32_t[FFT_SAMPLE_SIZE];
//MAX30101::uint8AndFloat hrRed, hrGreen, hrIR;
//uint32_t sampleNumber = 0;
//queue<MAX30101::uint8AndFloat> logQueue;
//uint8_t pulseAmplitude = 1;



/*void logDumper(){
        stdio_mutex.lock();
        pc.printf("\n");
        int i=0;
        while(logQueue.size()){
            MAX30101::uint8AndFloat dummy = logQueue.front();
            pc.printf("%u\t\t", dummy.hr);
            if(dummy.weight>=0){
            pc.printf("%1.2e\t", dummy.weight);
            }
            else{pc.printf("\t");}
            logQueue.pop();
            i++;
            if(i%4==0){
                pc.printf("\n");
                i = 0;
            }
        }
        pc.printf("\n");
        stdio_mutex.unlock();
}

void pulseAmplitudeUpdater(){
    while(1){  
        pulseAmplitude++;
        sensorHR.setPulseAmplitude(pulseAmplitude, pulseAmplitude, pulseAmplitude, pulseAmplitude);
        if(pulseAmplitude==0xFF){
            greenLed=0;
            pulseAmplitude=0;
        }
    }
}

void logUpdater(){
    MAX30101::uint8AndFloat dummy;
    dummy.hr=pulseAmplitude;
    dummy.weight=-1;
    logQueue.push(dummy);
    logQueue.push(hrRed);
    logQueue.push(hrGreen);
    logQueue.push(hrIR);
}

void interruptDummyM(){
    pc.printf("Currently in context %p\n", Thread::gettid());
    for(int i=0;i<32;i++){
        osEvent evt = sensorM.mailBox.get();
        if (evt.status == osEventMail) {
            MPL3115A2::mail_t *mail = (MPL3115A2::mail_t*)evt.value.p;
            stdio_mutex.lock();
            switch(mail->type){
                case MPL_TYPE_ALTITUDE : {
                    pc.printf("Current altitude = %f m\n", mail->value);
                    break;
                }
                case MPL_TYPE_PRESSURE : {
                    pc.printf("Current pressure = %f Pa\n", mail->value);
                    break;
                }
                case MPL_TYPE_TEMPERATURE: {
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
            bufferHRRed[sampleNumber] = mail->ledSamples[0].value;
            bufferHRGreen[sampleNumber] = mail->ledSamples[1].value;
            bufferHRIR[sampleNumber] = mail->ledSamples[2].value;
            sampleNumber++;
            if(sampleNumber==FFT_SAMPLE_SIZE){
                hrRed = sensorHR.getHR(bufferHRRed, FFT_SAMPLE_SIZE);
                hrGreen = sensorHR.getHR(bufferHRGreen, FFT_SAMPLE_SIZE);
                hrIR = sensorHR.getHR(bufferHRIR, FFT_SAMPLE_SIZE);
                pc.printf("%u\t\t\t%u\t\t%1.2e\t%u\t\t%1.2e\t%u\t\t%1.2e\n", pulseAmplitude, hrRed.hr, hrRed.weight, hrGreen.hr, hrGreen.weight, hrIR.hr, hrIR.weight);
                sampleNumber = 0;
                logUpdater();
            }
            delete[] mail->ledSamples;
            mail->ledSamples = 0;
            sensorHR.mailBox.free(mail);
        }
        else{return;}
    }
}
*/



int main(){
    pc.baud(19200);
    //dumpThread.start(callback(&dumpQueue, &EventQueue::dispatch_forever));
    //pulseUpdaterThread.start(callback(&pulseUpdaterQueue, &EventQueue::dispatch_forever));
    //button1.rise(dumpQueue.event(logDumper));  //TODO It will need to be attached to the touch buttons of the HexiWear, not of the Docking Station.
    //ticker.attach(pulseUpdaterQueue.event(pulseAmplitudeUpdater), 15.0);
    //sensorM.setInterrupt(MPL_PIN_ONE, I_FIFO_MPL, &interruptDummyM, true);
    //sensorL.setInterrupt(80, 120, TSL_ONE_CYCLE, &interruptDummyL);
    //sensorHR.setPulseAmplitude(0x01, 0x01, 0x01, 0x01);
    //sensorHR.setMultiLedTiming(MAX_LED_RED, MAX_LED_GREEN, MAX_LED_IR, MAX_LED_NONE);
    //sensorHR.setInterrupt(I_FIFO_FULL_MAX, &interruptDummyHR, 0x0F);
    //pc.printf("Pulse Amplitude\t\tRed HR\t\tRed Weight\tGreen HR\tGreen Weight\tInfrared HR\tIR Weight\n");
    //float barometer;
    //float temperature;
    //float light;
    //float humidity;
    while(1){
        screen.addBox(5, 5, 50, 50, SSD1351::getColour(120, 20, 30), 1, true, SSD1351::getColour(0, 80, 12), 1);
        screen.draw();
        Thread::wait(500); 
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
        sensorM.setMode(MPL_ALTIMETER); 
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