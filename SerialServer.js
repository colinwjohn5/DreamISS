"use strict";
process.title = 'node-serial-ws';
const ReadLine = require('readline');
var serialPort = require("serialport")

var WebSocket = require('ws')

var webSocket = new WebSocket("wss://0gokys5xu6.execute-api.us-east-1.amazonaws.com/development/");



// Websocket
var webSocketsServerPort = 1337;
var webSocketServer = require('websocket').server;
var http = require('http');
var server = http.createServer(function(request, response) {
    // Not important for us. We're writing WebSocket server, not HTTP server
});
var clients = [];

server.listen(webSocketsServerPort, function() {
    console.log((new Date()) + " Server is listening on port " + webSocketsServerPort);
});

var wsServer = new webSocketServer({
    httpServer: server
});


//wss://0gokys5xu6.execute-api.us-east-1.amazonaws.com/development/
wsServer.on('request', function(request) {
    console.log((new Date()) + ' Connection from origin ' + request.origin + '.');
    var connection = request.accept(null, request.origin); 
    console.log((new Date()) + ' Connection accepted.');

    var index = clients.push(connection) - 1;

    // user sent some message
    connection.on('message', function(message) {
      onReceive(message);
    });

    // user disconnected
    connection.on('close', function(connection) {
        if (userName !== false && userColor !== false) {
            console.log((new Date()) + " Peer "
                + connection.remoteAddress + " disconnected.");
            // remove user from the list of connected clients
            clients.splice(index, 1);
        }
    });

});

function onReceive(msg)
{
  console.log("ws msg:" + msg);
  serialPort.write(msg);
}

function onSerial(msg)
{
  console.log("uart msg:" + msg);
  for (var i=0; i < clients.length; i++) 
    clients[i].sendUTF(msg);
}

// Serial port
 