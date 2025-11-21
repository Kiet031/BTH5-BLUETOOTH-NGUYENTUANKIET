function connectSerial() {
    fetch('/DHT11/connect')
    .then(response => response.json())
    .then(console.log)
};

function disconnectSerial() {
    fetch('/DHT11/disconnect')
    .then(response => response.json())
    .then(console.log)
}

// setInterval(() => {
//     fetch('/DHT11/read')
//     .then(response => response.json())
//     .then(data => {
//         console.log('Data nhan: ', data)
//         document.getElementById('dht11Data').innerText = data.data;
//     })
//     .catch(err => console.error("ERR: ", err));
// }, 1000);