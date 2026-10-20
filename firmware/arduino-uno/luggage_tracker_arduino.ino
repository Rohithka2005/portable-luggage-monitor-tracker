/*
 * Portable Luggage Monitor and Tracker
 * Arduino UNO firmware, based on the project report.
 *
 * Libraries: TinyGPSPlus, SoftwareSerial, Wire, and a compatible MPU6050 library.
 * Verify the MPU6050 library API and wiring with your installed library/hardware.
 */

#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <MPU6050.h>
#include <math.h>

TinyGPSPlus gps;
SoftwareSerial gpsSerial(3, 4);  // Arduino RX, TX connected to GPS TX, RX
MPU6050 mpu;

unsigned long lastSendTime = 0;
const unsigned long sendInterval = 5000;

void setup() {
  Serial.begin(9600);       // UART output to ESP32
  gpsSerial.begin(9600);    // NEO-6M GPS
  Wire.begin();
  mpu.initialize();
}

void loop() {
  while (gpsSerial.available()) {
    gps.encode(gpsSerial.read());
  }

  if (millis() - lastSendTime >= sendInterval) {
    lastSendTime = millis();
    sendDataToESP32();
  }
}

void sendDataToESP32() {
  String data = "";

  if (gps.location.isValid()) {
    data += "Lat: " + String(gps.location.lat(), 6);
    data += ", Lng: " + String(gps.location.lng(), 6);
  } else {
    data += "Location: INVALID";
  }

  if (gps.time.isValid()) {
    int hour = gps.time.hour();
    int minute = gps.time.minute();
    int second = gps.time.second();

    // GPS time is UTC; convert to India Standard Time (UTC+5:30).
    minute += 30;
    if (minute >= 60) {
      minute -= 60;
      hour += 1;
    }
    hour += 5;
    if (hour >= 24) hour -= 24;

    data += ", Time (IST): ";
    if (hour < 10) data += "0";
    data += String(hour) + ":";
    if (minute < 10) data += "0";
    data += String(minute) + ":";
    if (second < 10) data += "0";
    data += String(second);
  } else {
    data += ", Time: INVALID";
  }

  if (gps.date.isValid()) {
    data += ", Date: " + String(gps.date.day()) + "/" +
            String(gps.date.month()) + "/" + String(gps.date.year());
  } else {
    data += ", Date: INVALID";
  }

  // Simple tilt heuristic using acceleration-derived angles.
  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);

  float angleX = atan2((float)ax, sqrt((float)ay * ay + (float)az * az))
                 * 180.0 / PI;
  float angleY = atan2((float)ay, sqrt((float)ax * ax + (float)az * az))
                 * 180.0 / PI;

  if (fabs(angleX) > 15 || fabs(angleY) > 15) {
    data += ", Status: Tampered";
  } else {
    data += ", Status: Secured";
  }

  Serial.println(data);  // Send a newline-terminated record to ESP32.
}
