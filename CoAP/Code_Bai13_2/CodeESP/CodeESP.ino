#if defined(ESP8266)
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif
#include <WiFiUdp.h>
#include <coap-simple.h>

WiFiUDP udp;
Coap coap(udp);

//// POLLING/GET
// void callback(CoapPacket &packet, IPAddress ip, int port) {
//   Serial.print("Response: ");
//   Serial.println((char*)packet.payload);
// }

// void setup() {
//   Serial.begin(115200);
//   WiFi.begin("Tuan Kiet", "10000000");
//   while (WiFi.status() != WL_CONNECTED) delay(500);

//   coap.response(callback);
//   coap.start();
// }

// void loop() {
//   coap.get(
//   IPAddress(192, 168, 0, 102), // IP Node.js server
//   5683,
//   "sensor/temp"
//   );
//   delay(2000);
// }

//// OBSERVE/PUSH
void callback(CoapPacket &packet, IPAddress ip, int port) {
  Serial.print("Notify: ");
  Serial.println((char*)packet.payload);
}
void setup() {
  Serial.begin(115200);
  WiFi.begin("SSID", "PASSWORD");
  while (WiFi.status() != WL_CONNECTED) delay(500);
  coap.response(callback);
  coap.start();
  // Đăng ký Observe
  coap.observe(
  IPAddress(192,168,1,100),
  5683,
  "sensor/temp"
  );
}
void loop() {
  coap.loop();
}