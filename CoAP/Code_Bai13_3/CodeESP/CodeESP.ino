#if defined(ESP8266)
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif
#include <WiFiUdp.h>
#include <coap-simple.h>

WiFiUDP udp;
bool sent = false;
Coap coap(udp);

void callback(CoapPacket &packet, IPAddress ip, int port) {
  Serial.print("Received block, size=");
  Serial.println(packet.payloadlen);
}

void setup() {
  Serial.begin(115200);
  WiFi.begin("Tuan Kiet", "10000000");
  while (WiFi.status() != WL_CONNECTED) delay(500);

  coap.response(callback);
  coap.start();

  // Yêu cầu firmware
  coap.get(
  IPAddress(192, 168, 0, 102), // IP Node.js server
  5683,
  "firmware"
  );
}

void loop() {
  // if (!sent) {
  //   coap.loop();
  //   delay(50);
  //   Serial.println("WiFi OFF after GET sent");
  //   WiFi.disconnect(true);
  //   WiFi.mode(WIFI_OFF);
  //   sent = true;
  // }
  coap.loop();
}