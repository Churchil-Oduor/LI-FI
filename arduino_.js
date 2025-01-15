const { SerialPort } = require('serialport');
const { ReadlineParser } = require('@serialport/parser-readline');

// Replace 'COM6' with the port your Arduino is connected to
const port = new SerialPort({ path: 'COM6', baudRate: 9600 });

// Use the readline parser to handle serial data
const parser = port.pipe(new ReadlineParser({ delimiter: '\r\n' }));

parser.on('data', (data) => {
  console.log(`Sensor Value: ${data}`);
});

port.on('error', (err) => {
  console.error(`Error: ${err.message}`);
});

