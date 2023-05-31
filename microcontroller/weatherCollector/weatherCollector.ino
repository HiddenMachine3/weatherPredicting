#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>
#include <DHT.h>

#define D4 2
#define DHTPIN D4       // Pin connected to the DHT11 sensor
#define DHTTYPE DHT11   // DHT sensor type
#define ALTITUDE 920.0  // Altitude in meters

DHT dht(DHTPIN, DHTTYPE);
Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);

void setup() {
  Serial.begin(9600);
  dht.begin();

  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP180 sensor, check wiring!");
    while (1)
      ;
  }

  initNetworking();
}

void loop() {
  delay(500);  // Delay for 2 seconds between readings

  float temperature = dht.readTemperature();  // Read temperature in Celsius
  float humidity = dht.readHumidity();        // Read humidity

  sensors_event_t event;
  bmp.getEvent(&event);

  // Check if any sensor reads failed and exit early (to try again)
  if (isnan(temperature) || isnan(humidity) || event.pressure == 0) {
    Serial.println("Failed to read sensor data!");
    return;
  }

  // Convert pressure to bar
  float pressure = event.pressure / 1000.0;

  // Print temperature, humidity, and pressure to the Serial Monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C\t");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print(" %\t");
  Serial.print("Pressure: ");
  Serial.print(pressure);
  Serial.println(" bar");

  String timeString = getCurrentDateTimeString();

  sendHTTPWeatherRequest(timeString, temperature, humidity, pressure);  
}

float seaLevelPressure(float pressure) {
  return pressure / pow(1.0 - (ALTITUDE / 44330.0), 5.255);
}




#include <NTPClient.h>
#include <pt.h>
#include <SPI.h>
//#include <Ethernet.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

//time
const long utcOffsetInSeconds = 3600;
char daysOfTheWeek[7][12] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };
// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

//insertion code : insert into pulsedata (time, value) values ('0000-00-00 00:00:00', 12);
//  iot variables
int Threshold = 550;           // Determine which Signal to "count as a beat", and which to ingore.

//network variables
// replace the MAC address below by the MAC address printed on a sticker on the Arduino Shield 2
//80:7D:3A:6F:0C:FB
byte mac[] = {0x40,0xF5,0x20,0xF3,0xD1,0xBA };//0x80, 0x7D, 0x3A, 0x6F, 0x0C, 0xFB };
char* ssid = "RVU";
char* pass = "RVU@guru";
//char* ssid = "RVU";
//char* pass = "RVU@guru";

//EthernetClient client;
WiFiClient client;
//http://192.168.29.65/insertpulsedata.php?value=12&time=0000-00-00 0000:00:00

int HTTP_PORT = 80;
String HTTP_METHOD = "GET";
char HOST_NAME[] = "172.16.185.144";//"172.16.185.144";//"192.168.29.65";  //172.16.185.144";  //"192.168.154.174";  // change to your PC's IP address //RVU : 172.16.185.144
String PATH_NAME = "/insertweatherdata.php";
//String queryPattern = "?value=%d&time=%s";
//String queryString = "?value=12&time=2001-10-10%2010:01:10";

void initNetworking() {
  Serial.print("Current mac address : ");
  Serial.println(WiFi.macAddress());

  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(5000);
    Serial.print(".");
  }
  Serial.println("\nConnected!");

  // connect to web server on port 80:
  if (client.connect(HOST_NAME, HTTP_PORT)) {
    // if connected:
    Serial.println("Connected to server");
    // make a HTTP request:
    // send HTTP header
    // print "My age is " and age variable to buffer variable

    //   client.println(HTTP_METHOD + " " + PATH_NAME + queryString + " HTTP/1.1");
    //   client.println("Host: " + String(HOST_NAME));
    //   client.println("Connection: close");
    //   client.println();  // end HTTP header


    while (client.connected()) {
      if (client.available()) {
        // read an incoming byte from the server and print it to serial monitor:
        char c = client.read();
        Serial.print(c);
      }
    }

    // the server's disconnected, stop the client:
    // client.stop();
    // Serial.println();
    // Serial.println("disconnected");
  } else {  // if not connected:
    Serial.println("connection failed");
  }
}

String getCurrentDateTimeString() {
  timeClient.update();

  //time
  time_t epochTime = timeClient.getEpochTime();
  String formattedTime = timeClient.getFormattedTime();
  //extracting milliseconds
  struct timeval tv;
  if (gettimeofday(&tv, NULL) != 0) {
    Serial.println("Failed to obtain time");
  }
  char usec[30];
  sprintf(usec, "%lld", (long long)tv.tv_usec);

  formattedTime += ".";
  formattedTime += usec;

  struct tm* ptm = gmtime((time_t*)&epochTime);  //Get a time structure

  int monthDay = ptm->tm_mday;
  int currentMonth = ptm->tm_mon + 1;
  int currentYear = ptm->tm_year + 1900;
  //print(usec)
  //Print complete date:
  String currentDate = String(currentYear) + "-" + String(currentMonth) + "-" + String(monthDay);

  return currentDate + "%20" + formattedTime;
}

void sendHTTPWeatherRequest(String timestamp, float temperature, float humidity, float pressure) {  //needed timestamp format : 2001-10-10%2010:01:10 TODO: and millis to be introduced
  if (client.connect(HOST_NAME, HTTP_PORT)) {
    // String query_string = HTTP_METHOD + " " + PATH_NAME + "?temperature=" + temperature + "&presure=" + pressure + "&humidity=" + humidity + "&time=" + timestamp + " HTTP/1.1";
    // client.println(query_string);
    Serial.println(timestamp);
    client.println("GET 172.16.185.144/insertweatherdata.php?time=2023-12-12%20016:30:00&pressure=12&temperature=12&humidity=12 HTTP/1.1");
    client.println("Host: " + String(HOST_NAME));
    client.println("Connection: close");
    client.println();  // end HTTP header

    while (client.connected()) {
      if (client.available()) {
        // read an incoming byte from the server and print it to serial monitor:
        char c = client.read();
        //Serial.print(c);
      }
    }
  } else
    Serial.println("Connection failed!");
}
