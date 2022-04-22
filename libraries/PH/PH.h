#ifndef PH_H
#define PH_H

#include <Arduino.h>

class PH{
    private:
        byte pin;
        float avgValue;
        float offset;
        int buffer[10];
    
    public:
        PH(byte pin);
        void init();
        void collecting();
        void off();
        float pHValue;

};
#endif