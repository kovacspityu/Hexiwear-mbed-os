#ifndef _SENSOR_CONFIG_
#define _SENSOR_CONFIG_

#include "SensorHandler_enum.h"
#include "FXAS21002C_enum.h"
#include "FXOS8700CQ_enum.h"
#include "HTU21D_enum.h"
#include "MAX30101_enum.h"
#include "MPL3115A2_enum.h"
#include "TSL2561_enum.h"


struct SensorConfig{
    public: 
        SensorConfig();
        snsh::SensorType sensor;

};

namespace FXA{
    struct Config : public SensorConfig{
        Config(){this.sensor= FXAS21002C_T;}
        Config(FXA::Range range, FXA::ODR dataRate, FXA::Low lowThreshold, FXA::High highThreshold){
            this.range          = range;
            this.dataRate       = dataRate;
            this.lowThreshold   = lowThreshold;
            this.highThreshold  = highThreshold;
            Config();
        }
        /*Those same parameters as members*/
        FXA::Range  range;
        FXA::ODR    dataRate;
        FXA::Low    lowThreshold;
        FXA::High   highThreshold;
        //Need the parameters for interrupts, need to think a smart way to do them.
    }
}

namespace FXO{
    struct Config : public SensorConfig{
        Config(){this.sensor= FXOS8700CQ_T;}
        Config(FXO::Mode mode, FXO::Range range, FXO::ODR awakeODR, FXO::ODR asleepODR, FXO::High threshold){
            this.mode       = mode;
            this.range      = range;
            this.awakeODR   = awakeODR;
            this.asleepODR  = asleepODR;
            this.threshold  = threshold;
            Config();
        }
        /*Those same parameters as members*/
        FXO::Mode mode;
        FXO::Range range;
        FXO::ODR awakeODR;
        FXO::ODR asleepODR;
        FXO::High threshold
    }
}

namespace HTU{
    struct Config : public SensorConfig{
        Config(){this.sensor= HTU21D_T;}
        Config(HTU::Resolution resolution){
            this.resolution = resolution;
            Config();
        }
        /*Those same parameters as members*/
        HTU::Resolution resolution;
    }
}

namespace MAX{
    struct Config : public SensorConfig{
        Config(){this.sensor= MAX30101_T;}
        Config( MAX::Mode mode = MAX::HR_MODE, MAX::Oversample oversample = MAX::OVERSAMPLE_1,
                bool fifoRollover = false, uint8_t fifoThreshold = 0, MAX::Led slot1 = MAX::RED_LED, 
                MAX::Led slot2 = MAX::NONE_LED, MAX::Led slot3 = MAX::NONE_LED, MAX::Led slot4 = MAX::NONE_LED,
                uint8_t redAmplitude, uint8_t irAmplitude, uint8_t greenAmplitude = 0, uint8_t pilotAmplitude = 0,
                MAX::Oxygen_Rate rate, MAX::Oxygen_Range range){
                    this.mode = mode;
                    this.oversample = oversample;
                    this.fifoRollover = fifoRollover;
                    this.fifoThreshold = fifoThr;
                    slot[0] = slot1;
                    slot[1] = slot2;
                    slot[2] = slot3;
                    slot[3] = slot4;
                    amplitude[0] = redAmplitude;
                    amplitude[1] = irAmplitude;
                    amplitude[2] = greenAmplitude;
                    amplitude[3] = pilotAmplitude;
                    this.rate = rate;
                    this.range = range;
                    Config();
        }
        /*Those same parameters as members*/
        MAX::Mode mode;
        MAX::Oversample oversample;
        bool fifoRollover;
        uint8_t fifoThreshold;
        MAX::Led slot[4];
        uint8_t amplitude[4];
        MAX::Oxygen_Rate rate;
        MAX::Oxygen_Range range;
    }
}

namespace MPL{
    struct Config : public SensorConfig{
        Config(){this.sensor= MPL3115A2_T;}
        Config( MPL::Mode mode = MPL::BAROMETER, MPL::Os_Ratio ratio = MPL::OS_512_MS, 
                MPL::Time_Step timeStep = MPL::TS_1S){
                    this.mode       = mode;
                    this.ratio      = ratio;
                    this.timeStep   = timeStep;
                    Config();
        }
        /*Those same parameters as members*/
        MPL::Mode mode;
        MPL::Os_Ratio ratio;
        MPL::Time_Step timeStep;
    }
} 

namespace TSL{
    struct Config : public SensorConfig{
        Config(){this.sensor= TSL2561_T;}
        Config(TSL::Gain gain = TSL::LOW_GAIN, TSL::Os_Rate rate = TSL::OS_400MS{
            this.gain = gain;
            this.rate = rate;
            Config();
        }
        /*Those same parameters as members*/
        TSL::Gain gain;
        TSL::Os_Rate rate;
    }
}


#endif