#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2); 
int sensorPin = A0;     // Soil moisture sensor analog output pin
float nilaisensor = 0;  // Variable to store the sensor value
int nilaimapping = 0; // Variable to store the mapped value

void setup() {
  Serial.begin(9600); // Initialize serial communication for debugging
}

void loop() {
  nilaisensor = analogRead(sensorPin); // Read the raw sensor value

  // Map the sensor value to a range (e.g., 0-100%) for better readability
  nilaimapping = map(nilaisensor, 0, 1024, 100, 0);

  lcd.setCursor(1, 1);
  lcd.print("K. Tanah: ");
  lcd.print(nilaimapping);
  lcd.print("%");
  Serial.print("K. Tanah: ");
  Serial.println(nilaimapping);
  Serial.println("%");  

  delay(1000); // Wait for a second before taking the next reading
}
