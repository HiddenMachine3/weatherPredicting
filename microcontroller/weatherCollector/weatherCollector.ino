#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>
#include <DHT.h>

#define DHTPIN D4         // Pin connected to the DHT11 sensor
#define DHTTYPE DHT11     // DHT sensor type
#define ALTITUDE 100.0    // Altitude in meters

DHT dht(DHTPIN, DHTTYPE);
Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);

void setup() {
  Serial.begin(9600);
  dht.begin();
  
  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP180 sensor, check wiring!");
    while (1);
  }
}

void loop() {
  delay(2000); // Delay for 2 seconds between readings

  float temperature = dht.readTemperature(); // Read temperature in Celsius
  float humidity = dht.readHumidity();       // Read humidity
  
  sensors_event_t event;
  bmp.getEvent(&event);

  // Check if any sensor reads failed and exit early (to try again)
  if (isnan(temperature) || isnan(humidity) || event.pressure == 0) {
    Serial.println("Failed to read sensor data!");
    return;
  }

  // Convert pressure to hPa
  float pressure = event.pressure / 100.0;

  // Print temperature, humidity, and pressure to the Serial Monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C\t");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print(" %\t");
  Serial.print("Pressure: ");
  Serial.print(pressure);
  Serial.println(" hPa");

  // Calculate altitude based on pressure
  float altitude = bmp.pressureToAltitude(920, seaLevelPressure(pressure));
  Serial.print("Altitude: ");
  Serial.print(altitude, 2);
  Serial.println(" meters");
}

float seaLevelPressure(float pressure) {
  return pressure / pow(1.0 - (ALTITUDE / 44330.0), 5.255);
}