#ifndef ATMOSPHERIC_H
#define ATMOSPHERIC_H

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

class Atmospheric{
    private:
        byte pin;
        Adafruit_BME280 bme;
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