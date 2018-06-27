#ifndef _SENSOR_
#define _SENSOR_
#include "SensorConfig.h"

class Sensor{
    public:
        virtual SensorConfig config(SensorConfig config[2]);
};

#endif