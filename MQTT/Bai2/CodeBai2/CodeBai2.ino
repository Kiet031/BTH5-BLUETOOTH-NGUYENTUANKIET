/* Lựa chọn thư viện Wifi dựa trên loại mạch bạn dùng (Bỏ comment dòng tương ứng) */
#include <WiFi.h>        // Dùng cho ESP32
#include <PubSubClient.h>

/* ===== CẤU HÌNH NGƯỜI DÙNG (CẦN SỬA) ===== */
const char* ssid = "Tuan Kiet";
const char* password = "10000000";
const char* mqtt_server = "192.168.0.105"; // IP của Home Assistant
const int mqtt_port = 1883;

/* TÀI KHOẢN MQTT */
const char* mqtt_user = "mptt-user"; // User tạo trong HA
const char* mqtt_pass = "12345"; // Pass tạo trong HA

/* ĐỊNH DANH SINH VIÊN */
const char* student_id = "10123195"; // Sửa thành mã SV của bạn

/* MQTT TOPICS (Tự động nối chuỗi) */
// Cmd: iot/lab2/sv01/cmd
// State: iot/lab2/sv01/state
String topic_cmd_str = String("iot/lab2/") + student_id + "/cmd";
String topic_state_str = String("iot/lab2/") + student_id + "/state";

const char* topic_cmd = topic_cmd_str.c_str();
const char* topic_state = topic_state_str.c_str();

/* CHÂN LED (GPIO 2 thường là LED Onboard) */
const int LED_PIN = 2; 
bool ledState = false;

WiFiClient espClient;
PubSubClient client(espClient);

/* ================== KẾT NỐI WIFI ================== */
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

/* ================== XỬ LÝ LỆNH TỪ MQTT ================== */
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  String message;
  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.println(message);

  // Xử lý bật tắt LED
  if (message == "ON") {
    ledState = true;
    digitalWrite(LED_PIN, HIGH); 
  }
  else if (message == "OFF") {
    ledState = false;
    digitalWrite(LED_PIN, LOW);
  }

  // Gửi ngược trạng thái về Home Assistant để cập nhật giao diện
  client.publish(topic_state, ledState ? "ON" : "OFF");
}

/* ================== KẾT NỐI MQTT ================== */
void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection... ");
    
    // Tạo Client ID ngẫu nhiên hoặc theo MSSV để tránh trùng
    String clientId = "ESP-Lab2-";
    clientId += student_id;

    if (client.connect(clientId.c_str(), mqtt_user, mqtt_pass)) {
      Serial.println("connected");
      
      // Đăng ký nhận lệnh từ topic CMD
      // Lưu ý: Phải convert lại sang const char* vì hàm subscribe yêu cầu
      client.subscribe(topic_cmd_str.c_str()); 
      Serial.print("Subscribed to: ");
      Serial.println(topic_cmd_str);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 2 seconds");
      delay(2000);
    }
  }
}

/* ================== SETUP ================== */
void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW); // Mặc định tắt
  
  Serial.begin(115200);
  setup_wifi();
  
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

/* ================== LOOP ================== */
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}