#include "Turbidity.h"

Turbidity::Turbidity(byte pin) {
    this->pin = pin;
    this->voltage = 0.0;
    this->turbidity = 0.0;
}

void Turbidity::collecting(){
    this->voltage = 0;
    for(int i = 0; i < 800; i++){
        this->voltage += ((float)analogRead(this->pin)/1023)*5;
    }
    this->voltage =  this->voltage/800;
    this->voltage = this->voltage;
    if(this->voltage < 2.5){
        Serial.println(this->voltage);
        this->turbidity = 3000;
    }
    else{
        this->turbidity = -1120.4*square(this->voltage)+ 5742.3*this->voltage - 4352.9; 
    }
}

int Turbidity::getTurbidity(){
    return this->turbidity;
}