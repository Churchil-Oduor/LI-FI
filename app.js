const express = require('express');
const http = require('http');
const socketIo = require('socket.io');
const { SerialPort } = require('serialport');
const { ReadlineParser } = require('@serialport/parser-readline'); // Correct import for the parser

const app = express();
const server = http.createServer(app);
const io = socketIo(server);

// Serve static files
app.use(express.static('public'));

// Serial Communication
const port = new SerialPort({ path: 'COM6', baudRate: 9600 });
const parser = port.pipe(new ReadlineParser({ delimiter: '\r\n' })); // Use ReadlineParser

parser.on('data', (data) => {
  console.log(`Sensor Value: ${data}`);
  io.emit('sensorData', data); // Send data to clients
});

// WebSocket connection
io.on('connection', (socket) => {
  console.log('A user connected');
  socket.on('disconnect', () => {
    console.log('A user disconnected');
  });
});

// Start server
server.listen(3000, () => {
  console.log('Server running on http://localhost:3000');
});

