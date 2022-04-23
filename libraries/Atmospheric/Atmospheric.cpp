#include "Atmospheric.h"
#define SEALEVELPRESSURE_HPA (1013.25)



Atmospheric::Atmospheric() {
    this->tempC = 0.0;
    this->pressure = 0.0;
    this->altitudeM = 0.0;
    this->humidity = 0.0;
}

void Atmospheric::collecting(){
    unsigned status;
    
    // default settings
    status = bme.begin();  
    this->tempC = bme.readTemperature();
    this->pressure =  bme.readPressure() / 100.0F;
    this->altitudeM = bme.readAltitude(SEALEVELPRESSURE_HPA);
    this->humidity = bme.readHumidity();

    delay(1000);
}
