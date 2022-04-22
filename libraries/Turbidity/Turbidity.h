#ifndef TURBIDITY_H
#define TURBIDITY_H

#include <Arduino.h>

class Turbidity{
    private:
        byte pin;
    public:
        Turbidity(byte pin);
        void collecting();
        void off();
        int getTurbidity();
        float turbidity;
        float voltage;   

};
#endif