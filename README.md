# GPS Data Logger and Viewer

This project involves logging GPS data from an Arduino using a GPS module, sending the data to Firebase Realtime Database, and displaying it on a web page. The GPS data includes latitude, longitude, altitude, speed, number of satellites, date, and time, which are sent from the Arduino to Firebase and then fetched by a web page for display.

## Project Overview

- **Arduino**: The Arduino reads data from a GPS module and sends it to Firebase via an ESP8266 Wi-Fi module. The data is sent in JSON format, including:
  - Latitude
  - Longitude
  - Altitude
  - Speed
  - Satellites
  - Date
  - Time

- **Web Page**: The web page fetches the latest GPS data from Firebase and displays it in real-time. The data is refreshed every second. The page shows:
  - Latitude
  - Longitude
  - Altitude
  - Speed
  - Satellites
  - Date
  - Time (IST)
  - IP Address

## Prerequisites

- **Hardware**:
  - Arduino Uno (or any compatible Arduino board)
  - GPS module (e.g., Neo-6M)
  - ESP8266 Wi-Fi module

- **Software**:
  - Arduino IDE
  - Firebase Realtime Database

## Setup Instructions

### 1. Set up Firebase

- Create a Firebase project at [Firebase Console](https://console.firebase.google.com/).
- Set up Firebase Realtime Database and get your Database URL and API key.
- Update the Firebase URL and API key in the Arduino code.

### 2. Upload Arduino Code

- Open the Arduino IDE and upload the provided Arduino code to your Arduino.
- Ensure that the correct board and port are selected.
- The Arduino code uses the `TinyGPS++`, `ESP8266WiFi`, and `ArduinoJson` libraries. You may need to install them through the Arduino Library Manager.

### 3. Set up Wi-Fi

- Update the `ssid` and `password` variables in the Arduino code with your Wi-Fi credentials.

### 4. Web Page Setup

- Open the provided HTML file in a browser to view the GPS data.
- The web page fetches the latest GPS data from Firebase and displays it in real-time.

### 5. Test the System

- Once everything is set up, power the Arduino and wait for the GPS module to get a fix.
- The Arduino will start sending GPS data to Firebase.
- The web page will fetch and display the data.

## How it Works

1. **Arduino**: Collects GPS data and sends it to Firebase every 5 seconds.
2. **Web Page**: Periodically fetches the latest GPS data from Firebase and updates the display.

## Data Structure in Firebase

The data sent from Arduino to Firebase is structured as follows:

```json
{
  "latitude": 12.345678,
  "longitude": 98.765432,
  "altitude": 100.0,
  "satellites": 8,
  "speed": 50.0,
  "timestamp": 1614728492,
  "day": 12,
  "month": 11,
  "year": 2024,
  "hour": 10,
  "minute": 30,
  "second": 45,
  "ip": "192.168.1.1"
}
