const express = require('express');
const router = express.Router();

const serial = require('../public/services/serialService');

let latesData = "No data";

router.get('/', function(req, res) {
    // res.send('DHT11 Home Page');
    res.render('LineChart_DHT11_Display', {title: 'Theo doi nhiet do va do am'});
});
// ========== MỞ CỔNG SERIAL ==========
router.get('/connect', async (req, res) => {
    try {
        const message = await serial.connectSerial();
        res.json({status: 'success', message: message});
    } catch (error) {
        res.json({status: 'error', message: error.message});
    }
});

/* GET DHT11 data. */
router.get('/read', (req, res) => {
    const data = serial.readDHT11();
    res.json({status: 'success', data: data});
});

// ========== ĐÓNG SERIAL ==========
router.get('/disconnect', (req, res) => {
    serial.disconnectSerial();
    res.json({status: 'success', message: 'Serial port disconnected'});
});

module.exports = router;