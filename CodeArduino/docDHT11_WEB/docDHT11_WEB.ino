#include <DHT.h>
#include <DHT_U.h>

const int DHTPin = 12;
const int DHTTYPE = DHT11;

DHT dht(DHTPin, DHTTYPE);
void setup()
{
  Serial.begin(9600);
  dht.begin();
}
void loop()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  Serial.println("Nhiet do: " + String(t) + "*C | Do am: " + String(h) + "%");
  delay(3000);
}