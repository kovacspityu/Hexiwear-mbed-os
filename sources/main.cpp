#include "mbed.h"
#include "MPL3115A2.h"
#include "TSL2561.h"
#include "HTU21D.h"
#include "FXAS21002C.h"
#include "MAX30101.h"
#include "SSD1351.h"
#include "FXOS8700CQ.h"
#include "FXAS21002C.h"
#include "gAssets.h"
#include <queue>
#include <string>

#define FFT_SAMPLE_SIZE 64

//Ticker ticker;
//Thread pulseUpdaterThread, dumpThread;
//EventQueue dumpQueue, pulseUpdaterQueue;
Serial pc(USBTX, USBRX);
Mutex stdio_mutex;
MPL3115A2 sensorM(MPL::BAROMETER, MPL::OS_512_MS, MPL::TS_4S);
//TSL2561 sensorL;
HTU21D sensorH;
MAX30101 sensorHR(MAX::HR_MODE, MAX::OVERSAMPLE_4);
//FXAS21002C sensorO(FXA::RANGE2000);
//FXOS8700CQ sensorAM(FXO::ACCELEROMETER);
SSD1351 screen;
//InterruptIn button1(BUTTON1);
//DigitalOut redLed(LED_RED, 1);
//DigitalOut greenLed(LED_GREEN, 1);
//DigitalOut blueLed(LED_BLUE, 1);
uint32_t *bufferHRRed = new uint32_t[FFT_SAMPLE_SIZE];
uint32_t *bufferHRGreen = new uint32_t[FFT_SAMPLE_SIZE];
uint32_t *bufferHRIR = new uint32_t[FFT_SAMPLE_SIZE];
MAX30101::uint8AndFloat hrRed, hrGreen, hrIR;
uint32_t sampleNumber = 0;
//queue<MAX30101::uint8AndFloat> logQueue;
uint8_t pulseAmplitude = 1;
//char* mag = "Current Magnetic Value for Axis ";
//char* acc = "Current Acceleration Value for Axis ";
SSD1351::TextProperties_t liminalProperties = SSD1351::TextProperties_t(LIMINAL7, SSD1351::getColour(120, 20, 30));
SSD1351::TextProperties_t robotoProperties = SSD1351::TextProperties_t(ROBOTO15, SSD1351::getColour(255, 100, 0));
SSD1351::TextProperties_t robotoGreenProperties = SSD1351::TextProperties_t(ROBOTO11, SSD1351::getColour(10, 10, 10));        



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
*/
void interruptDummyM(){
    while(1){
        osEvent evt = sensorM.mailBox.get(10);
        if (evt.status == osEventMail){
            MPL3115A2::mail_t *mail = (MPL3115A2::mail_t*)evt.value.p;
            stdio_mutex.lock();
            switch(mail->type){
                case MPL::TYPE_ALTITUDE : {
                    pc.printf("Current altitude = %f m\n", mail->value);
                    break;
                }
                case MPL::TYPE_PRESSURE : {
                    pc.printf("Current pressure = %f Pa\n", mail->value);
                    char text[20];
                    sprintf(text, "%.1f Pa", mail->value);
                    screen.fillArea(23, 10, 47, 10, 0, true);
                    screen.fillArea(71, 7, 16, 16, 0, true);
                    screen.addTextInBox(text, 20, liminalProperties, 23, 10, 47, 10, SSD1351::getColour(200, 200, 0), 0, true);
                    if(mail->value>101100){
                        screen.addImage(gfx::sunny + 2, 71, 7, gfx::sunny[0], gfx::sunny[1], true);
                    }
                    else if(mail->value>101000){
                        screen.addImage(gfx::cloudy + 2, 72, 8, gfx::cloudy[0], gfx::cloudy[1], true);
                    }
                    else if(mail->value>100800){
                        screen.addImage(gfx::rain + 2, 72, 8, gfx::rain[0], gfx::rain[1], true);
                    }
                    else{
                        screen.addImage(gfx::storm + 2, 72, 8, gfx::storm[0], gfx::storm[1], true);
                    }
                    break;
                }
                case MPL::TYPE_TEMPERATURE: {
                    pc.printf("Current temperature = %f°C\n", mail->value);
                    break;
                }
            }
            stdio_mutex.unlock();
            sensorM.mailBox.free(mail);
        }
        else{return;}
    }
}
/*
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
*/
void interruptDummyHR(){
    while(1){    
        osEvent evt = sensorHR.mailBox.get(10);
        if (evt.status == osEventMail){
            MAX30101::mail_t *mail = (MAX30101::mail_t*)evt.value.p;
            bufferHRRed[sampleNumber] = mail->ledSamples[0].value;
            //bufferHRGreen[sampleNumber] = mail->ledSamples[1].value;
            //bufferHRIR[sampleNumber] = mail->ledSamples[2].value;
            sampleNumber++;
            if(sampleNumber%32==0){
                sensorH.setAuxTemperature(mail->temperature);
            }
            if(sampleNumber==FFT_SAMPLE_SIZE){
                hrRed = sensorHR.getHR(bufferHRRed, FFT_SAMPLE_SIZE);
                //hrGreen = sensorHR.getHR(bufferHRGreen, FFT_SAMPLE_SIZE);
                //hrIR = sensorHR.getHR(bufferHRIR, FFT_SAMPLE_SIZE);
                //pc.printf("%u\t\t\t%u\t\t%1.2e\t%u\t\t%1.2e\t%u\t\t%1.2e\n", pulseAmplitude, hrRed.hr, hrRed.weight, hrGreen.hr, hrGreen.weight, hrIR.hr, hrIR.weight);
                sampleNumber = 0;
                char text[4];
                sprintf(text, "%u", hrRed.hr);
                screen.fillArea(35, 28, 20, 20, 0, true);
                screen.addText(43, 36, text, 4, true, robotoGreenProperties);
                screen.addImageAtBottom(gfx::heart + 2, 35, 28, gfx::heart[0], gfx::heart[1]);
                //logUpdater();
            }
            delete[] mail->ledSamples;
            sensorHR.mailBox.free(mail);
        }
        else{return;}
    }
}


/*void pulseDummyAM(){
    while(1){    
        osEvent evt = sensorAM.mailBox.get(10);
        if (evt.status == osEventMail){
            FXOS8700CQ::mail_t *mail = (FXOS8700CQ::mail_t*)evt.value.p;
            stdio_mutex.lock();
            switch(mail->axis){
                case 4: {
                    screen.fillArea(0,0, SCREEN_SIZE, SCREEN_SIZE, SSD1351::getColour(255, 0, 0), true);
                    break;
                }
                case 2: {
                    screen.fillArea(0,0, SCREEN_SIZE, SCREEN_SIZE, SSD1351::getColour(0, 255, 0), true);
                    break;
                }
                case 1: {
                    screen.fillArea(0,0, SCREEN_SIZE, SCREEN_SIZE, SSD1351::getColour(0, 0, 255), true);
                    break;
                }
                default: break;
            }
            sensorAM.mailBox.free(mail);
            screen.draw();
            stdio_mutex.unlock();
        }
        else{return;}
    }    
}

void FIFOdummyAM(){
    while(1){
        osEvent evt = sensorAM.mailBox.get(10);
        if(evt.status == osEventMail){
            FXOS8700CQ::mail_t *mail =(FXOS8700CQ::mail_t*) evt.value.p;
            stdio_mutex.lock();
            if(mail->axis>4){
                printf("%s%u\t= %.1f\n", mag, 2 - ((mail->axis)>>4), mail->value);
            }
            else{
                printf("%s%u\t= %.6f\n", acc, 2 - ((mail->axis)>>1), mail->value);
            }
            stdio_mutex.unlock();
            sensorAM.mailBox.free(mail);
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
    sensorM.setInterrupt(MPL::PIN_ONE, MPL::I_NEW_DATA, &interruptDummyM);
    //sensorL.setInterrupt(80, 120, TSL_ONE_CYCLE, &interruptDummyL);
    //sensorHR.setPulseAmplitude(0x01, 0x01, 0x01, 0x01);
    //sensorHR.setMultiLedTiming(MAX::LED_RED, MAX::LED_GREEN, MAX::LED_IR, MAX::LED_NONE);
    sensorHR.setPulseAmplitude(200, 0);
    sensorHR.setPulseWidth(MAX::PULSE_WDT_69);
    sensorHR.setProximityDelay(0);
    sensorHR.setInterrupt(MAX::I_FIFO_FULL, &interruptDummyHR, 0x0F);
    //uint8_t config = FXO::ENABLE_LATENCY | FXO:: ENABLE_DOUBLE_Z | FXO:: ENABLE_DOUBLE_Y | FXO:: ENABLE_DOUBLE_X; 
    //float timing = 100;
    //float latency = 30;
    //float window = 200;
    //float threshold[] = {2000, 2000, 2000};
    //sensorAM.setPulse(FXO::PIN_ONE, &pulseDummyAM, config, timing, threshold, latency, window);
    //sensorAM.setNewData(FXO::PIN_ONE, &FIFOdummyAM);
    //sensorAM.setFIFO(FXO::PIN_ONE, &FIFOdummyAM, FXO::NO_OVERFLOW);
    //pc.printf("Pulse Amplitude\t\tRed HR\t\tRed Weight\tGreen HR\tGreen Weight\tInfrared HR\tIR Weight\n");
    float temperature=0;
    //float pressure=0;
    //float light;
    float humidity=0;
    //float *acceleration;
    //uint8_t heartRate=0;
    screen.addImageOnTop(gfx::thermometer + 2, 47, 60, gfx::thermometer[0], gfx::thermometer[1]);
    screen.addImageOnTop(gfx::humidity + 2, 5, 62, gfx::humidity[0], gfx::humidity[1]);
    screen.addImageOnTop(gfx::heart + 2, 35, 28, gfx::heart[0], gfx::heart[1]);
    while(1){
        //for(uint8_t i=0;i<16; i++){
        //    screen.fillArea(0, 0, SCREEN_SIZE - 1, SCREEN_SIZE - 1, 1<<i, true);
        //    SSD1351::TextProperties_t prop = SSD1351::TextProperties_t(ROBOTO11, ~(1<<i));
        //    char text[17]="0000000000000000";
        //    text[15-i] = '1';
        //    screen.addText(15, 40, text, 17, true, prop);
        //    screen.draw(true);
        //    Thread::wait(500);
        //}
        //SSD1351::TextProperties_t properties = SSD1351::TextProperties_t(LIMINAL7, SSD1351::getColour(120, 20, 30));
        //char* text = "After this statement the control is transferred to the statement immediately following the enclosing loop or switch. As with any block exit, all automatic storage objects declared in enclosing compound statement or in the condition of a loop/switch are destroyed, in reverse order of construction, before the execution of the first line following the enclosing loop.";
        //char* text2 = "Tanto va la gatta al lardo che ci lascia lo zampino.";
        //screen.addText(0, 0, text, 367, true, properties);
        //screen.draw();
        //Thread::wait(3000);
        //screen.addTextInRoundedBox(text, 367, properties, 10, 5, 70, 45, 0, SSD1351::getColour(120, 20, 30), 0, true, SSD1351::getColour(0, 80, 12), 1);
        //screen.draw();
        //Thread::wait(3000);
        //screen.addRoundedBox(20, 20, 50, 50, 1, SSD1351::getColour(0, 255, 0), 2, true, SSD1351::getColour(255, 0, 0), 2);
        //screen.draw();
        //Thread::wait(3000);
        //for(int16_t i=0;i<360;i+=5){
        //    screen.addLine(50, 50, SCREEN_SIZE, i, i*300 + 5, 1, true);
        //    screen.draw();
        //}
        //for(int16_t i=0;i<360;i+=5){
        //    screen.addLine(SCREEN_SIZE + 20, 50, SCREEN_SIZE, i, SSD1351::getColour(255, 0, 0), 1, true);
        //    screen.draw();
        //}
        //screen.addCircle(50, 50, 15, SSD1351::getColour(255, 0, 0), 1, true, SSD1351::getColour(120, 120, 0), 1);
        //screen.draw();
        //Thread::wait(3000);
        //for(uint8_t i=15;i<41;i++){
        //    for(uint8_t j=0;j<4;j++){
        //        screen.addCircle(47, 47, i, SSD1351::getColour(255, 100, 0), j, true);
        //        screen.draw();
        //        Thread::wait(400);
        //    }
        //}
        //screen.addCircle(47, 47, 35, SSD1351::getColour(255, 100, 0), 2, true);
        //screen.draw(true);
        //Thread::wait(1000);
        //screen.addFilledCircle(47, 47, 35, SSD1351::getColour(255, 100, 0), 2, true, SSD1351::getColour(100, 255, 0));
        //screen.draw();
        //Thread::wait(3000);
        //light = sensorL.getLux();
        humidity = sensorH.waitForHumidity();
        temperature = sensorH.waitForTemperature();
        stdio_mutex.lock();
        pc.printf("Current humidity is %f %\n", humidity);
        pc.printf("Current temperature is %fC\n", temperature);
        char text[10];
        sprintf(text, "%u%%", (uint8_t) lround(humidity));
        screen.fillArea(15, 60, 25, 17, 0, true);
        screen.addText(15, 60, text, 10, true, robotoProperties);
        sprintf(text, "%.1f C", temperature);
        screen.fillArea(55, 60, 40, 17, 0, true);
        screen.addText(55, 60, text, 10, true, robotoProperties);            
        stdio_mutex.unlock();
        screen.draw(true);
        Thread::wait(1000);
        /*wait(1);
        uint8_t fifoWrite, fifoRead;
        uint8_t *data =  new uint8_t[2];
        sensorHR.read(MAX_FIFO_WRITE_PTR, &fifoWrite);
        sensorHR.read(MAX_FIFO_READ_PTR, &fifoRead);
        sensorHR.read(MAX_INTERRUPT_STATUS, data, 2);
        uint32_t sampleNumber = (32 + fifoWrite - fifoRead) % 32;
        delete[] data;
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
        }
        */
    }
}