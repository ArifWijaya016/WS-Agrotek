#include "DHT.h"
#include <ESP8266WiFi.h>
#include <LiquidCrystal_I2C.h>
#include <elapsedMillis.h>

#define DHTPIN D6     // DHT PIN 2
#define DHTTYPE DHT11   // DHT 11 (AM2302), AM2321
int sensorPin = A0;     // Soil moisture sensor analog output pin
DHT dht(DHTPIN, DHTTYPE);

LiquidCrystal_I2C lcd(0x3F, 16, 2);
float nilaisensor = 0;  // Variable to store the sensor value
int nilaimapping = 0; // Variable to store the mapped value
int t, h;

//Interval masing-masing fungsi
const unsigned long intervalInput   = 3000;   // Interval baca sensor (ms)
const unsigned long intervalOutput  = 1000;   // Interval print LCD (ms)

// Variabel untuk menghitung waktu yang telah berlalu
elapsedMillis elapsedInput;
elapsedMillis elapsedOutput;

void setup() {
  Serial.begin(115200);
  //memulai dht
  dht.begin();
  //setting lcd awal
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("S: ");
  lcd.setCursor(9, 0);
  lcd.print("KR: ");
  lcd.setCursor(0, 1);
  lcd.print("K.Tanah: ");
}

void loop() {
  if (elapsedInput >= intervalInput) {
    //DHT11 read
    h = dht.readHumidity();
    t = dht.readTemperature(); // Read temperature as Celsius (the default)
    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t) ) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }
    //Moist
    nilaisensor = analogRead(sensorPin); // Read the raw sensor value
    // Map the sensor value to a range (e.g., 0-100%) for better readability
    nilaimapping = map(nilaisensor, 0, 1023, 100, 0);
    elapsedInput = 0;
  }

  if (elapsedOutput >= intervalOutput) {
    //Serial
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.println(" *C\t");
    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.println(" %");
    Serial.print("K.Tanah : ");
    Serial.print(nilaimapping);
    Serial.println("%");
    //LCD
    lcd.setCursor(3, 0);
    lcd.print(t);
    lcd.print(char(223));
    lcd.print("C");
    lcd.setCursor(13, 0);
    lcd.print(h);
    lcd.print("%");
    lcd.setCursor(9, 1);
    lcd.print(nilaimapping);
    lcd.print("%");
    elapsedOutput = 0;
  }

}