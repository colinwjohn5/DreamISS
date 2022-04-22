 #include "LiquidCrystal.h"
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
   
#include <WaterFlow.h>
// which pin to use for reading the sensor? can use any pin!
#define FLOWSENSORPIN 2

volatile uint16_t pulses = 0;
// track the state of the pulse pin
volatile uint8_t lastflowpinstate;
// you can try to keep time of how long it is between pulses
volatile uint32_t lastflowratetimer = 0;
// and use that to calculate a flow rate
volatile float flowrate;


WaterFlow::WaterFlow(){
    this-> flow = 0.0;
    this-> volume = 0.0; 
    this-> pulseCount = 0.0;
}
// count how many pulses!
// Interrupt is called once a millisecond, looks for any pulses from the sensor!
SIGNAL(TIMER0_COMPA_vect) {
  uint8_t x = digitalRead(FLOWSENSORPIN);
  if (x == lastflowpinstate) {
    lastflowratetimer++;
    return; // nothing changed!
  }
  
  if (x == HIGH) {
    //low to high transition!
    pulses++;
  }
  lastflowpinstate = x;
  flowrate = 1000.0;
  flowrate /= lastflowratetimer;  // in hertz
  lastflowratetimer = 0;
}

void useInterrupt(boolean v) {
  if (v) {
    // Timer0 is already used for millis() - we'll just interrupt somewhere
    // in the middle and call the "Compare A" function above
    OCR0A = 0xAF;
    TIMSK0 |= _BV(OCIE0A);
  } else {
    // do not call the interrupt function COMPA anymore
    TIMSK0 &= ~_BV(OCIE0A);
  }
}

void WaterFlow::initialize() {

  lcd.begin(16, 2);
   
   pinMode(FLOWSENSORPIN, INPUT);
   digitalWrite(FLOWSENSORPIN, HIGH);
   lastflowpinstate = digitalRead(FLOWSENSORPIN);
   useInterrupt(true);
}

void WaterFlow::collecting()                     // run over and over again
{ 

  //lcd.setCursor(0, 0);
  //lcd.print("Pulses:"); lcd.print(pulses, DEC);
  //lcd.print(" Hz:");
  //lcd.print(flowrate);
  float liters = pulses;
   liters /= 7.5;
   liters /= 60.0;
  this->flow = flowrate;
  this->pulseCount = pulses;
  this->volume = liters;
  //lcd.setCursor(0, 1);
  //lcd.print(liters); lcd.print(" Liters        ");
    
  delay(100);
}


