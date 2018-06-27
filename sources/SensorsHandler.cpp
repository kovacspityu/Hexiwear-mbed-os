#include "SensorsHandler_enum.h"
#include "SensorsHandler.h"
#include "Sensor.h"
#include "SensorConfig.h"
#include "HexiApp.h"
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

using namespace snsh;

SensorsHandler::SensorsHandler() : initializedSensors(0), currentNumberOfApps(0){
    currentConfig[FXAS21002C_T] = new FXA::Config();
    currentConfig[FXOS8700CQ_T] = new FXO::Config();
    currentConfig[HTU21D_T] = new HTU::Config();
    currentConfig[MAX30101_T] = new MAX::Config();
    currentConfig[MPL3115A2_T] = new MPL::Config();
    currentConfig[TSL2561_T] = new TSL::Config();

    configExtremes[FXAS21002C_T][0] = new FXA::Config();
    configExtremes[FXAS21002C_T][1] = new FXA::Config();
    configExtremes[FXOS8700CQ_T][0] = new FXO::Config();
    configExtremes[FXOS8700CQ_T][1] = new FXO::Config();
    configExtremes[HTU21D_T][0] = new HTU::Config();
    configExtremes[HTU21D_T][1] = new HTU::Config();
    configExtremes[MAX30101_T][0] = new MAX::Config();
    configExtremes[MAX30101_T][1] = new MAX::Config();
    configExtremes[MPL3115A2_T][0] = new MPL::Config();
    configExtremes[MPL3115A2_T][1] = new MPL::Config();
    configExtremes[TSL2561_T][0] = new TSL::Config();
    configExtremes[TSL2561_T][1] = new TSL::Config();

    for(uint8_t i=0;i<MAX_NUMBER_OF_APPS;i++){
        requestedConfigs[i][FXAS21002C_T][0] = new FXA::Config();
        requestedConfigs[i][FXAS21002C_T][1] = new FXA::Config();
        requestedConfigs[i][FXOS8700CQ_T][0] = new FXO::Config();
        requestedConfigs[i][FXOS8700CQ_T][1] = new FXO::Config();
        requestedConfigs[i][HTU21D_T][0] = new HTU::Config();
        requestedConfigs[i][HTU21D_T][1] = new HTU::Config();
        requestedConfigs[i][MAX30101_T][0] = new MAX::Config();
        requestedConfigs[i][MAX30101_T][1] = new MAX::Config();
        requestedConfigs[i][MPL3115A2_T][0] = new MPL::Config();
        requestedConfigs[i][MPL3115A2_T][1] = new MPL::Config();
        requestedConfigs[i][TSL2561_T][0] = new TSL::Config();
        requestedConfigs[i][TSL2561_T][1] = new TSL::Config();
    }
}

SensorsHandler::~SensorsHandler(){
    delete currentConfig[FXAS21002C_T];
    delete currentConfig[FXOS8700CQ_T];
    delete currentConfig[HTU21D_T];
    delete currentConfig[MAX30101_T];
    delete currentConfig[MPL3115A2_T];
    delete currentConfig[TSL2561_T];

    delete configExtremes[FXAS21002C_T][0];
    delete configExtremes[FXAS21002C_T][1];
    delete configExtremes[FXOS8700CQ_T][0];
    delete configExtremes[FXOS8700CQ_T][1];
    delete configExtremes[HTU21D_T][0];
    delete configExtremes[HTU21D_T][1];
    delete configExtremes[MAX30101_T][0];
    delete configExtremes[MAX30101_T][1];
    delete configExtremes[MPL3115A2_T][0];
    delete configExtremes[MPL3115A2_T][1];
    delete configExtremes[TSL2561_T][0];
    delete configExtremes[TSL2561_T][1];

    for(uint8_t i=0;i<MAX_NUMBER_OF_APPS;i++){
        delete requestedConfigs[i][FXAS21002C_T][0];
        delete requestedConfigs[i][FXAS21002C_T][1];
        delete requestedConfigs[i][FXOS8700CQ_T][0];
        delete requestedConfigs[i][FXOS8700CQ_T][1];
        delete requestedConfigs[i][HTU21D_T][0];
        delete requestedConfigs[i][HTU21D_T][1];
        delete requestedConfigs[i][MAX30101_T][0];
        delete requestedConfigs[i][MAX30101_T][1];
        delete requestedConfigs[i][MPL3115A2_T][0];
        delete requestedConfigs[i][MPL3115A2_T][1];
        delete requestedConfigs[i][TSL2561_T][0];
        delete requestedConfigs[i][TSL2561_T][1];
    }
}

SensorConfig SensorsHandler::setSensor(HexiApp *app, SensorConfig config[2]){
    //TODO Needs to check if the app is already in the list, if it isn't add it,
    //TODO check if the sensor is initizalized, if it isn't do it, in any case update the config
    if(config[1]<config[0]){
        //TODO return some form of error
    }
    uint8_t appIndex=MAX_NUMBER_OF_APPS;
    for(uint8_t i=0;i<MAX_NUMBER_OF_APPS;i++){
        if(app==mApps[1]){
            appIndex=i;
            break;
        }
    }
    if(appIndex==MAX_NUMBER_OF_APPS){
        if(currentNumberOfApps==MAX_NUMBER_OF_APPS){
            //TODO return some form of error
        }
        else{
            appIndex = currentNumberOfApps;
        }
    }
    mApps[appIndex] = app;
    if(!(initializedSensors&(1<<config[0].sensor))){
        switch(config[0].sensor){
            case FXAS21002C_T   :{
                mSensors[config[0].sensor] = new FXAS21002C();
                break;}
            case FXOS8700CQ_T   :{
                mSensors[config[0].sensor] = new FXOS8700CQ();
                break;}
            case HTU21D_T       :{
                mSensors[config[0].sensor] = new HTU21D();
                break;}
            case MAX30101_T     :{
                mSensors[config[0].sensor] = new MAX30101();
                break;}
            case MPL3115A2_T    :{
                mSensors[config[0].sensor] = new MPL3115A2();
                break;}
            case TSL2561_T      :{
                mSensors[config[0].sensor] = new TSL2561();
                break;
            }
            case NO_SENSOR_T    :{
                break;
            }
        }
    }
    *(requestedConfigs[appIndex][config[0].sensor][0]) = config[0];
    *(requestedConfigs[appIndex][config[0].sensor][1]) = config[1];
    updateExtremes(config[0].sensor);
    return mSensors[config[0].sensor]->config(*((configExtremes[config[0].sensor])));
    /*
    if(!(initializedSensors&(1<<config[0].sensor))){
        switch(config[0].sensor){
        // Initialize the sensor following its specific code.
        case FXAS21002C_T   :{
            mGyroscope = new *FXAS21002C();
            break;}
        case FXOS8700CQ_T   :{
            mAccelerometer = new *FXOS8700CQ();
            break;}
        case HTU21D_T       :{
            mHygrometer = new *HTU21D();
            break;}
        case MAX30101_T     :{
            mHR = new *MAX30101();
            break;}
        case MPL3115A2_T    :{
            mBarometer = *new MPL3115A2();
            break;}
        case TSL2561_T      :{
            mPhotometer = *new TSL2561();
            break;
        }
    }
    switch(config[0].sensor){
        // Initialize the sensor following its specific code.
        case FXAS21002C_T   :{
            mGyroscope->config(config);
            break;}
        case FXOS8700CQ_T   :{
            mAccelerometer->config(config);
            break;}
        case HTU21D_T       :{
            mHygrometer->config(config);
            break;}
        case MAX30101_T     :{
            mHR->config(config);
            break;}
        case MPL3115A2_T    :{
            mBarometer->config(config);
            break;}
        case TSL2561_T      :{
            mPhotometer->config(config);
        }
    }
    */
}

SensorConfig SensorsHandler::getConfig(SensorType sensor){
    if(sensor!=NO_SENSOR_T){
    return *(currentConfig[sensor]);
    }
    else{
        SensorConfig config;
        config.sensor=NO_SENSOR_T;
        return config;
    }
}

void SensorsHandler::removeApp(HexiApp *app){
    uint8_t appIndex=findApp(app);
    removeApp(appIndex);
}

bool SensorsHandler::isSensorUsed(SensorType sensor){
    for(uint8_t i=0;i<currentNumberOfApps;i++){
        if((requestedConfigs[i][sensor][0])->sensor==sensor){return true;}
    }
    return false;
}

bool SensorsHandler::isAppActive(HexiApp *app){
    uint8_t appIndex = findApp(app);
    for(uint8_t i=0;i<NUMBER_OF_SENSORS;i++){
        if((requestedConfigs[appIndex][i][0])->sensor>=0){return true;}
    }
    return false;
}

uint8_t SensorsHandler::findApp(HexiApp *app){
    uint8_t appIndex=MAX_NUMBER_OF_APPS;
    for(uint8_t i=0;i<MAX_NUMBER_OF_APPS;i++){
        if(app==mApps[i]){
            appIndex=i;
            break;
        }
    }
    return appIndex;
}


bool SensorsHandler::removeSensor(HexiApp *app, SensorType sensor){
    //TODO Sets the configs that this app requested for this sensor to NO_SENSOR_T, then checks if the sensor
    //TODO and the app have anything left referring to them, otherwise removes them completely.
    uint8_t appIndex=findApp(app);
    if(appIndex<MAX_NUMBER_OF_APPS){
        removeConfig(sensor, appIndex);
        if(!isAppActive(app)){
            removeApp(appIndex);
        }
        if(isSensorUsed(sensor)){
            updateExtremes(sensor);
        }
        else{removeSensor(sensor);}
        return true;
    }
    else{
        //An app that has never requested a sensor is making this request, send an error and return.
        return false;
    }
}

void SensorsHandler::removeApp(uint8_t appIndex){
    for(uint8_t i=appIndex;i<currentNumberOfApps-1;i++){
        *mApps[i]=*mApps[i+1];
    }
    uint8_t dummy = 0;
    for(uint8_t i=0;i<NUMBER_OF_SENSORS;i++){
        if((requestedConfigs[appIndex][i][0])->sensor){dummy|=(1<<((requestedConfigs[appIndex][i][0])->sensor));}
    }
    for(uint8_t i=appIndex;i<currentNumberOfApps-1;i++){
        for(uint8_t j=0; j<NUMBER_OF_SENSORS;j++){
            *(requestedConfigs[i][j][0])=*(requestedConfigs[i+1][j][0]);
            *(requestedConfigs[i][j][1])=*(requestedConfigs[i+1][j][1]);
        }
    }
    currentNumberOfApps--;
    for(uint8_t i=0;dummy && i<NUMBER_OF_SENSORS;i++){
        if(dummy & (1<<i)){
            updateExtremes((SensorType) i);
        }
    }
}

void SensorsHandler::removeConfig(SensorType sensor, uint8_t appIndex){
    (requestedConfigs[appIndex][sensor][0])->sensor = NO_SENSOR_T;
    (requestedConfigs[appIndex][sensor][1])->sensor = NO_SENSOR_T;
}

void SensorsHandler::removeSensor(SensorType sensor){
    delete mSensors[sensor];
    initializedSensors&=~sensor;
}


void SensorsHandler::updateExtremes(SensorType sensor){
    SensorConfig *config;
    switch(sensor){
        case FXAS21002C_T   :{
            config = new FXA::Config[2];
            break;}
        case FXOS8700CQ_T   :{
            config = new FXO::Config[2];
            break;}
        case HTU21D_T       :{
            config = new HTU::Config[2];
            break;}
        case MAX30101_T     :{
            config = new MAX::Config[2];
            break;}
        case MPL3115A2_T    :{
            config = new MPL::Config[2];
            break;}
        case TSL2561_T      :{
            config = new TSL::Config[2];
        }
        case NO_SENSOR_T    :{
            return;
        }
    }
    config[0].sensor=NO_SENSOR_T;
    config[1].sensor=NO_SENSOR_T;
    for(uint8_t i=0;i<currentNumberOfApps-1;i++){
        if((requestedConfigs[i][sensor][0])->sensor==sensor){
            config[0]=*(requestedConfigs[i][sensor][0]);
            config[1]=*(requestedConfigs[i][sensor][1]);
            break;
        }
    }
    if(config[0].sensor==NO_SENSOR_T){
        removeSensor(sensor);
    }
    else{
        for(uint8_t i=1;i<currentNumberOfApps-1;i++){
            if((requestedConfigs[i][sensor][0])->sensor==sensor){
                if(config[0]>*(requestedConfigs[i][sensor][0])){config[0]=*(requestedConfigs[i][sensor][0]);}
                if(config[1]<*(requestedConfigs[i][sensor][1])){config[1]=*(requestedConfigs[i][sensor][1]);}
            }
        }
        *(configExtremes[sensor][0])=config[0];
        *(configExtremes[sensor][1])=config[1];
        *(currentConfig[sensor]) = (mSensors[sensor])->config(config);
    }
    delete[] config;
    /*
    switch(sensor){
        case FXAS21002C_T   :{
            FXA::Config config[2];
            for(uint8_t i=0;i<currentNumberOfApps;i++){
                if(requestedFXAConfigs.parameter<config[0].parameter){config[0].parameter=requestedFXAConfigs.parameter;}
                if(requestedFXAConfigs.parameter>config[1].parameter){config[1].parameter=requestedFXAConfigs.parameter;}
            }
            extremesFXA[0]=config[0];
            extremesFXA[1]=config[1];
            FXA::Config *dummy = mGyroscope->config(config);
            currentFXAConfig = *dummy;
            delete dummy;
            break;
        }
        case FXOS8700CQ_T   :{
            FXO::Config config[2];
            for(uint8_t i=0;i<currentNumberOfApps;i++){
                if(requestedFXOConfigs.parameter<config[0].parameter){config[0].parameter=requestedFXOConfigs.parameter;}
                if(requestedFXOConfigs.parameter>config[1].parameter){config[1].parameter=requestedFXOConfigs.parameter;}
            }
            extremesFXO[0]=config[0];
            extremesFXO[1]=config[1];
            FXO::Config *dummy = mGyroscope->config(config);
            currentFXOConfig = *dummy;
            delete dummy;
            break;
        }
        case HTU21D_T       :{
            HTU::Config config[2];
            for(uint8_t i=0;i<currentNumberOfApps;i++){
                if(requestedHTUConfigs.parameter<config[0].parameter){config[0].parameter=requestedHTUConfigs.parameter;}
                if(requestedHTUConfigs.parameter>config[1].parameter){config[1].parameter=requestedHTUConfigs.parameter;}
            }
            extremesHTU[0]=config[0];
            extremesHTU[1]=config[1];
            HTU::Config *dummy = mGyroscope->config(config);
            currentHTUConfig = *dummy;
            delete dummy;
            break;
        }
        case MAX30101_T     :{
            MAX::Config config[2];
            for(uint8_t i=0;i<currentNumberOfApps;i++){
                if(requestedMAXConfigs.parameter<config[0].parameter){config[0].parameter=requestedMAXConfigs.parameter;}
                if(requestedMAXConfigs.parameter>config[1].parameter){config[1].parameter=requestedMAXConfigs.parameter;}
            }
            extremesMAX[0]=config[0];
            extremesMAX[1]=config[1];
            MAX::Config *dummy = mGyroscope->config(config);
            currentMAXConfig = *dummy;
            delete dummy;
            break;
        }
        case MPL3115A2_T    :{
            MPL::Config config[2];
            for(uint8_t i=0;i<currentNumberOfApps;i++){
                if(requestedMPLConfigs.parameter<config[0].parameter){config[0].parameter=requestedMPLConfigs.parameter;}
                if(requestedMPLConfigs.parameter>config[1].parameter){config[1].parameter=requestedMPLConfigs.parameter;}
            }
            extremesMPL[0]=config[0];
            extremesMPL[1]=config[1];
            MPL::Config *dummy = mGyroscope->config(config);
            currentMPLConfig = *dummy;
            delete dummy;
            break;
        }
        case TSL2561_T      :{
            TSL::Config config[2];
            for(uint8_t i=0;i<currentNumberOfApps;i++){
                if(requestedTSLConfigs.parameter<config[0].parameter){config[0].parameter=requestedTSLConfigs.parameter;}
                if(requestedTSLConfigs.parameter>config[1].parameter){config[1].parameter=requestedTSLConfigs.parameter;}
            }
            extremesTSL[0]=config[0];
            extremesTSL[1]=config[1];
            TSL::Config *dummy = mGyroscope->config(config);
            currentTSLConfig = *dummy;
            delete dummy;
            break;
        }
    }
    */
}