#ifndef _SSD1351_
#define _SSD1351_

#include "SSD1351_enum.h"
#include "mbed.h"

class SSD1351{
    public:
        SSD1351();
        ~SSD1351();

        void powerUp();
        void powerOff();
        void reset();
        void sleep();
        void wakeUp();

        void setBrightness(uint8_t brightness);
        void setColourBrightness(uint8_t red, uint8_t green, uint8_t blue);

        void draw();
        void clearScreen();
        void addLine(uint8_t xPosition, uint8_t yPosition, uint8_t length, float angle, uint16_t internalColour, uint8_t internalThickness, bool topOrBottom, uint16_t externalColour = 0, uint8_t externalThickness = 0);
        void addLineOnTop(uint8_t xPosition, uint8_t yPosition, uint8_t length, float angle, uint16_t internalColour, uint8_t internalThickness, uint16_t externalColour, uint8_t externalThickness);
        void addLineAtBottom(uint8_t xPosition, uint8_t yPosition, uint8_t length, float angle, uint16_t internalColour, uint8_t internalThickness, uint16_t externalColour, uint8_t externalThickness);
        SSD_Error addImage(uint16_t *image, uint8_t xPosition, uint8_t yPosition, uint8_t width, uint8_t height, bool topOrBottom);
        SSD_Error addImageOnTop(uint16_t *image, uint8_t xPosition, uint8_t yPosition, uint8_t width, uint8_t height);
        SSD_Error addImageAtBottom(uint16_t *image, uint8_t xPosition, uint8_t yPosition, uint8_t width, uint8_t height);

        void scrollingOn(bool direction, uint8_t startingRow, uint8_t numberOfRows, SSD_Scrolling_Period period);
        void scrollingOff();
        
        static uint16_t getColour(uint8_t red, uint8_t green, uint8_t blue);
        
    private:
        SPI mSPI;
        DigitalOut dataORcommand;
        DigitalOut mPower;
        DigitalOut resetPin, chipSelect;

        Thread mThread;
        Mutex mMutex;

        static uint8_t xStartActive, xEndActive, yStartActive, yEndActive, xStartDrawing, xEndDrawing, yStartDrawing, yEndDrawing;
        uint16_t **activeScreenBuffer, **drawingScreenBuffer, *screenBuffer1, *screenBuffer2;

        void lock();
        void unlock();
        void lockCommands();
        void unlockCommands();
        void setChargePeriods(uint8_t periods); // Possibly it should be public, but some care needs to go into it.
        

        void workerDraw();

        SSD_Error boundaryCheck(uint8_t xPosition, uint8_t yPosition, int16_t deltaX, int16_t deltaY);
        

        int write(SSD_Command command);
        int write(uint8_t *data, int lenght = 1);

};







#endif