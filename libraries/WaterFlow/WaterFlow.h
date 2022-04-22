#ifndef WATERFLOW_H
#define WATERFLOW_H

#include <Arduino.h>


class WaterFlow{

    
    public:
        WaterFlow();
        void initialize();
        void collecting();
        void off();
        float flow;
        float volume;
        volatile uint16_t pulseCount;

};
#endif