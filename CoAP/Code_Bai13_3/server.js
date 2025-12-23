const coap = require('coap');
const server = coap.createServer();

const firmware = Buffer.alloc(2048, 'A'); // giả lập firmware 2KB

server.on('request', (req, res) => {
    if (req.url === '/firmware') {
        console.log('Firmware requested');
        res.end(firmware);
    }
});

server.listen(() => {
    console.log('CoAP Blockwise server started on port 5683');
});