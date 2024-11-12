#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
// Replace with your Firebase Realtime Database URL and API key
const char* DATABASE_URL = "https://neaak-2c785-default-rtdb.firebaseio.com/gps.json"; 
const char* API_KEY = "AIzaSyAWbDUMemEPJG5Xj4pZXau66uuq_9-Qf98";

// WiFi credentials (replace with your network SSID and password)
const char* ssid = "A";
const char* password = "1234567890";

// Create an instance of TinyGPS++
TinyGPSPlus gps;

// Create a software serial port for GPS (Pin 4 = RX, Pin 3 = TX)
SoftwareSerial gpsSerial(4, 3);

void setup() {
  Serial.begin(9600);        // Begin serial communication with your PC
  gpsSerial.begin(9600);     // Begin serial communication with GPS
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println(" Connected!");

  Serial.println("Waiting for GPS signal...");
}

void loop() {
  while (gpsSerial.available() > 0) {
    gps.encode(gpsSerial.read()); // Read GPS data

    if (gps.location.isUpdated()) {
      // Collect GPS data
      float latitude = gps.location.lat();
      float longitude = gps.location.lng();
      float altitude = gps.altitude.meters();
      int satellites = gps.satellites.value();
      float speed = gps.speed.kmph();
      int day = gps.date.day();
      int month = gps.date.month();
      int year = gps.date.year();
      int hour = gps.time.hour();
      int minute = gps.time.minute();
      int second = gps.time.second();

      // Display on Serial Monitor
      Serial.print("Latitude: ");
      Serial.println(latitude, 6);
      Serial.print("Longitude: ");
      Serial.println(longitude, 6);
      Serial.print("Altitude: ");
      Serial.println(altitude);
      Serial.print("Satellites: ");
      Serial.println(satellites);
      Serial.print("Speed: ");
      Serial.print(speed);
      Serial.println(" km/h");
      Serial.print("Date: ");
      Serial.print(day);
      Serial.print("/");
      Serial.print(month);
      Serial.print("/");
      Serial.println(year);
      Serial.print("Time: ");
      Serial.print(hour);
      Serial.print(":");
      Serial.print(minute);
      Serial.print(":");
      Serial.println(second);
      Serial.println();

      // Send data to Firebase
      sendToFirebase(latitude, longitude, altitude, satellites, speed, day, month, year, hour, minute, second);
      
      delay(5000); // Delay to prevent spamming requests
    }
  }
}




void sendToFirebase(double latitude, double longitude, double altitude, int satellites, double speed, int day, int month, int year, int hour, int minute, int second) {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClientSecure client;
    client.setInsecure(); // Disable certificate verification (use only for testing)

    HTTPClient http;
    String url = String(DATABASE_URL) + "?auth=" + API_KEY;

    // Get the local IP address
    String ipAddress = WiFi.localIP().toString();

    // JSON Payload
    StaticJsonDocument<256> jsonDoc;
    jsonDoc["latitude"] = latitude;
    jsonDoc["longitude"] = longitude;
    jsonDoc["altitude"] = altitude;
    jsonDoc["satellites"] = satellites;
    jsonDoc["speed"] = speed;
    jsonDoc["timestamp"] = millis() / 1000; // Send timestamp in seconds
    jsonDoc["day"] = day;
    jsonDoc["month"] = month;
    jsonDoc["year"] = year;
    jsonDoc["hour"] = hour;
    jsonDoc["minute"] = minute;
    jsonDoc["second"] = second;
    jsonDoc["ip"] = ipAddress; // Add IP address to the payload

    String requestBody;
    serializeJson(jsonDoc, requestBody);

    // Print payload for debugging
    Serial.print("JSON Payload: ");
    Serial.println(requestBody);

    http.begin(client, url);
    http.addHeader("Content-Type", "application/json");

    // Send a POST request
    int httpResponseCode = http.POST(requestBody);
    
    // Check for success or failure
    if (httpResponseCode > 0) {
      Serial.print("Data sent successfully, HTTP response code: ");
      Serial.println(httpResponseCode);
      Serial.print("Response body: ");
      Serial.println(http.getString()); // Print response body for debugging
    } else {
      Serial.print("Error sending data, HTTP response code: ");
      Serial.println(httpResponseCode);
      Serial.print("Error message: ");
      Serial.println(http.errorToString(httpResponseCode).c_str());
    }

    // End the HTTP request
    http.end();
  } else {
    Serial.println("Wi-Fi not connected. Cannot send data.");
  }
}
