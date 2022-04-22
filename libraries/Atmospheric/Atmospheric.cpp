#include "Atmospheric.h"

Atmospheric::Atmospheric() {
    this->sampleNumber = 0;
    this->tempC = 0.0;
    this->pressure = 0.0;
    this->altitudeM = 0.0;
    this->humidity = 0.0;
}

void Atmospheric::collecting(){
    /**
    BME280 atmosphericSensor;
    if (atmosphericSensor.beginI2C() == false) //Begin communication over I2C
    {
        Serial.println("The sensor did not respond. Please check wiring.");
    }
    **/
    this->tempC = atmosphericSensor.readTempC();
    this->pressure = atmosphericSensor.readFloatPressure();
    this->altitudeM = atmosphericSensor.readFloatAltitudeMeters();
    this->humidity = atmosphericSensor.readFloatHumidity();

    delay(1000);
}
