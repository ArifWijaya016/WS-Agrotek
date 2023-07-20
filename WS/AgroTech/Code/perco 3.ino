#include "DHT.h"
#include <ESP8266WiFi.h>
#include <LiquidCrystal_I2C.h>
#include <elapsedMillis.h>
#include <ThingSpeak.h>

#define DHTPIN D6     // DHT PIN 2
#define DHTTYPE DHT11   // DHT 11 (AM2302), AM2321
#define relay_fan D5 //Relay fan di D5
#define relay_pump D4 //Relay pompa di pin D6
int sensorPin = A0;     // Soil moisture sensor analog output pin
DHT dht(DHTPIN, DHTTYPE);

LiquidCrystal_I2C lcd(0x3F, 16, 2);
float nilaisensor = 0;  // Variable to store the sensor value
int nilaimapping = 0; // Variable to store the mapped value
int t, h;

const char* apiKey = "B44DZ6YQWAMXQQ1Z"; // hapus "****" dan isi dengan API key dari Thinkspeak 
const char* ssid = "Iphone"; // ganti *** dan isi dengan SSID WiFi
const char* password = "hqet4377"; // ganti *** dan isi dengan Password WiFi
const unsigned long chID    = 2211272;  // Ganti dengan ID channel ThingSpeak Anda
const char* server = "api.thingspeak.com";
WiFiClient client;

//Interval masing-masing fungsi
const unsigned long intervalInput   = 2000;   // Interval baca sensor (ms)
const unsigned long intervalOutput  = 3000;   // Interval print LCD (ms)
const unsigned long intervalControl  = 4000;   // Interval kontrol kondisi relay (ms)
const unsigned long intervalUpload  = 5000;   // Interval kirim data ke Thinkspeak (ms)
// Variabel untuk menghitung waktu yang telah berlalu
elapsedMillis elapsedInput;
elapsedMillis elapsedOutput;
elapsedMillis elapsedControl;
elapsedMillis elapsedUpload;

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

  //Setting Relay
  pinMode(relay_fan,OUTPUT);
  digitalWrite(relay_fan,HIGH);
  pinMode(relay_pump,OUTPUT);
  digitalWrite(relay_pump,HIGH);

  //Setting Wifi + Thinkspeak
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
//mulai Thinkspeak
  ThingSpeak.begin(client);
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

if (elapsedControl >= intervalControl) {
    if (t > 30)
    {
        digitalWrite(relay_fan,HIGH);//fan ON
    }
    else{
        digitalWrite(relay_fan,LOW);//fan off
    }
    if (nilaimapping > 70)
    {
        digitalWrite(relay_pump,LOW);
    }
    else{
        digitalWrite(relay_pump,HIGH);
    }
  elapsedControl = 0;
}
if (elapsedUpload >= intervalUpload) {
    ThingSpeak.setField(1, t); // kirim nilai suhu
    ThingSpeak.setField(2, h); // kirim nilai kelembaban ruangan
    ThingSpeak.setField(3, nilaimapping); // kirim nilai kelembapan tanah
    ThingSpeak.writeFields(chID, apiKey);
    
    elapsedUpload = 0;
}

}