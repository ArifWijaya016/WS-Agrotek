#include "DHT.h"
#define DHTPIN 8     // DHT PIN 2
#define DHTTYPE DHT11   // DHT 11 (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);
float c,h

void setup() {
  Serial.begin(9600);
  Serial.println("DHT 22 test!");
  dht.begin();
}

void loop() {
  delay(1000);
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) ) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C\t");
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.println(" %");
}