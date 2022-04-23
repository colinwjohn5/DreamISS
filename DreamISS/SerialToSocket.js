var WebSocket = require('ws')

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

  
  
  const { SerialPort } = require('serialport')
  const { ReadlineParser } = require('@serialport/parser-readline')
  const port = new SerialPort({ path: 'COM3', baudRate: 115200 })
  
  const parser = port.pipe(new ReadlineParser({ delimiter: '\r\n' }))
  
  let bitsArray = []
  parser.on('data', function(data) {
  var bits = data;
  // add code here
  bitsArray.push(bits)
  console.log(bits);
  webSocket.send(bits);
});

    webSocket.addEventListener('message', function (event) {
      var commandObject = JSON.parse(event.data);
      incomingCommand = commandObject.Message;
      port.write(incomingCommand);
  });

  
  
  // Pipe the data into another stream (like a parser or standard out)
  
  //console.log(parser);
//webSocket.close();
 