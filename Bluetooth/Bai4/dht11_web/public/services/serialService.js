const SerialPort = require('serialport').SerialPort;
const { ReadlineParser } = require('@serialport/parser-readline');

let port = null;
let parser = null;
let isOpen = false;
let lastData = 'No data';

// ==== CONNECT ARDUINO ====
function connectSerial() {
    return new Promise((resolve, reject) => {
        if (isOpen) {
            resolve('Serial port already open');
            return;
        }
        port = new SerialPort({
            path: 'COM2',
            baudRate: 9600,
            autoOpen: false
        });

        parser = port.pipe(new ReadlineParser({ delimiter: '\r\n' }));

        parser.on('data', (line) => {
            console.log('DHT11: ', line);
            lastData = line;
        })

        port.open((err) => {
            if (err) {
                console.log('Error opening port: ', err.message);
                isOpen = false;
                reject(err.message);
            }
            else {
                console.log('Serial port opened');
                isOpen = true;
                resolve('Serial port opened successfully');
            }
        });
        port.on('error', (err) => {
            console.log('Error: ', err.message);
            isOpen = false;
        });
    });
};

// ==== READ DHT11 ====
function readDHT11(req, res) {
    return lastData;
};

function disconnectSerial() {
    if (port && isOpen) {
        port.close();
        isOpen = false;
        console.log('Serial port closed');
    }
};

module.exports = { connectSerial, readDHT11, disconnectSerial };