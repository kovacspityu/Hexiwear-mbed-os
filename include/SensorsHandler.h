#ifndef _SENSORS_HANDLER_
#define _SENSORS_HANDLER_

#include "Sensor.h"
#include "SensorsHandler_enum.h"
#include "FXAS21002C_enum.h"
#include "FXAS21002C.h"
#include "FXOS8700CQ_enum.h"
#include "FXOS8700CQ.h"
#include "HTU21D_enum.h"
#include "HTU21D.h"
#include "MAX30101_enum.h"
#include "MAX30101.h"
#include "MPL3115A2_enum.h"
#include "MPL3115A2.h"
#include "TSL2561_enum.h"
#include "TSL2561.h"
#include "SensorConfig.h"
#include "HexiApp.h"

#define MAX_NUMBER_OF_APPS (8)
#define NUMBER_OF_SENSORS (6)



class SensorsHandler{
    public:
        SensorsHandler();
        ~SensorsHandler();
        // Initializes the sensor requested if necessary,
        // sets it up with values within the ones requested if possible, 
        // otherwise returns an error that describes the problem 
        // (or it could return the current available interval).
        // If the sensor's parameters are modified, all apps using it
        // should be notified (or this class should hold all info)
        SensorConfig setSensor(HexiApp *app, SensorConfig config[2]);
        SensorConfig getConfig(snsh::SensorType type);
        // Checks if there are still other apps using this sensor, otherwise deletes it.
        bool removeSensor(HexiApp *app, snsh::SensorType sensor);   
        void removeApp(HexiApp *app);
        bool isSensorUsed(snsh::SensorType sensor);
        bool isAppActive(HexiApp *app);
                                                                    
    private:
        Sensor  *mSensors[NUMBER_OF_SENSORS];
        FXAS21002C  *mAccelerometer;
        FXOS8700CQ  *mGyroscope;
        HTU21D      *mHygrometer;
        MAX30101    *mHR;
        MPL3115A2   *mBarometer;
        TSL2561     *mPhotometer;

        uint8_t initializedSensors;
        uint8_t currentNumberOfApps;

        void removeApp(uint8_t appIndex);
        void removeConfig(snsh::SensorType sensor, uint8_t appIndex);
        void removeSensor(snsh::SensorType sensor);
        void updateExtremes(snsh::SensorType sensor);

        uint8_t findApp(HexiApp *app);

        // Series of intervals available for the sensors parameters, basically stores
        // the minimum and maximum value possible based on what the apps have requested so far.
        SensorConfig *currentConfig[NUMBER_OF_SENSORS], *configExtremes[NUMBER_OF_SENSORS][2], *requestedConfigs[MAX_NUMBER_OF_APPS][NUMBER_OF_SENSORS][2];

        //FXA::Config currentFXAConfig, extremesFXA[2], requestedFXAConfigs[MAX_NUMBER_OF_APPS][2];
        //FXO::Config currentFXOConfig, extremesFXO[2], requestedFXOConfigs[MAX_NUMBER_OF_APPS][2];
        //HTU::Config currentHTUConfig, extremesHTU[2], requestedHTUConfigs[MAX_NUMBER_OF_APPS][2];
        //MAX::Config currentMAXConfig, extremesMAX[2], requestedMAXConfigs[MAX_NUMBER_OF_APPS][2];
        //MPL::Config currentMPLConfig, extremesMPL[2], requestedMPLConfigs[MAX_NUMBER_OF_APPS][2];
        //TSL::Config currentTSLConfig, extremesTSL[2], requestedTSLConfigs[MAX_NUMBER_OF_APPS][2];
        //Arrays of pointers to all the apps running, to keep just the right configs.
        HexiApp *mApps[MAX_NUMBER_OF_APPS];
};









#endif