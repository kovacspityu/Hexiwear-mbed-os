#ifndef _SSD1351_
#define _SSD1351_

#include "SSD1351_enum.h"
#include "SSD1351_Fonts.h"
#include "SSD1351_Fonts_enum.h"
#include "mbed.h"


#define X_OFFSET 16
#define Y_OFFSET 0
#define CRITICAL_ANGLE_MIN 30
#define CRITICAL_ANGLE_MAX 60
#define DEFAULT_TEXT_INDEX ROBOTO11
#define DEFAULT_TEXT_COLOUR 0x861

extern FontDatabase fontDatabase;
extern uint16_t fontSizes[];

class SSD1351{
    public:
        SSD1351();
        ~SSD1351();

        struct TextProperties_t{
            FontIndex index;
            uint16_t colour;
            TextProperties_t(FontIndex i, uint16_t colour){
                this->index=i;
                this->colour=colour;
            }
            TextProperties_t(){
                TextProperties_t(DEFAULT_TEXT_INDEX, DEFAULT_TEXT_COLOUR);
            }
        };

        void powerUp();
        void powerOff();
        void reset();
        void sleep();
        void wakeUp();

        void setBrightness(uint8_t brightness);
        void setColourBrightness(uint8_t red, uint8_t green, uint8_t blue);

        void draw(bool keepScreen = false);
        void drawAndKeep();
        void drawAndClear();
        void clearScreen();
        SSD_Error addText(uint8_t xPosition, uint8_t yPosition, char* text, uint16_t textLength, bool topOrBottom, TextProperties_t textProperties);
        SSD_Error addBox(uint8_t xPosition, uint8_t yPosition, uint8_t width, uint8_t height, uint16_t internalColour, uint8_t internalThickness, bool topOrBottom, uint16_t externalColour = 0, uint8_t externalThickness = 0);
        SSD_Error addTextInBox(char* text, uint16_t textLength, TextProperties_t textProperties, uint8_t xPosition, uint8_t yPosition, uint8_t width, uint8_t height, uint16_t internalColour, uint8_t internalThickness, bool topOrBottom, uint16_t externalColour = 0, uint8_t externalThickness = 0)
        SSD_Error addLine(uint8_t xPosition, uint8_t yPosition, uint8_t length, uint16_t angle, uint16_t internalColour, uint8_t internalThickness, bool topOrBottom, uint16_t externalColour = 0, uint8_t externalThickness = 0);
        SSD_Error addLineOnTop(uint8_t xPosition, uint8_t yPosition, uint8_t length, uint16_t angle, uint16_t internalColour, uint8_t internalThickness, uint16_t externalColour, uint8_t externalThickness);
        SSD_Error addLineAtBottom(uint8_t xPosition, uint8_t yPosition, uint8_t length, uint16_t angle, uint16_t internalColour, uint8_t internalThickness, uint16_t externalColour, uint8_t externalThickness);
        SSD_Error addImage(uint16_t *image, uint8_t xPosition, uint8_t yPosition, uint8_t width, uint8_t height, bool topOrBottom);
        SSD_Error addImageOnTop(uint16_t *image, uint8_t xPosition, uint8_t yPosition, uint8_t width, uint8_t height);
        SSD_Error addImageAtBottom(uint16_t *image, uint8_t xPosition, uint8_t yPosition, uint8_t width, uint8_t height);
        SSD_Error fillArea(uint8_t xPosition, uint8_t yPosition, uint8_t width, uint8_t height, uint16_t colour, bool topOrBottom);

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


        static const TextProperties_t defaultTextProperties;
        static uint8_t xStartActive, xEndActive, yStartActive, yEndActive, xStartDrawing, xEndDrawing, yStartDrawing, yEndDrawing;
        uint16_t **activeScreenBuffer, **drawingScreenBuffer, *screenBuffer1, *screenBuffer2;

        void lock();
        void unlock();
        void lockCommands();
        void unlockCommands();
        void setChargePeriods(uint8_t periods); // Possibly it should be public, but some care needs to go into it.
        

        void workerDraw();

        SSD_Error boundaryCheck(uint8_t xPosition, uint8_t yPosition, int16_t deltaX, int16_t deltaY);
        void addLineInternal(int16_t coordinates, uint8_t length, int8_t transversalDelta, int8_t tangentialDelta, int8_t counter, int8_t counter2, uint16_t colour, bool topOrBottom);
        void calculateLineParameters(const uint16_t angle, const uint8_t length, uint8_t &internalLength, uint8_t &counter, uint8_t &counter2,  int8_t &tangentialDelta, int8_t &transversalDelta);
        SSD_Error addTextInternal(uint8_t xPosition, uint8_t yPosition, char* text, uint16_t textLength, bool topOrBottom, TextProperties_t textProperties);
        uint16_t* calculateTextSpace(char *text, uint16_t textLength, TextProperties_t textProperties);
        

        int write(SSD_Command command);
        int write(uint8_t *data, int lenght = 1);

};







#endif