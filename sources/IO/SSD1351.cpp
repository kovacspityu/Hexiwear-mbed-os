#include "SSD1351_enum.h"
#include "SSD1351.h"
#include <algorithm>
#include <cmath>

#define X_OFFSET 16
#define Y_OFFSET 0
#define CRITICAL_ANGLE_MIN 25
#define CRITICAL_ANGLE_MAX 65

uint8_t SSD1351::xStartActive = 0;
uint8_t SSD1351::xEndActive = SCREEN_SIZE - 1; 
uint8_t SSD1351::yStartActive = 0; 
uint8_t SSD1351::yEndActive = SCREEN_SIZE - 1;
uint8_t SSD1351::xStartDrawing = SCREEN_SIZE - 1;
uint8_t SSD1351::xEndDrawing = 0; 
uint8_t SSD1351::yStartDrawing = SCREEN_SIZE - 1; 
uint8_t SSD1351::yEndDrawing = 0;


SSD1351::SSD1351() : mSPI(PTB22, NC, PTB21, PTB20), dataORcommand(PTD15), mPower(PTC13), resetPin(PTE6), chipSelect(PTB20){
    mSPI.frequency(8000000);
    mSPI.format(8);
    dataORcommand = 0;
    mPower = 0;
    wait_ms(1);
    resetPin = 0;
    wait_ms(1);
    resetPin = 1;
    wait_ms(1);
    mPower = 1;
    wait(1);
    unlockCommands();
    write(SSD_SLEEP);
    uint8_t *data = new uint8_t[3];
    data[0] = 0xF1;
    write(SSD_FPS);
    write(data);
    data[0] = 0;
    data[1] = SCREEN_SIZE-1;
    write(SSD_ACTIVE_ROWS);
    write(data+1);
    data[0] = 0x60;
    write(SSD_OPTIONS);
    write(data);
    data[0] = X_OFFSET;
    data[1] = SCREEN_SIZE - 1 + X_OFFSET;
    write(SSD_SELECT_HORIZONTAL);
    write(data, 2);
    data[0] = Y_OFFSET;
    data[1] = SCREEN_SIZE - 1 + Y_OFFSET;
    write(SSD_SELECT_VERTICAL);
    write(data, 2); 
    data[0] = 0x5F;
    write(SSD_IMAGE_V_OFFSET);
    write(data);
    data[0] = 0x80;
    write(SSD_DISPLAY_V_OFFSET);
    write(data); 
    setChargePeriods(0x32);
    unlockCommands();
    //data[0] = 0x05;
    //write(SSD_ROW_DEFAULT_V);
    //write(data);
    lockCommands();
    write(SSD_DISPLAY_ON); 
    //setColourBrightness(0x8A, 0x51, 0x8A);
    //setBrightness(0xCF);
    //data[0] = 0xA0;
    //data[1] = 0xB5;
    //data[2] = 0x55;
    //write(SSD_COLUMN_DEFAULT_V);
    //write(data, 3);
    data[0] = 0x01;
    write(SSD_PHASE_3);
    write(data);
    delete[] data;
    write(SSD_WAKE_UP);
    screenBuffer1 = new uint16_t[SCREEN_SIZE * SCREEN_SIZE];
    screenBuffer2 = new uint16_t[SCREEN_SIZE * SCREEN_SIZE];
    activeScreenBuffer = &screenBuffer2;
    drawingScreenBuffer = &screenBuffer1;
    std::fill(screenBuffer1, screenBuffer1 + SCREEN_SIZE*SCREEN_SIZE, 0);
    std::fill(screenBuffer2, screenBuffer2 + SCREEN_SIZE*SCREEN_SIZE, 0);
    mThread.start(callback(this, &SSD1351::workerDraw));
}


SSD1351::~SSD1351(){
    // TODO delete[] whatever we have new[]
    mThread.terminate();
    powerOff();
}

void SSD1351::powerUp(){
    resetPin = 0;
    wait_ms(2);
    resetPin = 1;
    wait_ms(1);
    resetPin = 0;
    wait_ms(2);
    mPower = 1;
    wait_ms(1);
    write(SSD_WAKE_UP);
    wait_ms(200);
}

void SSD1351::powerOff(){
    write(SSD_SLEEP);
    mPower = 0;
    wait_ms(1);
}

void SSD1351::reset(){
    powerOff();
    powerUp();
}

void SSD1351::sleep(){
    uint8_t data = 0;
    write(SSD_SLEEP);
    write(SSD_SLEEP_EXTRA);
    write(&data);
}

void SSD1351::wakeUp(){
    uint8_t data = 1;
    write(&data);
    write(SSD_SLEEP_EXTRA);
    write(SSD_WAKE_UP);
}


void SSD1351::setBrightness(uint8_t brightness){
    brightness%=17;
    write(SSD_BRIGHTNESS);
    write(&brightness);
}

void SSD1351::setColourBrightness(uint8_t red, uint8_t green, uint8_t blue){
    uint8_t *brightness = new uint8_t[3];
    brightness[0] = red;
    brightness[1] = green;
    brightness[2] = blue;
    write(SSD_COLOUR_BRIGTHNESS);
    write(brightness, 3);
    delete[] brightness;
}

void SSD1351::draw(){
    while(mThread.get_state() != Thread::WaitingThreadFlag) ;
    if(*activeScreenBuffer == screenBuffer1){
        activeScreenBuffer  = &screenBuffer2;
        drawingScreenBuffer = &screenBuffer1;
    }
    else{
        activeScreenBuffer  = &screenBuffer1;
        drawingScreenBuffer = &screenBuffer2;
    }
    std::fill(
        *activeScreenBuffer + xStartActive*SCREEN_SIZE + yStartActive, 
        *activeScreenBuffer + xEndActive*SCREEN_SIZE + yEndActive, 
        0);
    xStartDrawing = xStartActive;
    xEndDrawing = xEndActive;
    yStartDrawing = yStartActive;
    yEndDrawing = yEndActive;
    mThread.signal_set(0x01);
    xStartActive = SCREEN_SIZE - 1;
    xEndActive = 0; 
    yStartActive = SCREEN_SIZE - 1; 
    yEndActive = 0;
}

void SSD1351::clearScreen(){
    xStartActive = 0;
    xEndActive = SCREEN_SIZE - 1;
    yStartActive = 0;
    yEndActive = SCREEN_SIZE - 1;
    std::fill(*activeScreenBuffer, *activeScreenBuffer + SCREEN_SIZE*SCREEN_SIZE, 0);
}

SSD_Error SSD1351::addLine(uint8_t xPosition, uint8_t yPosition, uint8_t length, float angle, uint16_t internalColour, uint8_t internalThickness, bool topOrBottom, uint16_t externalColour, uint8_t externalThickness){
    uint8_t coefficient = (abs(sin(angle*M_PI/180))>=0.5) ? 1 : 0;
    SSD_Error error;
    if(fmod(abs(angle), 90)<CRITICAL_ANGLE_MIN || fmod(abs(angle), 90)>CRITICAL_ANGLE_MAX){
        uint8_t i;
        SSD_Error errorDummy = addLineInternalSimple(xPosition, yPosition, length, angle, internalColour, topOrBottom);
        if(errorDummy!=SSD_NO_ERROR){error=errorDummy;}
        for(i=1;i<=internalThickness;i++){
            errorDummy=addLineInternalSimple(xPosition + i*coefficient, yPosition + (1-coefficient)*i, length, angle, internalColour, topOrBottom); 
            if(errorDummy!=SSD_NO_ERROR){error=errorDummy;}
            errorDummy=addLineInternalSimple(xPosition - i*coefficient, yPosition - (1-coefficient)*i, length, angle, internalColour, topOrBottom); 
            if(errorDummy!=SSD_NO_ERROR){error=errorDummy;}
        }
        for(uint j=0;j<externalThickness;j++){
            errorDummy=addLineInternalSimple(xPosition + (i+j)*coefficient, yPosition + (1-coefficient)*(i+j), length, angle, externalColour, topOrBottom); 
            if(errorDummy!=SSD_NO_ERROR){error=errorDummy;}
            errorDummy=addLineInternalSimple(xPosition - (i+j)*coefficient, yPosition - (1-coefficient)*(i+j), length, angle, externalColour, topOrBottom); 
            if(errorDummy!=SSD_NO_ERROR){error=errorDummy;}
        }
    }
    else{
        uint8_t i;
        SSD_Error errorDummy=addLineInternalExact(xPosition, yPosition, length, angle, internalColour, topOrBottom); 
        if(errorDummy!=SSD_NO_ERROR){error=errorDummy;}
        for(i=1;i<=internalThickness;i++){
            SSD_Error errorDummy=addLineInternalExact(xPosition + i*coefficient, yPosition + (1-coefficient)*i, length, angle, internalColour, topOrBottom); 
            if(errorDummy!=SSD_NO_ERROR){error=errorDummy;}
            errorDummy=addLineInternalExact(xPosition - i*coefficient, yPosition - (1-coefficient)*i, length, angle, internalColour, topOrBottom); 
            if(errorDummy!=SSD_NO_ERROR){error=errorDummy;}
        }
        for(uint j=0;j<externalThickness;j++){
            SSD_Error errorDummy=addLineInternalExact(xPosition + (i+j)*coefficient, yPosition + (1-coefficient)*(i+j), length, angle, externalColour, topOrBottom); 
            if(errorDummy!=SSD_NO_ERROR){error=errorDummy;}
            errorDummy=addLineInternalExact(xPosition - (i+j)*coefficient, yPosition - (1-coefficient)*(i+j), length, angle, externalColour, topOrBottom); 
            if(errorDummy!=SSD_NO_ERROR){error=errorDummy;}
        }
    }
    return error;
}

SSD_Error SSD1351::addLineOnTop(uint8_t xPosition, uint8_t yPosition, uint8_t length, float angle, uint16_t internalColour, uint8_t internalThickness, uint16_t externalColour, uint8_t externalThickness){
    return addLine(xPosition, yPosition, length, angle, internalColour, internalThickness, true, externalColour, externalThickness);
}

SSD_Error SSD1351::addLineAtBottom(uint8_t xPosition, uint8_t yPosition, uint8_t length, float angle, uint16_t internalColour, uint8_t internalThickness, uint16_t externalColour, uint8_t externalThickness){
    return addLine(xPosition, yPosition, length, angle, internalColour, internalThickness, false, externalColour, externalThickness);
}


SSD_Error SSD1351::addImage(uint16_t *image, uint8_t xPosition, uint8_t yPosition, uint8_t width, uint8_t height, bool topOrBottom){
    SSD_Error result = boundaryCheck(xPosition, yPosition, width, height);
    if(result){return result;}
    for(uint i=xPosition;i<=xPosition+width;i++){
        for(uint j=yPosition;j<=yPosition+height;j++){
            if((*activeScreenBuffer)[j*SCREEN_SIZE + i]){
                if(topOrBottom){continue;}
            }
            (*activeScreenBuffer)[j*SCREEN_SIZE + i] = image[(j - yPosition)*width + i - xPosition];
        }
    }
    return SSD_NO_ERROR;
}

SSD_Error SSD1351::addImageOnTop(uint16_t *image, uint8_t xPosition, uint8_t yPosition, uint8_t width, uint8_t height){
    return addImage(image, xPosition, yPosition, width, height, true);
}

SSD_Error SSD1351::addImageAtBottom(uint16_t *image, uint8_t xPosition, uint8_t yPosition, uint8_t width, uint8_t height){
    return addImage(image, xPosition, yPosition, width, height, false);
    
}

void SSD1351::scrollingOn(bool direction, uint8_t startingRow, uint8_t numberOfRows, SSD_Scrolling_Period period){
    uint8_t *data = new uint8_t[5];
    if(direction){data[0] = 1;}
    else{data[0] = 64;}
    data[1] = std::min(startingRow, (uint8_t)(SCREEN_SIZE - 1));
    data[2] = std::min(numberOfRows, (uint8_t)(SCREEN_SIZE - data[1]));
    data[3] = 0;
    data[4] = period;
    write(SSD_SCROLLING);
    write(data, 5);
    write(SSD_SCROLLING_START);
    delete[] data;
}

void SSD1351::scrollingOff(){
    write(SSD_SCROLLING_STOP);
}

uint16_t SSD1351::getColour(uint8_t red, uint8_t green, uint8_t blue){
    return blue>>3 | (green>>2)<<5 | (red>>3)<<11;
}


void SSD1351::lock(){
    uint8_t data = 16;
    write(SSD_COMMAND_LOCK);
    write(&data);
}

void SSD1351::unlock(){
    uint8_t data = 12;
    write(SSD_COMMAND_LOCK);
    write(&data);
}

void SSD1351::lockCommands(){
    unlock();
    uint8_t data = 0xB0;
    write(SSD_COMMAND_LOCK);
    write(&data);
}

void SSD1351::unlockCommands(){
    unlock();
    uint8_t data = 0xB1;
    write(SSD_COMMAND_LOCK);
    write(&data);
}

void SSD1351::setChargePeriods(uint8_t periods){
    unlockCommands();
    write(SSD_PHASE_1_AND_2);
    write(&periods);
    lockCommands();
}


void SSD1351::workerDraw(){
    while(1){
        Thread::signal_wait(0x01);
        uint8_t *data = new uint8_t[2];
        data[0] = xStartDrawing + X_OFFSET;
        data[1] = xEndDrawing + X_OFFSET;
        write(SSD_SELECT_HORIZONTAL);
        write(data, 2);
        data[0] = yStartDrawing + Y_OFFSET;
        data[1] = yEndDrawing + Y_OFFSET;
        write(SSD_SELECT_VERTICAL);
        write(data, 2);
        delete[] data;
        write(SSD_WRITE_ENABLE);
        mMutex.lock();
        for(uint8_t i=yStartDrawing;i<yEndDrawing || i==yStartDrawing;i++){
            write((uint8_t *)(*drawingScreenBuffer + i*SCREEN_SIZE + xStartDrawing),
            2 * (xEndDrawing-xStartDrawing+1)); // TODO Check!!!
        }
        mMutex.unlock();
    }
}





SSD_Error SSD1351::boundaryCheck(uint8_t xPosition, uint8_t yPosition, int16_t deltaX, int16_t deltaY){
    int16_t minX = (xPosition < xPosition + deltaX) ? xPosition : xPosition + deltaX;
    int16_t maxX = (xPosition > xPosition + deltaX) ? xPosition : xPosition + deltaX;
    int16_t minY = (yPosition < yPosition + deltaY) ? yPosition : yPosition + deltaY;
    int16_t maxY = (yPosition > yPosition + deltaY) ? yPosition : yPosition + deltaY;
    if(minX < 0)                {return SSD_OUT_OF_LEFT_BORDER;}
    if(maxX > SCREEN_SIZE - 1)  {return SSD_OUT_OF_RIGHT_BORDER;}   
    if(minY < 0)                {return SSD_OUT_OF_TOP_BORDER;}             
    if(maxY > SCREEN_SIZE - 1)  {return SSD_OUT_OF_BOTTOM_BORDER;}
    
    if(minX < xStartActive)  {xStartActive = minX;}
    if(maxX > xEndActive)    {xEndActive   = maxX;}
    if(minY < yStartActive)  {yStartActive = minY;}
    if(maxY > yEndActive)    {yEndActive   = maxY;}

    return SSD_NO_ERROR;
}

SSD_Error SSD1351::addLineInternalSimple(uint8_t xPosition, uint8_t yPosition, uint8_t length, float angle, uint16_t colour, bool topOrBottom){
    float cosAngle = cos(angle * M_PI/180);
    float sinAngle = sin(angle * M_PI/180);
    SSD_Error error = boundaryCheck(xPosition, yPosition, lround(length*cosAngle), lround(length*sin(angle)));
    // Checks whether the line would go beyond the screen borders, and shrinks it accordingly.
    switch(error){
        //TODO Check!
        case SSD_OUT_OF_LEFT_BORDER:{
            length-=abs(lround((length*cosAngle + xPosition)/cosAngle));
            break;
        }
        case SSD_OUT_OF_RIGHT_BORDER:{
            length-=abs(lround((length*cosAngle + xPosition - SCREEN_SIZE)/cosAngle));
            break;
        }
        case SSD_OUT_OF_TOP_BORDER:{
            length-=abs(lround((length*sinAngle + yPosition)/sinAngle));
            break;
        }
        case SSD_OUT_OF_BOTTOM_BORDER:{
            length-=abs(lround((length*sinAngle + yPosition - SCREEN_SIZE)/sinAngle));
            break;
        }
        case SSD_NO_ERROR:{}
    }
    // This identifies whether the line is compared to the diagonal lines, 
    // which is used to determine whether the majority of the pixels will
    // be shifted vertically or horizontally compared to the previous ones.
    int8_t transversalCoefficient, tangentialCoefficient, counter, counter2;
    if(std::abs(sinAngle)>=0.5){
        length*=sinAngle;
        transversalCoefficient = copysign(1, cosAngle);
        tangentialCoefficient = copysign(SCREEN_SIZE, sinAngle);
        if(cosAngle){
            counter = abs(lround(sinAngle/cosAngle));
            counter2 = abs(lround(2*sinAngle/cosAngle));
            if(counter2==2*counter){counter2 = length;}
        }
        else{
            counter = length;
            counter2 = length;
        }
    }
    else{
        length*=cosAngle;
        transversalCoefficient = copysign(SCREEN_SIZE, sinAngle);
        tangentialCoefficient = copysign(1, cosAngle);
        if(sinAngle){
            counter = abs(lround(cosAngle/sinAngle));
            counter2 = abs(lround(2*cosAngle/sinAngle));
            if(counter2==2*counter){counter2 = length;}
        }
        else{
            counter = length;
            counter2 = length;
        }
    }
    int16_t coordinates = xPosition + SCREEN_SIZE * yPosition;
    for(uint i=0;i<length;i++){
        if(i){
            if(i%counter==0){
                coordinates+=transversalCoefficient;
                if(counter2!=length){   
                    int8_t counterDummy = counter;
                    counter = counter2;
                    counter2 = counterDummy;
                }
            }
            coordinates+=tangentialCoefficient;
        }
        if(topOrBottom && (*activeScreenBuffer)[coordinates]){}
            else{(*activeScreenBuffer)[coordinates] = colour;}
    }
    return error;
}

SSD_Error SSD1351::addLineInternalExact(uint8_t xPosition, uint8_t yPosition, uint8_t length, float angle, uint16_t colour, bool topOrBottom){
    float cosAngle = cos(angle * M_PI/180);
    float sinAngle = sin(angle * M_PI/180);
    SSD_Error error = boundaryCheck(xPosition, yPosition, lround(length*cosAngle), lround(length*sinAngle));
    // Checks whether the line would go beyond the screen borders, and shrinks it accordingly.
    switch(error){
        //TODO Check!
        case SSD_OUT_OF_LEFT_BORDER:{
            length-=abs(lround((length*cosAngle + xPosition)/cosAngle));
            break;
        }
        case SSD_OUT_OF_RIGHT_BORDER:{
            length-=abs(lround((length*cosAngle + xPosition - SCREEN_SIZE)/cosAngle));
            break;
        }
        case SSD_OUT_OF_TOP_BORDER:{
            length-=abs(lround((length*sinAngle + yPosition)/sinAngle));
            break;
        }
        case SSD_OUT_OF_BOTTOM_BORDER:{
            length-=abs(lround((length*sinAngle + yPosition - SCREEN_SIZE)/sinAngle));
            break;
        }
        case SSD_NO_ERROR:{}
    }
    int16_t coordinates = SCREEN_SIZE * yPosition + xPosition;
    for(uint8_t i=0;i<length;i++){
        if(topOrBottom && (*activeScreenBuffer)[coordinates + lround(i*cosAngle) + SCREEN_SIZE * lround(i*sinAngle)]){}        
        else{(*activeScreenBuffer)[coordinates + lround(i*cosAngle) + SCREEN_SIZE * lround(i*sinAngle)] = colour;} 
   }
   return error;
}

int SSD1351::write(SSD_Command command){
    dataORcommand = 0;
    int result = mSPI.write((uint8_t) command);
    return result;
}

int SSD1351::write(uint8_t *data, int length){
    int result, tempResult = 0;
    dataORcommand = 1;
    for(uint8_t i=0;i<length;i++){
        tempResult = mSPI.write(*(data+i));
        if(tempResult){
            result = tempResult;
            break;
        }
    }
    return result;
}