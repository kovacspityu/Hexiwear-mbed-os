#ifndef _SENSORS_HANDLER_
#define _SENSORS_HANDLER_

#include "FXAS21002C_enum.h"
#include "FXOS8700CQ_enum.h"
#include "HTU21D_enum.h"
#include "MAX30101_enum.h"
#include "MPL3115A2_enum.h"
#include "TSL2561_enum.h"
#include "SensorConfig.h"

#define MAX_NUMBER_OF_APPS 8

using snsh;


class SensorsHandler{
    public:
        SensorsHandler();
        // Initializes the sensor requested if necessary,
        // sets it up with values within the ones requested if possible, 
        // otherwise returns an error that describes the problem 
        // (or it could return the current available interval).
        // If the sensor's parameters are modified, all apps using it
        // should be notified (or this class should hold all info)
        SensorConfig setSensor(HexiApp *app, SensorConfig config);
        SensorConfig getConfig(snsh::SensorType type);
        // Checks if there are still other apps using this sensor, otherwise deletes it.
        void removeSensor(HexiApp *app, SensorType sensor);   
                                                                    
    private:
        FXAS21002C  *mAccelerometer;
        FXOS8700CQ  *mGyroscope;
        HTU21D      *mHygrometer;
        MAX30101    *mHR;
        MPL3115A2   *mBarometer;

        uint8_t initializedSensors;
        uint16_t currentNumberOfApps;

        void setFXAS21002C(FXA::Config config);   
        void setFXOS8700CQ(FXO::Config config);   
        void setHTU21D(HTU::Config config);
        void setMAX30101(MAX::Config config);
        void setMPL3115A2(MPL::Config config);
        void setTSL2561(TSL::Config config);

        // Series of intervals available for the sensors parameters, basically stores
        // the minimum and maximum value possible based on what the apps have requested so far.
        FXA::Config currentFXAConfig, extremesFXA[2], requestedConfigs[MAX_NUMBER_OF_APPS][2];
        FXO::Config currentFXOConfig, extremesFXO[2], requestedConfigs[MAX_NUMBER_OF_APPS][2];
        HTU::Config currentHTUConfig, extremesHTU[2], requestedConfigs[MAX_NUMBER_OF_APPS][2];
        MAX::Config currentMAXConfig, extremesMAX[2], requestedConfigs[MAX_NUMBER_OF_APPS][2];
        MPL::Config currentMPLConfig, extremesMPL[2], requestedConfigs[MAX_NUMBER_OF_APPS][2];
        TSL::Config currentTSLConfig, extremesTSL[2], requestedConfigs[MAX_NUMBER_OF_APPS][2];
        //Arrays of pointers to all the apps running, to keep just the right configs.
        HexiApp mApps[MAX_NUMBER_OF_APPS];
};









#endif