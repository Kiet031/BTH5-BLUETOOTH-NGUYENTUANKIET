// IMPORT FOR ESP32
#include <WiFi.h>

#include <PubSubClient.h>
#include <ArduinoJson.h>

/* ====== CONFIG ====== */
const char* device_name = "Kiet_ESP32";

const char* ssid = "Tuan Kiet";
const char* password = "10000000";

// IP Home Assistant
// const char* mqtt_server = "10.174.178.157";
const char* mqtt_server = "192.168.0.105";
const int mqtt_port = 1883;

const char* mqtt_user = "mptt-user";
const char* mqtt_pass = "12345";

const char* student_id = "10123195";
const char* mqtt_topic = "iot/lab1/10123195/sensor";
/* ==================== */

WiFiClient espClient;
PubSubClient client(espClient);

/* ===== WIFI CONNECT ===== */
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);      // ESP32 cần khai báo mode
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

/* ===== MQTT RECONNECT ===== */
void reconnect() {
  while (!client.connected()) {
    String clientId = device_name;
    clientId += "_";
    clientId += student_id;

    Serial.print("Attempting MQTT connection...");
    if (client.connect(clientId.c_str(), mqtt_user, mqtt_pass)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" retrying in 2 seconds");
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  StaticJsonDocument<200> doc;
  doc["temp"] = random(250, 350) / 10.0;
  doc["hum"]  = random(400, 800) / 10.0;

  char payload[128];
  serializeJson(doc, payload);

  client.publish(mqtt_topic, payload);

  Serial.print("Publish: ");
  Serial.println(payload);

  delay(5000);
  client.disconnect();
}
