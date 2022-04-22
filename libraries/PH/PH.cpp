#include "PH.h"

PH::PH(byte pin) {
    this->pin = pin;
    this->offset = 0.00;
}

void PH::collecting(){
  for(int i = 0; i < 10; i++){
      this->buffer[i] = analogRead(this->pin);
  }
    for(int i = 0; i < 9; i++){
        for(int j = i+1; j< 10; j++){
          if(this->buffer[i]> this->buffer[j]){
              int temp = this->buffer[i];
              this->buffer[i] = this->buffer[j];
              this->buffer[j] = temp;   
          }     
        }
    }
    this->avgValue = 0;
    for(int i =2; i<8;i++){
        this->avgValue += this->buffer[i];
    }
    this->pHValue = (float)this->avgValue*5.0/1024/6;
    this->pHValue = 3.5*this->pHValue+ this->offset;
    delay(1000);
}