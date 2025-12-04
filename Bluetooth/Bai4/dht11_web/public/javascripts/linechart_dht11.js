// ==== Biến lưu dữ liệu chart ====
const labels = [];  // thời gian
const tempData = [];  // nhiệt độ
const humData = [];   // độ ẩm

// ==== Cấu hình Chart.js ====
const ctx = document.getElementById('dht11Chart').getContext('2d');
const dht11Chart = new Chart(ctx, {
    type: 'line',
    data: {
        labels: labels,
        datasets: [
            {
                label: 'Nhiệt độ (°C)',
                data: tempData,
                borderColor: 'red',
                backgroundColor: 'rgba(255,0,0,0.1)',
                fill: false,
                tension: 0.2
            },
            {
                label: 'Độ ẩm (%)',
                data: humData,
                borderColor: 'blue',
                backgroundColor: 'rgba(0,0,255,0.1)',
                fill: false,
                tension: 0.2
            }
        ]
    },
    options: {
        responsive: true,
        interaction: {
            mode: 'index',
            intersect: false
        },
        scales: {
            x: {
                title: {
                    display: true,
                    text: 'Thời gian'
                }
            },
            y: {
                title: {
                    display: true,
                    text: 'Giá trị'
                },
                beginAtZero: true
            }
        }
    }
});

// ==== Hàm parse dữ liệu từ backend ====
function parseDHT11Data(str) {
    // Giả sử backend trả dạng: "Nhiet do: 30*C, Do am: 57%"
    const tempMatch = str.match(/Nhiet do:\s*([\d.]+)/);
    const humMatch = str.match(/Do am:\s*([\d.]+)/);

    const temp = tempMatch ? parseFloat(tempMatch[1]) : null;
    const hum = humMatch ? parseFloat(humMatch[1]) : null;

    return { temp, hum };
}

// ==== Fetch dữ liệu mỗi 1 giây ====
setInterval(() => {
    fetch('/DHT11/read', { cache: 'no-store' })
        .then(res => res.json())
        .then(data => {
            if (data.status === 'success') {
                const { temp, hum } = parseDHT11Data(data.data);

                if (temp !== null && hum !== null) {
                    const now = new Date().toLocaleTimeString();

                    labels.push(now);
                    tempData.push(temp);
                    humData.push(hum);

                    // Giới hạn chart 20 điểm gần nhất
                    if (labels.length > 20) {
                        labels.shift();
                        tempData.shift();
                        humData.shift();
                    }

                    dht11Chart.update();
                }
            }
        })
        .catch(err => console.error("Fetch error:", err));
}, 1000);
