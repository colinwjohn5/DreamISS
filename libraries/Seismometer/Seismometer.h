#ifndef SEISMOMETER_H
#define SEISMOMETER_H

#include <Arduino.h>

class Seismometer{
    private:
        byte pin;
    
    public:
        Seismometer(byte pin);
        void collecting();
        float roll;
        float pitch;
        float yaw;
};
#endif