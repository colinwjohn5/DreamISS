# DreamISS

DreamISS is an arduino project (library and sketch) that allows for sensors to be commanded and sensor data to be sent to and from a websocket. 

## Installation

Follow these installation steps to run DreamISS.
 1. Install Arduino https://www.arduino.cc/en/software
 2. Install Node.js https://nodejs.org/en/download/
     - Confirm installation by running node -v in a command prompt
 3. Install javascript libraries:
     - npm install ws
     - npm install serialport 
     - npm install ReadLineParser
 4. Install Arduino libraries (under arduino IDE manage libraries)
     - Board specific library (i.e install board driver for Arduino uno)
     - Adafruit Sleepy Dog Library 
     - ArduinoJson
     - SparkFun BME280 (can be changed based on whatever sensor is chosen).
     - LiquidCrystal 
 
## Documentation
This is a vertical slice explanation for how sensor data is gathered and processed from one specific sensor.
### Setup
Code shown below utilizes the setup method to initialize I2C, Serial port at 115200 baud and digital readings for Water Flow sensor
```cpp
void setup() {
  // Setup code 115200 baud
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

```
### Commanding
DreamISS waits for a command to be published over serial and returns the full string once read
```cpp
String listenForCommand(){
  String incomingCommand;
  while(Serial.available()) {
    incomingCommand= Serial.readString();
    break;
  }
  return incomingCommand;
}
```
### Command Processing
DreamISS gathers the string and splits it at the given delimeter (in our case (-)) and uses the parts that the suite cares about (sensor_name, cmd_value, duration). Once these are processed it commands the specific sensor (sensor_name) to its ON/OFF (cmd_value) for its specified duration in loop.

```cpp
void processCommand(String commandInjest){
  String id= getValue(commandInjest,'_',0);
  String timeStamp = getValue(commandInjest,'_',1);
  String sensorName = getValue(commandInjest,'_',2);
  String cmdVal = getValue(commandInjest,'_',3);
  String sampleRate = getValue(commandInjest,'_',4);
  static int runCount = 0;
  runCount++;

  long starttime = millis();
  long endtime = starttime;
  while ((endtime - starttime) <= sampleRate.toInt()* 1000) 
  {

  if(sensorName.equals("TRBID")){

    if(cmdVal == "ON"){
      call_turbidity();
    }
    else{
      int sleepMS = Watchdog.sleep(sampleRate.toInt()*1000); 
      break;
    }
  }

``` 

 ### Example sensor
 call_turbidity method exists within the arduino main loop, it gathers the sensor data from the turbidity library turb.collecting(); and setsup the return data as a json document and writes to serial.

```cpp 
void call_turbidity(){
  turb.collecting();
  turb_doc["Sensor"] = F("TRBID");
  turb_doc["Time"] = millis();
  turb_doc["Val"] = turb.turbidity;
  serializeJson(turb_doc, Serial);
  Serial.println("\n");
}
```

### Sensor Algorithms
Each sensor is considered a library by arduino. While this is clunky on setup, it's the only real way to do OOP in arduino. 

### Serial Server
The serial server listens for commands being sent over a websocket, and publishes those commands to serial. The module also listens to data published on serial and then opens a websocket to publish data.
#### Open Websocket
```javascript
var WebSocket = require('ws')


//WEBSOCKET ADDRESS, CHANGEME
var webSocket = new WebSocket("wss:websocket.issgroundstation.com/development/");
const wss = new WebSocket.Server({ port: 8080 });
var incomingCommand = '';

const myArgs = process.argv.slice(2);

webSocket.onopen = function (event) {
    console.log("OPEN");
  };

  wss.on('connection', function connection(ws) {
    ws.on('message', function incoming(message) {
      console.log('received: %s', message);
    });
  
    wss.on("message", function incoming(data) {
        ws.send(data);
    });
  }
  );
```

#### Define data SerialPort and listen for data, send over websocket
```javascript
  const { SerialPort } = require('serialport')
  const { ReadlineParser } = require('@serialport/parser-readline')
  const port = new SerialPort({ path: 'COM3', baudRate: 115200 })
  
  const parser = port.pipe(new ReadlineParser({ delimiter: '\r\n' }))
  
  let bitsArray = []
  parser.on('data', function(data) {
  var bits = data;
  bitsArray.push(bits)
  console.log(bits);
  webSocket.send(bits);
});
```

#### Listen for commands and publish to serial

```javascript
    webSocket.addEventListener('message', function (event) {
      var commandObject = JSON.parse(event.data);
      incomingCommand = commandObject.Message;
      if(incomingCommand != undefined){
        port.write(incomingCommand);
      }
      
  });

```



## Future Changes
For next semester, I've included a list of design decisions I would change.
1. Don't publish and listen to data over Serial (creates a race condition) invest in Arduino hardware (either bluetooth or wifi adapter) to publish directly to websocket.
2. In the same theme, have two channels for commanding and publishing data
3. Additional sensors can be added as a library, included as a .h file and an object created. To command them follow a similar structure as documented in the above sections.


## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

Please make sure to update tests as appropriate.

## License
[MIT](https://choosealicense.com/licenses/mit/)