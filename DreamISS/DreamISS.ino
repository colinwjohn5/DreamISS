#include <PH.h>
#include <Turbidity.h>
#include <Atmospheric.h>
#include <WaterFlow.h>


#include "Wire.h"
#include <Arduino.h>
#include <ArduinoJson.h>
#include <Seismometer.h>

#define ARDUINOJSON_ENABLE_PROGMEM 1

#include <Adafruit_SleepyDog.h>

const int MPU = 0x68; 
bool isProd = false;

//String testCommands[6] = {"1_STAMP_ALL_ON_1","1_STAMP_SEISM_ON_1", "1_STAMP_PH_ON_1", "1_STAMP_WFLOW_ON_1", "1_STAMP_TRBID_ON_1","1_STAMP_SAFE_ON_10"};

 
Turbidity turb(A2);
Atmospheric atmos;
WaterFlow waterFlow;
PH ph(PIN0);
Seismometer seismo(A5);
DynamicJsonDocument turb_doc(50);
DynamicJsonDocument atmosphere_doc(100);
DynamicJsonDocument pH_doc(50);
DynamicJsonDocument waterFlow_doc(50);
DynamicJsonDocument seismic_doc(50);



void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Wire.begin();
  Wire.beginTransmission(MPU);       // Start communication with MPU6050 // MPU=0x68
  Wire.write(0x6B);                  // Talk to the register 6B
  Wire.write(0x00);                  // Make reset - place a 0 into the 6B register
  Wire.endTransmission(true); 
  waterFlow.initialize();
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH); // Show we're awake


}

void call_turbidity(){
  turb.collecting();
  turb_doc["Sensor"] = F("TRBID");
  turb_doc["Time"] = millis();
  turb_doc["Val"] = turb.turbidity;
  serializeJson(turb_doc, Serial);
  Serial.println("\n");
}
void call_waterFlow(){
  waterFlow.collecting();
  waterFlow_doc["Sensor"] = F("WFLOW");
  waterFlow_doc["Time"] = millis();
  waterFlow_doc["Val"] = waterFlow.flow;
  serializeJson(waterFlow_doc, Serial);
  Serial.println("\n");
}

void call_seismic(){
  
  seismo.collecting();
  seismic_doc["Sensor"] = F("SEISM");
  seismic_doc["Time"] = (int)(millis()/1000);
  seismic_doc["Val"] = seismo.seismicReading;
  serializeJson(seismic_doc, Serial);
  Serial.println("\n");
}

void call_ph(){
  ph.collecting();
  pH_doc["Sensor"] = F("PH014");
  pH_doc["Time"] = millis();
  pH_doc["Val"] = ph.pHValue;
  serializeJson(pH_doc, Serial);
  Serial.println("\n");
}

void call_atmospheric(){
  atmos.collecting();
  atmosphere_doc["SensorP"] = F("PRES");
  atmosphere_doc["TimeP"] = millis();
  atmosphere_doc["ValP"] = atmos.pressure;
  atmosphere_doc["SensorT"] = F("TEMP");
  atmosphere_doc["TimeT"] = millis();
  atmosphere_doc["ValT"] = atmos.tempC;
  atmosphere_doc["SensorH"] = F("HUMD");
  atmosphere_doc["TimeH"] = millis();
  atmosphere_doc["ValH"] = atmos.humidity;
  serializeJson(atmosphere_doc, Serial);
  Serial.println("\n");
}

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void loop() {

    String commandInjest = listenForCommand();
    processCommand("1_STAMP_PH014_ON_1000");
   
}

String listenForCommand(){
  String incomingCommand;
  while(Serial.available()) {
    incomingCommand= Serial.readString();
    break;
  }
  return incomingCommand;
}


void processCommand(String commandInjest){
  String id= getValue(commandInjest,'_',0);
  String timeStamp = getValue(commandInjest,'_',1);
  String sensorName = getValue(commandInjest,'_',2);
  String cmdVal = getValue(commandInjest,'_',3);
  String sampleRate = getValue(commandInjest,'_',4);
  static int runCount = 0;
  String returnStatus = "";
  runCount++;

  long starttime = millis();
  long endtime = starttime;
  while ((endtime - starttime) <= sampleRate.toInt()* 1000) 
  {

  if(sensorName.equals("SEISM")){

    if(cmdVal == "ON"){
      call_seismic();
    }
    else{
      int sleepMS = Watchdog.sleep(sampleRate.toInt()*1000); 
      break;
    }
  }
  else if(sensorName.equals("PH014")){
    if(cmdVal == "ON"){
      call_ph();
    }
    else{
      int sleepMS = Watchdog.sleep(sampleRate.toInt()*1000); 
      break;
    }
  }
  else if(sensorName.equals("ATMOS")){
    if(cmdVal == "ON"){
      call_atmospheric();
    }
    else{
      int sleepMS = Watchdog.sleep(sampleRate.toInt()*1000); 
      break;
    }
  }
  else if(sensorName.equals("TRBID")){
    if(cmdVal == "ON"){
      call_turbidity();
    }
    else{
      int sleepMS = Watchdog.sleep(sampleRate.toInt()*1000); 
      break;
    }
  }
  else if(sensorName.equals("WFLOW")){
    if(cmdVal == "ON"){
      call_waterFlow();
    }
    else{
      int sleepMS = Watchdog.sleep(sampleRate.toInt()*1000); 
      break;
    }
  }
  //TODO ALL ON CASE
  else if(sensorName.equals("ALL")){

    if(cmdVal == "ON"){
      call_seismic();
      call_waterFlow();
      call_turbidity();
      call_atmospheric();
      call_ph();
    }
    //TODO ALL OFF CASE
    else{
      int sleepMS = Watchdog.sleep(1000); 
    }
  
  }
  else if(sensorName.equals("SAFE")){
    if(cmdVal == "ON"){
      digitalWrite(LED_BUILTIN, LOW);
      int sleepMS = Watchdog.sleep(sampleRate.toInt()*1000); 
      digitalWrite(LED_BUILTIN, HIGH);
      
    }
  }
  runCount = runCount+1;
  endtime = millis();
  }


}
