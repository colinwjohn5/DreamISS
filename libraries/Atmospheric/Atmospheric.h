#ifndef ATMOSPHERIC_H
#define ATMOSPHERIC_H

#include <Arduino.h>
#include "SparkFunBME280.h"

class Atmospheric{
    private:
        byte pin;
        BME280 atmosphericSensor;
        unsigned long sampleNumber;
    
    public:
        Atmospheric();
        void init();
        void collecting();
        void off();
        float tempC;
        float pressure;
        float humidity;
        float altitudeM;

};
#endif