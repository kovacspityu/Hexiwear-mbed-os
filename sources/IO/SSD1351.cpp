#include "SSD1351_enum.h"
#include "SSD1351.h"
#include <algorithm>
#include <cmath>
#include "SSD1351_Fonts.h"

using namespace SSD;

uint8_t SSD1351::xStartActive = SCREEN_SIZE;
uint8_t SSD1351::xEndActive = 0; 
uint8_t SSD1351::yStartActive = SCREEN_SIZE; 
uint8_t SSD1351::yEndActive = 0;
uint8_t SSD1351::xStartDrawing = 0;
uint8_t SSD1351::xEndDrawing = SCREEN_SIZE - 1; 
uint8_t SSD1351::yStartDrawing = 0; 
uint8_t SSD1351::yEndDrawing = SCREEN_SIZE - 1;


//TODO There's the possibility of using 3 buffers: one that is what's currently been written on,
//TODO one that is being drawn on screen, and one more that doesn't exist right now that is
//TODO the last screen drawn, so that one could compare it to the buffer to draw and further shrink 
//TODO the amount of data that needs to be updated, using the ability of the OLED to be updated one pixel at a time.
//TODO It would also help if one wanted to add animation support in any form, as it's a natural fit for them.

SSD1351::SSD1351() : mSPI(PTB22, NC, PTB21, PTB20), dataORcommand(PTD15), mPower(PTC13), resetPin(PTE6), chipSelect(PTB20){
    mSPI.frequency(8000000);
    mSPI.format(8, 3);
    reset();
    screenBuffer1 = new uint16_t[SCREEN_SIZE * SCREEN_SIZE];
    screenBuffer2 = new uint16_t[SCREEN_SIZE * SCREEN_SIZE];
    activeScreenBuffer = &screenBuffer2;
    drawingScreenBuffer = &screenBuffer1;
    fill(screenBuffer1, screenBuffer1 + SCREEN_SIZE*SCREEN_SIZE, 0);
    fill(screenBuffer2, screenBuffer2 + SCREEN_SIZE*SCREEN_SIZE, 0);
    mThread.start(callback(this, &SSD1351::workerDraw));
    mThread.set_priority(osPriorityAboveNormal);
    toBeCleared = false;
    unlockCommands();
    write(SLEEP);
    uint8_t *data = new uint8_t[3];
    data[0] = 0xF1;
    write(FPS);
    write(data);
    data[0] = 0;
    data[1] = SCREEN_SIZE;
    write(ACTIVE_ROWS);
    write(data+1);
    data[0] = 0x60;
    write(OPTIONS);
    write(data);
    data[0] = X_OFFSET;
    data[1] = SCREEN_SIZE - 1 + X_OFFSET;
    write(SELECT_HORIZONTAL);
    write(data, 2);
    data[0] = Y_OFFSET;
    data[1] = SCREEN_SIZE - 1 + Y_OFFSET;
    write(SELECT_VERTICAL);
    write(data, 2); 
    data[0] = SCREEN_SIZE;
    write(IMAGE_V_OFFSET);
    write(data);
    data[0] = 0;
    write(DISPLAY_V_OFFSET);
    write(data); 
    setChargePeriods(0x32);
    write(DISPLAY_ON); 
    setBrightness(0xCF);
    data[0] = 0x01;
    write(PHASE_3);
    write(data);
    delete[] data;
    write(WAKE_UP);
    mThread.signal_set(0x01);
}


SSD1351::~SSD1351(){
    // TODO delete[] whatever we have new[]
    delete[] screenBuffer1;
    delete[] screenBuffer2;
    mThread.terminate();
    powerOff();
}

void SSD1351::powerUp(){
    resetPin = 0;
    wait_ms(2);
    resetPin = 1;
    wait_ms(1);
    mPower = 1;
    wait_ms(1);
    write(WAKE_UP);
    wait_ms(200);
}

void SSD1351::powerOff(){
    write(SLEEP);
    mPower = 0;
    wait_ms(1);
}

void SSD1351::reset(){
    powerOff();
    powerUp();
}

void SSD1351::sleep(){
    uint8_t data = 0;
    write(SLEEP);
    write(SLEEP_EXTRA);
    write(&data);
}

void SSD1351::wakeUp(){
    uint8_t data = 1;
    write(&data);
    write(SLEEP_EXTRA);
    write(WAKE_UP);
}


void SSD1351::setBrightness(uint8_t brightness){
    brightness%=17;
    write(BRIGHTNESS);
    write(&brightness);
}

void SSD1351::setColourBrightness(uint8_t red, uint8_t green, uint8_t blue){
    uint8_t brightness[3] = {red, green, blue};
    write(COLOUR_BRIGTHNESS);
    write(brightness, 3);
}

void SSD1351::draw(bool keepScreen){
    while(mThread.get_state() != Thread::WaitingThreadFlag) ;
    if(toBeCleared){
        clearBuffer();
        toBeCleared = false;
    }
    if(*activeScreenBuffer == screenBuffer1){
        activeScreenBuffer  = &screenBuffer2;
        drawingScreenBuffer = &screenBuffer1;
    }
    else{
        activeScreenBuffer  = &screenBuffer1;
        drawingScreenBuffer = &screenBuffer2;
    }
    xStartDrawing = xStartActive;
    xEndDrawing = xEndActive;
    yStartDrawing = yStartActive;
    yEndDrawing = yEndActive;
    if(keepScreen){
        memcpy((void*) *activeScreenBuffer, (void*) *drawingScreenBuffer, 2*SCREEN_SIZE*SCREEN_SIZE);
    }
    else{
        fill(*activeScreenBuffer, *activeScreenBuffer + SCREEN_SIZE*SCREEN_SIZE, 0);
        toBeCleared = true;
    }
    xStartActive=SCREEN_SIZE;
    xEndActive=0;
    yStartActive=SCREEN_SIZE;
    yEndActive=0;
    mThread.signal_set(0x01);
}

void SSD1351::drawAndKeep(){
    draw(true);
}

void SSD1351::drawAndClear(){
    draw(false);
}




void SSD1351::dumpScreen(uint16_t *screen, uint8_t &xPosition, uint8_t &yPosition, uint8_t &width, uint8_t &height){
    xPosition = xStartActive;
    yPosition = yStartActive;
    width = xEndActive-xStartActive;
    height = yEndActive-yStartActive;
    screen = new uint16_t[width*height];
    for(uint8_t j=yStartActive;j<=yEndActive;j++){
        for(uint8_t i=xStartActive;i<=xEndActive;i++){
            screen[(j-yStartActive)*SCREEN_SIZE + i-xStartActive] = (*activeScreenBuffer)[j*SCREEN_SIZE + i];
        }
    }
}



Error SSD1351::addBox(uint8_t xPosition, uint8_t yPosition, uint8_t width, uint8_t height, uint16_t internalColour, uint8_t internalThickness, bool topOrBottom, uint16_t externalColour, uint8_t externalThickness){
    Error            error=addLine(xPosition + internalThickness, yPosition, width - 2*internalThickness, 0, internalColour, internalThickness, topOrBottom, externalColour, externalThickness);
    error = (Error) (error|addLine(xPosition + width, yPosition + internalThickness, height - 2*internalThickness, 90, internalColour, internalThickness, topOrBottom, externalColour, externalThickness));
    error = (Error) (error|addLine(xPosition + internalThickness, yPosition + height, width - 2*internalThickness, 0, internalColour, internalThickness, topOrBottom, externalColour, externalThickness));
    error = (Error) (error|addLine(xPosition, yPosition + internalThickness, height - 2*internalThickness, 90, internalColour, internalThickness, topOrBottom, externalColour, externalThickness));
    //Each of the subsequent groups fills in the three areas at the corners on that delimit the box, 
    //in order the one that is vertically outside together with the one that is both vertically and horizontally outside, 
    //the one to the side, and finally the internal corners.
    // -------------------------------
    //|      1
    //|_______________________________
    //|     |
    //|  2  |      _____________________
    //|     |     |  3  |
    //|     |     |_____|______________
    //|     |     |     |
    //|     |     |     |
    if(internalThickness){
        //Left uppermost corner
        error = (Error) (error|fillArea(xPosition - internalThickness + 1 - externalThickness, yPosition - internalThickness + 1 - externalThickness, 2*internalThickness + externalThickness, externalThickness, externalColour, topOrBottom));
        error = (Error) (error|fillArea(xPosition - internalThickness + 1 - externalThickness, yPosition - internalThickness + 1, externalThickness, 2*internalThickness, externalColour, topOrBottom));
        error = (Error) (error|fillArea(xPosition - internalThickness + 1, yPosition - internalThickness + 1, 2*internalThickness - 1, 2*internalThickness - 1, internalColour, topOrBottom));
        //Right uppermost corner
        error = (Error) (error|fillArea(xPosition + width - internalThickness + 1, yPosition - internalThickness + 1 - externalThickness, 2*internalThickness - 1 + externalThickness, externalThickness, externalColour, topOrBottom));
        error = (Error) (error|fillArea(xPosition + width + internalThickness, yPosition - internalThickness + 1, externalThickness, 2*internalThickness - 1, externalColour, topOrBottom));
        error = (Error) (error|fillArea(xPosition + width - internalThickness + 1, yPosition - internalThickness + 1, 2*internalThickness - 1, 2*internalThickness - 1, internalColour, topOrBottom));
        //Left lowermost corner
        error = (Error) (error|fillArea(xPosition - internalThickness + 1 - externalThickness, yPosition + height + internalThickness , 2*internalThickness + externalThickness, externalThickness, externalColour, topOrBottom));
        error = (Error) (error|fillArea(xPosition - internalThickness + 1 - externalThickness, yPosition + height - internalThickness + 1, externalThickness, 2*internalThickness - 1, externalColour, topOrBottom));
        error = (Error) (error|fillArea(xPosition - internalThickness + 1, yPosition + height - internalThickness + 1, 2*internalThickness - 1, 2*internalThickness - 1, internalColour, topOrBottom));   
        //Right lowermost corner
        error = (Error) (error|fillArea(xPosition + width - internalThickness + 1, yPosition + height + internalThickness, 2*internalThickness - 1 + externalThickness, externalThickness, externalColour, topOrBottom));
        error = (Error) (error|fillArea(xPosition + width + internalThickness, yPosition + height - internalThickness + 1, externalThickness, 2*internalThickness - 1, externalColour, topOrBottom));
        error = (Error) (error|fillArea(xPosition + width - internalThickness + 1, yPosition + height - internalThickness + 1, 2*internalThickness - 1, 2*internalThickness - 1, internalColour, topOrBottom));
    }
    return error;
}

Error SSD1351::addRoundedBox(uint8_t xPosition, uint8_t yPosition, uint8_t width, uint8_t height, float flexingRatio, uint16_t internalColour, uint8_t internalThickness, bool topOrBottom, uint16_t externalColour, uint8_t externalThickness){
    flexingRatio=fabs(flexingRatio);
    if(flexingRatio>0.5){flexingRatio=0.5;}
    uint8_t delta = lround(min(width, height)*flexingRatio/2);
    Error error =           addLineP2P(xPosition + delta, yPosition, xPosition + width - delta, yPosition, internalColour, internalThickness, topOrBottom, externalColour, externalThickness);
    error = (Error)  (error|addLineP2P(xPosition + width, yPosition + delta, xPosition + width, yPosition + height - delta, internalColour, internalThickness, topOrBottom, externalColour, externalThickness));
    error = (Error)  (error|addLineP2P(xPosition + delta, yPosition + height, xPosition + width - delta, yPosition + height, internalColour, internalThickness, topOrBottom, externalColour, externalThickness));
    error = (Error)  (error|addLineP2P(xPosition, yPosition + delta, xPosition, yPosition + height - delta, internalColour, internalThickness, topOrBottom, externalColour, externalThickness));
    //Checks if the rounded vertices require just a single 45° lines or three lines at multiples of 22.5 degrees.
    if(min(width, height)*flexingRatio>FLEXING_THRESHOLD){
        //This requires a bit trigonometry, mostly the formula to get the sine and cosine of an angle in terms of its double.
        //Top-Left vertex
        float length = ((float) delta)/ (sqrt(1 + M_SQRT1_2) + M_SQRT1_2);
        uint8_t lengthSin=lround(length*sin(22.5*M_PI/180));
        uint8_t lengthCos=lround(length*cos(22.5*M_PI/180));
        for(uint8_t j=0;j<=internalThickness+externalThickness;j++){
            //Top-Left Vertex
            error = (Error) (error|addLineP2P(xPosition + j, yPosition + delta, xPosition + j + lengthSin, yPosition + delta - lengthCos, j>internalThickness ? externalColour : internalColour, 0, topOrBottom));
            error = (Error) (error|addLineP2P(xPosition + j + lengthSin, yPosition + delta - lengthCos, xPosition + delta - lengthCos, yPosition + j + lengthSin, j>internalThickness ? externalColour : internalColour, 0, topOrBottom));
            error = (Error) (error|addLineP2P(xPosition + delta - lengthCos, yPosition + j + lengthSin, xPosition + delta, yPosition + j, j>internalThickness ? externalColour : internalColour, 0, topOrBottom));
            //Top-Right Vertex
            error = (Error) (error|addLineP2P(xPosition - j + width, yPosition + delta, xPosition - j + width - lengthSin, yPosition + delta - lengthCos, j>internalThickness ? externalColour : internalColour, 0, topOrBottom));
            error = (Error) (error|addLineP2P(xPosition - j + width - lengthSin, yPosition + delta - lengthCos, xPosition + width - delta + lengthCos, yPosition + j + lengthSin, j>internalThickness ? externalColour : internalColour, 0, topOrBottom));
            error = (Error) (error|addLineP2P(xPosition + width - delta + lengthCos, yPosition + j + lengthSin, xPosition + width - delta, yPosition + j, j>internalThickness ? externalColour : internalColour, 0, topOrBottom));
            //Bottom-Left Vertex
            error = (Error) (error|addLineP2P(xPosition + j, yPosition + height - delta, xPosition + j + lengthSin, yPosition + height - delta + lengthCos, j>internalThickness ? externalColour : internalColour, 0, topOrBottom));
            error = (Error) (error|addLineP2P(xPosition + j + lengthSin, yPosition + height - delta + lengthCos, xPosition + delta - lengthCos, yPosition - j + height - lengthSin, j>internalThickness ? externalColour : internalColour, 0, topOrBottom));
            error = (Error) (error|addLineP2P(xPosition + delta - lengthCos, yPosition - j + height - lengthSin, xPosition + delta, yPosition + height - j, j>internalThickness ? externalColour : internalColour, 0, topOrBottom));
            //Bottom-Right Vertex
            error = (Error) (error|addLineP2P(xPosition - j + width, yPosition + height - delta, xPosition - j + width - lengthSin, yPosition + height - delta + lengthCos, j>internalThickness ? externalColour : internalColour, 0, topOrBottom));
            error = (Error) (error|addLineP2P(xPosition - j + width - lengthSin, yPosition + height - delta + lengthCos, xPosition + width - delta + lengthCos, yPosition - j + height - lengthSin, j>internalThickness ? externalColour : internalColour, 0, topOrBottom));
            error = (Error) (error|addLineP2P(xPosition + width - delta + lengthCos, yPosition - j + height - lengthSin, xPosition + width - delta, yPosition + height - j, j>internalThickness ? externalColour : internalColour, 0, topOrBottom));
            if(j){
                //Top-Left Vertex
                error = (Error) (error|addLineP2P(xPosition - j, yPosition + delta, xPosition - j + lengthSin, yPosition + delta - lengthCos, j>internalThickness ? externalColour : internalColour, 0, topOrBottom));
                error = (Error) (error|addLineP2P(xPosition - j + lengthSin, yPosition + delta - lengthCos, xPosition + delta - lengthCos, yPosition - j + lengthSin, j>internalThickness ? externalColour : internalColour, 0, topOrBottom));
                error = (Error) (error|addLineP2P(xPosition + delta - lengthCos, yPosition + lengthSin - j, xPosition + delta, yPosition - j, j>internalThickness ? externalColour : internalColour, 0, topOrBottom));
                //Top-Right Vertex
                error = (Error) (error|addLineP2P(xPosition + j + width, yPosition + delta, xPosition + j + width - lengthSin, yPosition + delta - lengthCos, j>internalThickness ? externalColour : internalColour, 0, topOrBottom));
                error = (Error) (error|addLineP2P(xPosition + j + width - lengthSin, yPosition + delta - lengthCos, xPosition + width - delta + lengthCos, yPosition - j + lengthSin, j>internalThickness ? externalColour : internalColour, 0, topOrBottom));
                error = (Error) (error|addLineP2P(xPosition + width - delta + lengthCos, yPosition - j + lengthSin, xPosition + width - delta, yPosition - j, j>internalThickness ? externalColour : internalColour, 0, topOrBottom));
                //Bottom-Left Vertex
                error = (Error) (error|addLineP2P(xPosition - j, yPosition + height - delta, xPosition - j + lengthSin, yPosition + height - delta + lengthCos, j>internalThickness ? externalColour : internalColour, 0, topOrBottom));
                error = (Error) (error|addLineP2P(xPosition - j + lengthSin, yPosition + height - delta + lengthCos, xPosition + delta - lengthCos, yPosition + j + height - lengthSin, j>internalThickness ? externalColour : internalColour, 0, topOrBottom));
                error = (Error) (error|addLineP2P(xPosition + delta - lengthCos, yPosition + j + height - lengthSin, xPosition + delta, yPosition + height + j, j>internalThickness ? externalColour : internalColour, 0, topOrBottom));
                //Bottom-Right Vertex
                error = (Error) (error|addLineP2P(xPosition + j + width, yPosition + height - delta, xPosition + j + width - lengthSin, yPosition + height - delta + lengthCos, j>internalThickness ? externalColour : internalColour, 0, topOrBottom));
                error = (Error) (error|addLineP2P(xPosition + j + width - lengthSin, yPosition + height - delta + lengthCos, xPosition + width - delta + lengthCos, yPosition + j + height - lengthSin, j>internalThickness ? externalColour : internalColour, 0, topOrBottom));
                error = (Error) (error|addLineP2P(xPosition + width - delta + lengthCos, yPosition + j + height - lengthSin, xPosition + width - delta, yPosition + height + j, j>internalThickness ? externalColour : internalColour, 0, topOrBottom));
            }
        }
   }
    else{
        for(uint8_t j=0;j<=internalThickness+externalThickness;j++){
            error = (Error) (error|addLineP2P(xPosition + j, yPosition + delta, xPosition + delta, yPosition + j, j>internalThickness ? externalColour : internalColour, 0, topOrBottom));
            error = (Error) (error|addLineP2P(xPosition + width - delta, yPosition + j, xPosition - j + width, yPosition + delta, j>internalThickness ? externalColour : internalColour, 0, topOrBottom));
            error = (Error) (error|addLineP2P(xPosition + j, yPosition + height - delta, xPosition + delta, yPosition + height - j, j>internalThickness ? externalColour : internalColour, 0, topOrBottom));
            error = (Error) (error|addLineP2P(xPosition + width - delta, yPosition + height - j, xPosition - j + width, yPosition + height - delta, j>internalThickness ? externalColour : internalColour, 0, topOrBottom));
            if(j){
                error = (Error) (error|addLineP2P(xPosition - j, yPosition + delta, xPosition + delta, yPosition - j, j>internalThickness ? externalColour : internalColour, 0, topOrBottom));
                error = (Error) (error|addLineP2P(xPosition + width - delta, yPosition - j, xPosition + j + width, yPosition + delta, j>internalThickness ? externalColour : internalColour, 0, topOrBottom));
                error = (Error) (error|addLineP2P(xPosition - j, yPosition + height - delta, xPosition + delta, yPosition + height + j, j>internalThickness ? externalColour : internalColour, 0, topOrBottom));
                error = (Error) (error|addLineP2P(xPosition + width - delta, yPosition + height + j, xPosition + j + width, yPosition + height - delta, j>internalThickness ? externalColour : internalColour, 0, topOrBottom));
            }
        }
    }
    return error;
}

Error SSD1351::addText(uint8_t xPosition, uint8_t yPosition, char* text, uint16_t textLength, bool topOrBottom, TextProperties_t textProperties){
    return addText(xPosition, yPosition, text, textLength, topOrBottom, textProperties, SCREEN_SIZE, SCREEN_SIZE);
}

Error SSD1351::addTextInBox(char* text, uint16_t textLength, TextProperties_t textProperties, uint8_t xPosition, uint8_t yPosition, uint8_t width, uint8_t height, uint16_t internalColour, uint8_t internalThickness, bool topOrBottom, uint16_t externalColour, uint8_t externalThickness){
    Error error = addBox(xPosition, yPosition, width, height, internalColour, internalThickness, topOrBottom, externalColour, externalThickness);
    error = (Error) (error|addText(xPosition + internalThickness + externalThickness, yPosition + internalThickness + externalThickness, text, textLength, topOrBottom, textProperties, width + xPosition, height + yPosition));
    return error;
}

Error SSD1351::addTextInRoundedBox(char* text, uint16_t textLength, TextProperties_t textProperties, uint8_t xPosition, uint8_t yPosition, uint8_t width, uint8_t height, float flexingRatio, uint16_t internalColour, uint8_t internalThickness, bool topOrBottom, uint16_t externalColour, uint8_t externalThickness){
    Error error = addRoundedBox(xPosition, yPosition, width, height, flexingRatio, internalColour, internalThickness, topOrBottom, externalColour, externalThickness);
    uint8_t delta = lround(min(width, height)*min(fabs(flexingRatio), 0.125f));
    error = (Error) (error|addText(xPosition + internalThickness + externalThickness + delta, yPosition + internalThickness + externalThickness + delta, text, textLength, topOrBottom, textProperties, width + xPosition, height + yPosition));
    return error;
}

Error SSD1351::addCircle(uint8_t xPosition, uint8_t yPosition, uint8_t width, uint8_t height, uint16_t internalColour, uint8_t internalThickness, bool topOrBottom, uint16_t externalColour, uint8_t externalThickness){
    return addCircle(xPosition + lround(width/2), yPosition + lround(height/2), internalColour, internalThickness, topOrBottom, externalColour, externalThickness);
}

Error SSD1351::addCircle(uint8_t xCenter, uint8_t yCenter, float radius, uint16_t internalColour, uint8_t internalThickness, bool topOrBottom, uint16_t externalColour, uint8_t externalThickness){
    // Uses the addRoundedBox function with the max available flexing ratio if the radius is small enough for the approximation to hold.
    uint8_t uRadius = lround(abs(radius));
    if(radius<THRESHOLD_RADIUS){
        return addRoundedBox(xCenter - uRadius, yCenter - uRadius, 2*uRadius, 2*uRadius, 1, internalColour, internalThickness, topOrBottom, externalColour, externalThickness);
    }
    return addCircleInternal(xCenter, yCenter, radius, internalColour, internalThickness, topOrBottom, false, externalColour, externalThickness);
}

Error SSD1351::addFilledCircle(uint8_t xCenter, uint8_t yCenter, float radius, uint16_t lineColour, uint8_t thickness, bool topOrBottom, uint16_t fillColour){
    return addCircleInternal(xCenter, yCenter, radius, lineColour, thickness, topOrBottom, true, fillColour, 0);
}

Error SSD1351::addEmptyCircle(uint8_t xCenter, uint8_t yCenter, float radius, uint16_t lineColour, bool topOrBottom){
    return addCircle(xCenter, yCenter, radius, lineColour, 0, topOrBottom);
}      

Error SSD1351::addLineP2P(int16_t xStarting, int16_t yStarting, int16_t xEnd, int16_t yEnd, uint16_t internalColour, uint8_t internalThickness, bool topOrBottom, uint16_t externalColour, uint8_t externalThickness){
    // This function draws a line by approximating it with either an horizontal, vertical or diagonal one,
    // with two orders of corrections to them. This allows to use mostly int variables in the drawing algorithm.
    if(xStarting==xEnd && yStarting==yEnd){return NO_ERROR;}
    int8_t counter, counter2;
    int8_t tangentialDelta, transversalDelta;
    uint8_t length=max(abs(xEnd-xStarting), abs(yEnd-yStarting)) + 1;
    float coefficient = ((float)(yEnd-yStarting))/(xEnd-xStarting);
    uint8_t internalLength=length;
    int16_t x1=xStarting;
    int16_t x2=xEnd;
    int16_t y1=yStarting;
    int16_t y2=yEnd;
    Error error=shrinkLine(x1, y1, x2, y2, coefficient, internalLength);
    calculateLineParameters(x1, y1, x2, y2, counter, counter2, tangentialDelta, transversalDelta);
    float scaleFactor = hypot(xEnd-xStarting, yEnd-yStarting)/(length - 1);
    internalThickness = lround(scaleFactor*internalThickness);
    externalThickness = lround(scaleFactor*externalThickness);
    Error tempError;
    int16_t coordinates;
    for(uint8_t i=0;i<=internalThickness+externalThickness;i++){  
        tempError = shrinkLine(xStarting + div(i*transversalDelta, SCREEN_SIZE).rem, yStarting + (i*transversalDelta)/SCREEN_SIZE, xEnd + div(i*transversalDelta, SCREEN_SIZE).rem, yEnd + (i*transversalDelta)/SCREEN_SIZE, coefficient, coordinates, internalLength);
        error=(Error) (error|tempError);
        addLineInternal(coordinates, internalLength, tangentialDelta, transversalDelta, counter, counter2, i>internalThickness ? externalColour : internalColour, topOrBottom);
        if(i){
            tempError = shrinkLine(xStarting - div(i*transversalDelta, SCREEN_SIZE).rem, yStarting - (i*transversalDelta)/SCREEN_SIZE, xEnd - div(i*transversalDelta, SCREEN_SIZE).rem, yEnd - (i*transversalDelta)/SCREEN_SIZE, coefficient, coordinates, internalLength);
            error=(Error) (error|tempError);   
            addLineInternal(coordinates, internalLength, tangentialDelta, transversalDelta, counter, counter2, i>internalThickness ? externalColour : internalColour, topOrBottom);
        }
    }
    return error;
}

Error SSD1351::addLine(int16_t xPosition, int16_t yPosition, uint8_t length, int16_t angle, uint16_t internalColour, uint8_t internalThickness, bool topOrBottom, uint16_t externalColour, uint8_t externalThickness){
    return addLineP2P(xPosition, yPosition, xPosition+lround(length*cos(angle*M_PI/180)), yPosition+lround(length*sin(angle*M_PI/180)), internalColour, internalThickness, topOrBottom, externalColour, externalThickness);
}

Error SSD1351::addLineOnTop(int16_t xPosition, int16_t yPosition, uint8_t length, int16_t angle, uint16_t internalColour, uint8_t internalThickness, uint16_t externalColour, uint8_t externalThickness){
    return addLine(xPosition, yPosition, length, angle, internalColour, internalThickness, true, externalColour, externalThickness);
}

Error SSD1351::addLineAtBottom(int16_t xPosition, int16_t yPosition, uint8_t length, int16_t angle, uint16_t internalColour, uint8_t internalThickness, uint16_t externalColour, uint8_t externalThickness){
    return addLine(xPosition, yPosition, length, angle, internalColour, internalThickness, false, externalColour, externalThickness);
}


Error SSD1351::addImage(uint16_t *image, uint8_t xPosition, uint8_t yPosition, uint8_t width, uint8_t height, bool topOrBottom){
    uint8_t xStart, xEnd, yStart, yEnd;
    Error error = boundaryCheck(xPosition, yPosition, width, height);
    if(error&OUT_OF_LEFT_BORDER){
        xStart=0;
    }
    else{xStart=xPosition;}
    if(error&OUT_OF_RIGHT_BORDER){
        xEnd=SCREEN_SIZE - xPosition;
    }
    else{xEnd=xPosition+width;}
    if(error&OUT_OF_TOP_BORDER){
        yStart=0;
    }
    else{yStart=yPosition;}
    if(error&OUT_OF_BOTTOM_BORDER){
        yEnd=SCREEN_SIZE - yPosition;
    }
    else{yEnd=yPosition+height;}
    for(uint j=yStart;j<=yEnd;j++){    
        for(uint i=xStart;i<=xEnd;i++){ 
            if(topOrBottom || (*activeScreenBuffer)[j*SCREEN_SIZE + i]){
                (*activeScreenBuffer)[j*SCREEN_SIZE + i] = image[(j - yPosition)*width + i - xPosition];
            }
        }
    }
    return NO_ERROR;
}

Error SSD1351::addImageOnTop(uint16_t *image, uint8_t xPosition, uint8_t yPosition, uint8_t width, uint8_t height){
    return addImage(image, xPosition, yPosition, width, height, true);
}

Error SSD1351::addImageAtBottom(uint16_t *image, uint8_t xPosition, uint8_t yPosition, uint8_t width, uint8_t height){
    return addImage(image, xPosition, yPosition, width, height, false);
}

Error SSD1351::fillArea(uint8_t xPosition, uint8_t yPosition, uint8_t width, uint8_t height, uint16_t colour, bool topOrBottom){
    Error error=boundaryCheck(xPosition, yPosition, width, height);
    if(error&OUT_OF_LEFT_BORDER){
        xPosition=0;
    }
    if(error&OUT_OF_RIGHT_BORDER){
        width=SCREEN_SIZE - xPosition;
    }
    if(error&OUT_OF_TOP_BORDER){
        yPosition=0;
    }
    if(error&OUT_OF_BOTTOM_BORDER){
        height=SCREEN_SIZE - yPosition;
    }
    boundaryCheck(xPosition, yPosition, width, height);
    for(uint j=yPosition;j<yPosition+height;j++){
        for(uint i=xPosition;i<xPosition+width;i++){
            if(topOrBottom || !(*activeScreenBuffer)[j*SCREEN_SIZE + i]){
                (*activeScreenBuffer)[j*SCREEN_SIZE + i] = colour;
                }
        }
    }
    return error;
}


void SSD1351::scrollingOn(bool direction, uint8_t startingRow, uint8_t numberOfRows, Scrolling_Period period){
    uint8_t data[5];
    if(direction){data[0] = 1;}
    else{data[0] = 64;}
    data[1] = min(startingRow, (uint8_t)(SCREEN_SIZE - 1));
    data[2] = min(numberOfRows, (uint8_t)(SCREEN_SIZE - data[1]));
    data[3] = 0;
    data[4] = period;
    write(SCROLLING);
    write(data, 5);
    write(SCROLLING_START);
}

void SSD1351::scrollingOff(){
    write(SCROLLING_STOP);
}

uint16_t SSD1351::getColour(uint8_t red, uint8_t green, uint8_t blue){
    return green>>3 | (red>>2)<<5 | (blue>>3)<<11;
}


void SSD1351::lock(){
    uint8_t data = 16;
    write(COMMAND_LOCK);
    write(&data);
}

void SSD1351::unlock(){
    uint8_t data = 12;
    write(COMMAND_LOCK);
    write(&data);
}

void SSD1351::lockCommands(){
    unlock();
    uint8_t data = 0xB0;
    write(COMMAND_LOCK);
    write(&data);
}

void SSD1351::unlockCommands(){
    unlock();
    uint8_t data = 0xB1;
    write(COMMAND_LOCK);
    write(&data);
}

void SSD1351::setChargePeriods(uint8_t periods){
    unlockCommands();
    write(PHASE_1_AND_2);
    write(&periods);
    lockCommands();
}


void SSD1351::workerDraw(){
    while(1){
        Thread::signal_wait(0x01);
        if(xStartDrawing<SCREEN_SIZE && yStartDrawing<SCREEN_SIZE){
            uint8_t data[2];
            data[0] = xStartDrawing + X_OFFSET;
            data[1] = xEndDrawing + X_OFFSET;
            mMutex.lock();
            write(SELECT_HORIZONTAL);
            write(data, 2);
            data[0] = yStartDrawing + Y_OFFSET;
            data[1] = yEndDrawing + Y_OFFSET;
            write(SELECT_VERTICAL);
            write(data, 2);
            write(WRITE_ENABLE);
            for(uint8_t i=yStartDrawing;i<=yEndDrawing;i++){
                write((uint8_t *)(*drawingScreenBuffer + i*SCREEN_SIZE + xStartDrawing),
                2 * (xEndDrawing-xStartDrawing+1));
            }
            mMutex.unlock();
        }
    }
}





Error SSD1351::boundaryCheck(int16_t xPosition, int16_t yPosition, int16_t deltaX, int16_t deltaY){
    // Checks if the rectangle defined by the parameters goes outside the screen in any direction,
    // in which case returns an error that specifies each direction it does.
    // Otherwise it updates the startActive variables, which determine what is actually written
    // on the SSD1351 hardware buffer. 
    //
    // !!!!THIS FUNCTION NEEDS THUS TO BE CALLED BY ALL FUNCTIONS THAT DRAW!!!!
    //

    int16_t minX = (xPosition < xPosition + deltaX) ? xPosition : xPosition + deltaX;
    int16_t maxX = (xPosition > xPosition + deltaX) ? xPosition : xPosition + deltaX;
    int16_t minY = (yPosition < yPosition + deltaY) ? yPosition : yPosition + deltaY;
    int16_t maxY = (yPosition > yPosition + deltaY) ? yPosition : yPosition + deltaY;
    Error error = NO_ERROR;
    if(minX < 0)                {error = (Error) (error| OUT_OF_LEFT_BORDER);}
    if(maxX > SCREEN_SIZE - 1)  {error = (Error) (error| OUT_OF_RIGHT_BORDER);}   
    if(minY < 0)                {error = (Error) (error| OUT_OF_TOP_BORDER);}             
    if(maxY > SCREEN_SIZE - 1)  {error = (Error) (error| OUT_OF_BOTTOM_BORDER);}
    
    if(error==NO_ERROR){
        if(minX < xStartActive)  {xStartActive = minX;}
        if(maxX > xEndActive)    {xEndActive   = maxX;}
        if(minY < yStartActive)  {yStartActive = minY;}
        if(maxY > yEndActive)    {yEndActive   = maxY;}
    }

    return error;
}

void SSD1351::clearBuffer(){
    // This function compares the last drawn screen with the current one, and only clears the parts
    // of the last screen that are not gonna be overwritten by the new one, reducing the amount
    // of communication with the SSD1351 needed to draw each new screen.
    //
    // It does this by recognizing which of the 7 areas in the ASCII image below the last drawn screen    
    // hold data, and then goes through a switch to clear them efficiently.
    //
    //
    //                          |                       |                          
    //                    1     |           2           |     3  
    //                 _________|_______________________|__________
    //                          |                       |
    //                          |         NEXT          |
    //                     4    |        SCREEN         |     5
    //                          |                       |
    //                 _________|_______________________|__________
    //                          |                       |
    //                     5    |           6           |     7
    //                          |                       |
    Error error = NO_ERROR;
    if(xStartDrawing<xStartActive){error= (Error) (error|OUT_OF_LEFT_BORDER);}
    if(xEndDrawing>xEndActive){error= (Error) (error|OUT_OF_RIGHT_BORDER);}
    if(yStartDrawing<yStartActive){error= (Error) (error|OUT_OF_TOP_BORDER);}
    if(yEndDrawing>yEndActive){error= (Error) (error|OUT_OF_BOTTOM_BORDER);}
    uint8_t xDummyStart=xStartDrawing;
    uint8_t xDummyEnd=xEndDrawing;
    uint8_t yDummyStart=yStartDrawing;
    uint8_t yDummyEnd=yEndDrawing;
    fill(*drawingScreenBuffer, *drawingScreenBuffer + SCREEN_SIZE*SCREEN_SIZE, 0);
    switch(error){
        case NO_ERROR                   : {
            mThread.signal_set(0x01);
            while(mThread.get_state() != Thread::WaitingThreadFlag) ;
            break;
            } 
        case OUT_OF_TOP_BORDER          : {
            yEndDrawing = yStartActive;
            mThread.signal_set(0x01);
            while(mThread.get_state() != Thread::WaitingThreadFlag) ;
            break;
            } 
        case OUT_OF_BOTTOM_BORDER       : {
            yStartDrawing = yEndActive;
            mThread.signal_set(0x01);
            while(mThread.get_state() != Thread::WaitingThreadFlag) ;
            break;
            } 
        case OUT_OF_RIGHT_BORDER        : {
            xStartDrawing = xEndActive;
            mThread.signal_set(0x01);
            while(mThread.get_state() != Thread::WaitingThreadFlag) ;
            break;
            } 
        case OUT_OF_LEFT_BORDER         : {
            xEndDrawing = xStartActive;
            mThread.signal_set(0x01);
            while(mThread.get_state() != Thread::WaitingThreadFlag) ;
            break;
            } 
        case OUT_OF_VERTICAL_BORDERS    : {
            xStartDrawing = xEndActive;
            mThread.signal_set(0x01);
            while(mThread.get_state() != Thread::WaitingThreadFlag) ;
            xStartDrawing = xDummyStart;
            xEndDrawing = xStartActive;
            mThread.signal_set(0x01);
            while(mThread.get_state() != Thread::WaitingThreadFlag) ;
            break;
            } 
        case OUT_OF_TOP_RIGHT_BORDER    : {
            yEndDrawing = yStartActive;
            mThread.signal_set(0x01);
            while(mThread.get_state() != Thread::WaitingThreadFlag) ;
            yStartDrawing = yStartActive;
            yEndDrawing = yDummyEnd;
            xStartDrawing = xEndActive;
            mThread.signal_set(0x01);
            while(mThread.get_state() != Thread::WaitingThreadFlag) ;
            break;
            } 
        case OUT_OF_TOP_LEFT_BORDER     : {
            yEndDrawing = yStartActive;
            mThread.signal_set(0x01);
            while(mThread.get_state() != Thread::WaitingThreadFlag) ;
            yStartDrawing = yStartActive;
            yEndDrawing = yDummyEnd;
            xEndDrawing = xStartActive;
            mThread.signal_set(0x01);
            while(mThread.get_state() != Thread::WaitingThreadFlag) ;
            break;
            } 
        case OUT_OF_BOTTOM_RIGHT_BORDER : {
            yStartDrawing = yEndActive;
            mThread.signal_set(0x01);
            while(mThread.get_state() != Thread::WaitingThreadFlag) ;
            yStartDrawing = yDummyStart;
            yEndDrawing = yEndActive;
            xStartDrawing = xEndActive;
            mThread.signal_set(0x01);
            while(mThread.get_state() != Thread::WaitingThreadFlag) ;
            break;
            } 
        case OUT_OF_BOTTOM_LEFT_BORDER  : {
            yStartDrawing = yEndActive;
            mThread.signal_set(0x01);
            while(mThread.get_state() != Thread::WaitingThreadFlag) ;
            yStartDrawing = yDummyStart;
            yEndDrawing = yEndActive;
            xEndDrawing = xStartActive;
            mThread.signal_set(0x01);
            while(mThread.get_state() != Thread::WaitingThreadFlag) ;
            break;
            } 
        case OUT_OF_HORIZONTAL_BORDERS  : {
            yEndDrawing = yStartActive;
            mThread.signal_set(0x01);
            while(mThread.get_state() != Thread::WaitingThreadFlag) ;
            yStartDrawing = yEndActive;
            yEndDrawing = yDummyEnd;
            mThread.signal_set(0x01);
            while(mThread.get_state() != Thread::WaitingThreadFlag) ;
            break;
            } 
        case OUT_ALL_BUT_TOP            : {
            yStartDrawing = yEndActive;
            mThread.signal_set(0x01);
            while(mThread.get_state() != Thread::WaitingThreadFlag) ;
            yStartDrawing = yDummyStart;
            yEndDrawing = yEndActive;
            xStartDrawing = xEndActive;
            mThread.signal_set(0x01);
            while(mThread.get_state() != Thread::WaitingThreadFlag) ;
            xStartDrawing = xDummyStart;
            xEndDrawing = xStartActive;
            mThread.signal_set(0x01);
            while(mThread.get_state() != Thread::WaitingThreadFlag) ;
            break;
            } 
        case OUT_ALL_BUT_BOTTOM         : {
            yEndDrawing = yStartActive;
            mThread.signal_set(0x01);
            while(mThread.get_state() != Thread::WaitingThreadFlag) ;
            yStartDrawing = yStartActive;
            yEndDrawing = yDummyEnd;
            xStartDrawing = xEndActive;
            mThread.signal_set(0x01);
            while(mThread.get_state() != Thread::WaitingThreadFlag) ;
            xStartDrawing = xDummyStart;
            xEndDrawing = xStartActive;
            mThread.signal_set(0x01);
            while(mThread.get_state() != Thread::WaitingThreadFlag) ;
            break;
            } 
        case OUT_ALL_BUT_RIGHT          : {
            xEndDrawing = xStartActive;
            mThread.signal_set(0x01);
            while(mThread.get_state() != Thread::WaitingThreadFlag) ;
            xStartDrawing = xStartActive;
            xEndDrawing = xDummyEnd;
            yEndDrawing = yStartActive;
            mThread.signal_set(0x01);
            while(mThread.get_state() != Thread::WaitingThreadFlag) ;
            yStartDrawing = yEndActive;
            yEndDrawing = yDummyEnd;
            mThread.signal_set(0x01);
            while(mThread.get_state() != Thread::WaitingThreadFlag) ;
            break;
            } 
        case OUT_ALL_BUT_LEFT           : {
            xStartDrawing = xEndActive;
            mThread.signal_set(0x01);
            while(mThread.get_state() != Thread::WaitingThreadFlag) ;
            xStartDrawing = xDummyStart;
            xEndDrawing = xEndActive;
            yEndDrawing = yStartActive;
            mThread.signal_set(0x01);
            while(mThread.get_state() != Thread::WaitingThreadFlag) ;
            yStartDrawing = yEndActive;
            yEndDrawing = yDummyEnd;
            mThread.signal_set(0x01);
            while(mThread.get_state() != Thread::WaitingThreadFlag) ;
            break;
            } 
        case OUT_ALL_BORDERS            : {
            xStartDrawing = xEndActive;
            mThread.signal_set(0x01);
            while(mThread.get_state() != Thread::WaitingThreadFlag) ;
            xStartDrawing = xDummyStart;
            xEndDrawing = xEndActive;
            yEndDrawing = yStartActive;
            mThread.signal_set(0x01);
            while(mThread.get_state() != Thread::WaitingThreadFlag) ;
            yStartDrawing = yEndActive;
            yEndDrawing = yDummyEnd;
            mThread.signal_set(0x01);
            while(mThread.get_state() != Thread::WaitingThreadFlag) ;
            xEndDrawing = xStartActive;
            yStartDrawing = yStartActive;
            yEndDrawing = yEndActive;
            mThread.signal_set(0x01);
            while(mThread.get_state() != Thread::WaitingThreadFlag) ;
            break;
        } 
    }
}

void SSD1351::addLineInternal(int16_t coordinates, uint8_t length, int8_t tangentialDelta, int8_t transversalDelta, int8_t counter, int8_t counter2, uint16_t colour,  bool topOrBottom){
    // The algorithm works by drawing a line that can be either horizontal, vertical or at 45°,
    // but with corrections every counter and counter2 pixels, 
    // to approximate the correct coordinates without calculating them exactly.

    // The existence of two different for cycles is merely to avoid abrupt jumps of two pixels or
    // the line correcting first in one direction and then the opposite.
    bool correctionDue = false;
    if(counter2*transversalDelta>0){
        for(uint8_t i=0;i<length;i++){
            if(i){
                if((i+1)%counter2==0){
                    correctionDue= true;
                }
                if((i+1)%counter==0){
                    // In this cycle the two orders of corrections are opposite to one another, 
                    // so the code simply prevents one first order correction from happening when
                    // the second order one has been met.
                    if(correctionDue){correctionDue=false;}
                    else{coordinates+=transversalDelta;}
                }
                coordinates+=tangentialDelta;
            }
            if(topOrBottom || !(*activeScreenBuffer)[coordinates]){(*activeScreenBuffer)[coordinates] = colour;}
        }
    }
    else{
        for(uint8_t i=0;i<length;i++){
            if(i){
                if((i+1)%counter2==0){
                    correctionDue= true;
                }
                if((i+1)%counter==0){
                    coordinates+=transversalDelta;
                }
                else{
                    if(correctionDue){
                        // In this case the two correction are in the same direction, so the only thing to ensure
                        // is that they don't cumulate on the same pixel.
                        coordinates+=abs(transversalDelta);
                        correctionDue=false;
                    }
                }
                coordinates+=tangentialDelta;
            }
            if(topOrBottom || !(*activeScreenBuffer)[coordinates]){(*activeScreenBuffer)[coordinates] = colour;}
        }
    }
}

Error SSD1351::addCircleInternal(uint8_t xCenter, uint8_t yCenter, uint8_t radius, uint16_t lineColour, uint8_t internalThickness, bool topOrBottom, bool fill, uint16_t otherColour, uint8_t externalThickness){
    // This works by checking line by line which x coordinates fulfill the circle equation, then draws both the
    // thickness by making use of calculateLineParameters, and fills the space in between the two solutions
    // if fill==true. calculateCircleParameters is used to identify if the circle exits the screen and provides
    // the correct extremes for both coordinates.
    //TODO Re-implement the use of these 4 coordinates to avoid spending CPU time on parts of the circle that are off-screen.
    uint8_t x1, x2, y1, y2;
    uint8_t thickness = internalThickness + externalThickness;
    Error error=calculateCircleParameters(xCenter, yCenter, thickness, radius, x1, x2, y1, y2);
    int16_t xSolution, xSolutionPrevious = xCenter - lround( sqrt( pow(radius, 2) - pow(y1 - yCenter, 2) ) ), xSolution2Previous=xSolutionPrevious;
    int16_t coordinates;
    uint8_t internalLength;
    for(uint8_t j=max(x1 + 2, yCenter - radius + 2);j<=min(x2, (uint8_t) (yCenter-radius/2) );j++){
        xSolution= xCenter - lround( sqrt( pow(radius, 2) - pow(j - yCenter, 2) ) );
        // Only draws the line every two loop passes.
        if((j-yCenter + radius -2)%2==0){   
            // Draws a line between two points of the circle in the top left, then between 
            // their mirror points across the y axis, then the mirrors of those two couples across the x axis,
            // then the mirrors of those four couples across the diagonals
            addLineP2P(xSolution2Previous, j-2, xSolution, j, lineColour, internalThickness, topOrBottom, otherColour, externalThickness);
            addLineP2P(2*xCenter - xSolution2Previous, 2*yCenter - j+2, 2*xCenter - xSolution, 2*yCenter - j, lineColour, internalThickness, topOrBottom, otherColour, externalThickness);
            addLineP2P(2*xCenter - xSolution2Previous, j-2, 2*xCenter - xSolution, j, lineColour, internalThickness, topOrBottom, otherColour, externalThickness);
            addLineP2P(xSolution2Previous, 2*yCenter - j+2, xSolution, 2*yCenter - j, lineColour, internalThickness, topOrBottom, otherColour, externalThickness);
            addLineP2P(j-2 - yCenter + xCenter, xSolution2Previous - xCenter + yCenter, j - yCenter + xCenter, xSolution - xCenter + yCenter, lineColour, internalThickness, topOrBottom, otherColour, externalThickness);
            addLineP2P(yCenter - j+2 + xCenter, xCenter - xSolution2Previous + yCenter, yCenter - j + xCenter, xCenter - xSolution + yCenter, lineColour, internalThickness, topOrBottom, otherColour, externalThickness);
            addLineP2P(j-2 - yCenter + xCenter, xCenter - xSolution2Previous + yCenter, j - yCenter + xCenter, xCenter - xSolution + yCenter, lineColour, internalThickness, topOrBottom, otherColour, externalThickness);
            addLineP2P(yCenter - j+2 + xCenter, xSolution2Previous - xCenter + yCenter, yCenter - j + xCenter, xSolution - xCenter + yCenter, lineColour, internalThickness, topOrBottom, otherColour, externalThickness);

            xSolution2Previous = xSolution;
        }    
        if(fill){
            if(j>yCenter - radius + thickness){
                shrinkLine(xSolution + thickness+1, j, 2*xCenter - xSolution - thickness-1, j, 0, coordinates, internalLength);
                addLineInternal(coordinates, internalLength, 1, SCREEN_SIZE, internalLength+1, internalLength+1, otherColour, topOrBottom);
                
                shrinkLine(xSolution + thickness+1, 2*yCenter - j, 2*xCenter - xSolution - thickness-1, 2*yCenter - j, 0, coordinates, internalLength);
                addLineInternal(coordinates, internalLength, 1, SCREEN_SIZE, internalLength+1, internalLength+1, otherColour, topOrBottom);
                
            }
            if(j<=yCenter - radius/2 - thickness){
                for(uint8_t i=xSolution;i<=xSolutionPrevious;i++){    
                    shrinkLine(j-1 - yCenter + xCenter + thickness, i - xCenter + yCenter, yCenter - j+1 + xCenter - thickness, i - xCenter + yCenter, 0, coordinates, internalLength);
                    addLineInternal(coordinates, internalLength, 1, SCREEN_SIZE, internalLength+1, internalLength+1, otherColour, topOrBottom);
                
                    shrinkLine(j-1 - yCenter + xCenter + thickness, xCenter - i + yCenter, yCenter - j+1 + xCenter - thickness, xCenter - i + yCenter, 0, coordinates, internalLength);
                    addLineInternal(coordinates, internalLength, 1, SCREEN_SIZE, internalLength+1, internalLength+1, otherColour, topOrBottom);
                }
            }
        }
        xSolutionPrevious=xSolution;
    }
    return error;
}

void SSD1351::calculateLineParameters(int16_t xDelta, int16_t yDelta, uint16_t hypotenuseSquared, int8_t &tangentialDelta, int8_t &transversalDelta){
    // Divides all angles into 3 sets:  {0<x<CRITICAL_ANGLE_MIN}, {CRITICAL_ANGLE_MIN<x<CRITICAL_ANGLE_MAX}
    // and {CRITICAL_ANGLE_MAX<x<90}. All angles that are not between 0 and 90 are rotated to fit into that
    // interval. Each set is then approximated by either an horizontal, vertical or diagonal line.
    if(abs(yDelta)>abs(xDelta)){
        transversalDelta = copysign(1, xDelta);
        tangentialDelta = copysign(SCREEN_SIZE, yDelta);
    }
    else{
        transversalDelta = copysign(SCREEN_SIZE, yDelta);
        tangentialDelta = copysign(1, xDelta);
    }
    if(pow(yDelta, 2)>pow(sin(CRITICAL_ANGLE_MIN*M_PI/180), 2)*hypotenuseSquared && pow(yDelta, 2)<pow(sin(CRITICAL_ANGLE_MAX*M_PI/180), 2)*hypotenuseSquared){
        tangentialDelta+=transversalDelta;
        transversalDelta= -transversalDelta;
    }
}

void SSD1351::calculateLineParameters(uint8_t xStarting, uint8_t yStarting, uint8_t xEnd, uint8_t yEnd, int8_t &counter, int8_t &counter2, int8_t &tangentialDelta, int8_t &transversalDelta){
    // Divides all angles into 3 sets:  {0<x<CRITICAL_ANGLE_MIN}, {CRITICAL_ANGLE_MIN<x<CRITICAL_ANGLE_MAX}
    // and {CRITICAL_ANGLE_MAX<x<90}. All angles that are not between 0 and 90 are rotated to fit into that
    // interval. Each set is then approximated by either an horizontal, vertical or diagonal line.
    // counter and counter2 are two orders of corrections for this approximation, representing how many pixels
    // are necessary before there is an entire pixel of difference between the previous order of 
    // approximation and the exact solution.
    int16_t xDelta = xEnd - xStarting;
    int16_t yDelta = yEnd - yStarting;
    uint16_t hypotenuseSquared = pow(xDelta, 2) + pow(yDelta, 2);
    uint8_t length = max(abs(xDelta), abs(yDelta)) + 1;
    calculateLineParameters(xDelta, yDelta, hypotenuseSquared, tangentialDelta, transversalDelta);
    if(pow(yDelta, 2)>pow(sin(CRITICAL_ANGLE_MIN*M_PI/180), 2)*hypotenuseSquared && pow(yDelta, 2)<pow(sin(CRITICAL_ANGLE_MAX*M_PI/180), 2)*hypotenuseSquared){
        if(abs(xDelta)==abs(yDelta)){
            counter = length+1;
            counter2 = length+1;
        }
        else {
            if(abs(xDelta)>abs(yDelta)){
                // ∆y = y2 - y1 = ml - l => l/∆y = 1/(m-1)
                counter =    lround(    1.0/(    fabs( ( (float)(yDelta)   )/xDelta ) - 1     ) );
                float denominator = fabs( ( (float)(yDelta)   )/xDelta )  -  ( 1.0  + 1.0/counter );
                if(abs(denominator)>1.0/length){
                    counter2 = lround( 1.0/denominator );
                }
                else{
                    counter2 = length+1;
                }
            }
            else{
                // ∆x = x2 - x1 = l/m - l => l/∆y = 1/(1/m - 1) 
                counter =    lround(    1.0/(  fabs( ( (float)(xDelta)   )/yDelta  )  - 1 )    );
                float denominator = fabs( ( (float)(xDelta)   )/yDelta )  -  ( 1.0  + 1.0/counter );
                if(abs(denominator)>1.0/length){
                    counter2 = lround( 1.0/denominator );
                }
                else{
                    counter2 = length+1;
                }
            }
        }
    }
    else if(abs(xDelta)>abs(yDelta)){
        if(yDelta){
            counter =    lround(  ( (float) xDelta )/yDelta  );
            float denominator = (  (float)(yDelta)   )/xDelta     - 1.0/counter  ;
            if(abs(denominator)>1.0/length){
                counter2 =   lround( 1.0/denominator );
            }
            else{
                counter2 = length+1;
            }
        }
        else{
            counter = length+1;
            counter2 = length+1;
        }
    }
    else{
        if(xDelta){
            counter =    lround(  ( (float) yDelta )/xDelta  );
            float denominator = (  (float)(xDelta)   )/yDelta     - 1.0/counter  ;
            if(abs(denominator)>1.0/length){
                counter2 =   lround( 1.0/denominator );
            }
            else{
                counter2 = length+1;
            }
        }
        else{
            counter = length+1;
            counter2 = length+1;
        }
    }
}

Error SSD1351::shrinkLine(int16_t xStarting, int16_t yStarting, int16_t xEnd, int16_t yEnd, float coefficient, int16_t &coordinates, uint8_t &internalLength){
    Error error=shrinkLine(xStarting, yStarting, xEnd, yEnd, coefficient, internalLength);
    coordinates = yStarting*SCREEN_SIZE + xStarting;
    boundaryCheck(xStarting, yStarting, xEnd - xStarting, yEnd - yStarting);
    return error;
}

Error SSD1351::shrinkLine(int16_t &xStarting, int16_t &yStarting, int16_t &xEnd, int16_t &yEnd, float coefficient, uint8_t &internalLength){
    // This function does nothing more than check where the line exits the screen, calculate the coordinates of its
    // last valid point, and substitute it for either the Starting or End coordinates.
    Error error=boundaryCheck(xStarting, yStarting, xEnd - xStarting, yEnd - yStarting);
    if(error&OUT_OF_LEFT_BORDER){
        if(xEnd<0 && xStarting<0){
            internalLength=0;
            return error;
        }
        if(xStarting<0){
            yStarting-=lround(coefficient*xStarting);
            xStarting=0;
        }
        else{
            yEnd-=lround(coefficient*xEnd);
            xEnd=0;            
        }
    }
    if(error&OUT_OF_RIGHT_BORDER){
        if(xStarting>=SCREEN_SIZE && xEnd>=SCREEN_SIZE){
            internalLength=0;
            return error;
        }
        if(xEnd>=SCREEN_SIZE){
            yEnd-=lround(coefficient*(xEnd - SCREEN_SIZE + 1));
            xEnd= SCREEN_SIZE - 1;
        }
        else{
            yStarting-=lround(coefficient*(xStarting - SCREEN_SIZE + 1));
            xStarting= SCREEN_SIZE - 1;            
        }
    }
    if(error&OUT_OF_TOP_BORDER){
        if(yStarting<0 && yEnd<0){
            internalLength=0;
            return error;
        }
        if(yEnd<0){
            xEnd-=lround(((float) yEnd)/coefficient);
            yEnd=0;
        }
        else if(yStarting<0){
            xStarting-=lround(((float) yStarting)/coefficient);
            yStarting=0;
        }
    }
    if(error&OUT_OF_BOTTOM_BORDER){
        if(yStarting>=SCREEN_SIZE && yEnd>=SCREEN_SIZE){
            internalLength=0;
            return error;
        }
        if(yEnd>=SCREEN_SIZE - 1){
        xEnd-=lround(((float) (yEnd-SCREEN_SIZE + 1))/coefficient);
        yEnd=SCREEN_SIZE - 1;
        }
        else if(yStarting>=SCREEN_SIZE){
            xStarting-=lround(((float) (yStarting-SCREEN_SIZE + 1))/coefficient);
            yStarting=SCREEN_SIZE - 1;
        }
    }
    internalLength = max(abs(yEnd - yStarting), abs(xEnd - xStarting)) + 1;
    return error;
}



Error SSD1351::calculateCircleParameters(uint8_t xCenter, uint8_t yCenter, uint8_t thickness, uint8_t radius, uint8_t &x1, uint8_t &x2, uint8_t &y1, uint8_t &y2){
    // Simple boundary check and search for the extremes that the calling function will have to cycle through.
    Error error = boundaryCheck(xCenter - radius - thickness - 1, yCenter - radius - thickness - 1, 2*radius, 2*(radius + thickness));
    if(error&OUT_OF_TOP_BORDER){y1=thickness + 1;}
    else{y1= yCenter - radius;}
    if(error&OUT_OF_BOTTOM_BORDER){y2=SCREEN_SIZE - thickness - 1;}
    else{y2= yCenter + radius;}
    if(error&OUT_OF_LEFT_BORDER){x1=thickness + 1;}
    else{x1= xCenter - radius;}
    if(error&OUT_OF_RIGHT_BORDER){x2=SCREEN_SIZE - thickness - 1;}
    else{x2= xCenter + radius;}
    return error;
}



Error SSD1351::addText(uint8_t xPosition, uint8_t yPosition, char* text, uint16_t textLength, bool topOrBottom, TextProperties_t textProperties, uint8_t rightEdge, uint8_t bottomEdge){
    //TODO Check what happens if the text consists of just '\n' characters.
    // Finds the '\n' characters and creates a list of their positions, checks when the text goes beyond
    // horizontal border, goes back in the text until it doesn't and adds a new '\n' in the list.
    // Calls on addTextInternal for the actual writing to activeScreenBuffer.
    uint16_t *lines = new uint16_t[textLength];
    fill(lines, lines + textLength, textLength + 1);
    lines[0] = 0;
    uint16_t counter = 1;
    for(uint16_t i=0;i<textLength;i++){
        if(text[i] == 0){break;}
        if(text[i] == '\n'){
            lines[counter] = i+1;
            counter++;
            if(i<textLength-1 && text[i+1]=='\r'){
                i++;
            }
        }
        else if(text[i] == '\r'){
            lines[counter] = i+1;
            counter++;
            if(i<textLength-1 && text[i+1]=='\n'){
                i++;
            }
        }
    }
    Error error = NO_ERROR;
    for(uint16_t i=0;i<counter;i++){
        if(lines[i+1] - lines[i] - 1==0){continue;}
        uint16_t *textSpace = calculateTextSpace(text + lines[i], lines[i+1] - lines[i] - 1, textProperties);
        Error tempError = boundaryCheck(xPosition, yPosition + fnt::fontSizes[textProperties.index] * i, textSpace[0] + SCREEN_SIZE - rightEdge, textSpace[1] + SCREEN_SIZE - bottomEdge);
        delete[] textSpace;
        if(tempError&OUT_ALL_BUT_RIGHT){
            error=(Error) (error|tempError);
            break;
        }
        if(tempError==OUT_OF_RIGHT_BORDER){
            for(uint16_t j=lines[i+1]-1;j>lines[i];j--){
                if(text[j]==' '){
                    textSpace = calculateTextSpace(text + lines[i], j - lines[i], textProperties);
                    tempError = boundaryCheck(xPosition, yPosition + fnt::fontSizes[textProperties.index] * i, textSpace[0] + SCREEN_SIZE - rightEdge, textSpace[1] + SCREEN_SIZE - bottomEdge);
                    delete[] textSpace;
                    if(tempError==NO_ERROR){
                        for(uint16_t k=counter;k>i+1;k--){
                            lines[k] = lines[k-1];
                        }
                        lines[i+1] = j+1;
                        counter++;
                        break;
                    }
                }
            }
        }
        error= (Error) (error|tempError);
        if(tempError&OUT_ALL_BUT_RIGHT){
            break;
        }

        addTextInternal(xPosition, yPosition + fnt::fontSizes[textProperties.index] * i, text + lines[i], lines[i+1] - lines[i] - 1, topOrBottom, textProperties);
        
    }
    delete[] lines;
    return error;
}

void SSD1351::addTextInternal(uint8_t xPosition, uint8_t yPosition, char* text, uint16_t textLength, bool topOrBottom, TextProperties_t textProperties){
    // Reads from the fontDatabase each character in text and adds it to activeScreenBuffer.
    if(textLength==0){return;}
    Font* currentFont = &fnt::fontDatabase[textProperties.index];
    uint8_t currentX = xPosition;
    uint8_t currentY = yPosition;
    uint8_t tabLength = (currentFont->width[0] + 1) * TAB_SPACES;
    for(uint16_t i=0;i<textLength;i++){
        if(text[i]==0){break;}
        if(text[i]=='\t' + FONT_OFFSET){
            currentX+= tabLength - (currentX - xPosition)%tabLength;
            continue;
        }
        if(text[i]<FONT_OFFSET || text[i]>NUMBER_OF_CHARACTERS + FONT_OFFSET){continue;}
        uint8_t currentIndex = text[i] - FONT_OFFSET;
        for(uint8_t j=0;j<currentFont->height[currentIndex];j++){
            for(uint8_t k=0;k<currentFont->width[currentIndex];k++){
                if(topOrBottom || !(*activeScreenBuffer)[currentX + currentFont->xPosition[currentIndex] + k + (currentY + currentFont->yPosition[currentIndex] + j)*SCREEN_SIZE]){ 
                    (*activeScreenBuffer)[currentX + currentFont->xPosition[currentIndex] + k + (currentY + currentFont->yPosition[currentIndex] + j)*SCREEN_SIZE] = 
                    textProperties.colour * (currentFont->alphabet[currentIndex][k + j*currentFont->width[currentIndex]]);
                }
            }
        }
        // Adds one pixel between each letter.
        currentX+= currentFont->width[currentIndex] + 1;
    }
}

uint16_t* SSD1351::calculateTextSpace(char *text, uint16_t textLength, TextProperties_t textProperties){
    // Adds up all the width member values from the fontDatabase corresponding to the current text index.
    uint16_t resultX=0;
    uint8_t currentIndex;
    uint8_t tabLength = (fnt::fontDatabase[textProperties.index].width[0] + 1) * TAB_SPACES;
    for(uint16_t i=0;i<textLength; i++){
        if(text[i]==0){break;}
        if(text[i]=='\t' + FONT_OFFSET){
            resultX+= tabLength - resultX%tabLength;
            continue;
        }
        if(text[i]<FONT_OFFSET || text[i]>NUMBER_OF_CHARACTERS + FONT_OFFSET){continue;}
        currentIndex = text[i] - FONT_OFFSET;
        // Adds one pixel between each letter    
        resultX+=(fnt::fontDatabase[textProperties.index]).width[currentIndex] + 1;
    }
    uint16_t *result = new uint16_t[2];
    result[1] = fnt::fontSizes[textProperties.index];
    return result; 
}


int SSD1351::write(Command command){
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


