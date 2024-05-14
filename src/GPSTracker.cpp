#include <Arduino.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include "GPSTracker.h"
#include "MQTTSetup.h"

// Create an instance of the TinyGPS++ object
TinyGPSPlus gps;

// SoftwareSerial for GPS module
SoftwareSerial gpsSerial(D1, D2); // RX, TX

// Timer for GPS data processing
unsigned long lastGpsUpdate = 0;
const unsigned long gpsUpdateInterval = 1000; // 1 second

void setupGPS()
{
  gpsSerial.begin(9600); // Initialize GPS serial port
  Serial.println("GPS Module initialized.");
}

void processGPS()
{
  while (gpsSerial.available() > 0)
  {
    gps.encode(gpsSerial.read());
  }

  // Check if it's time to update GPS data
  if (millis() - lastGpsUpdate >= gpsUpdateInterval)
  {
    if (gps.location.isValid())
    {
      char gpsData[128];
      snprintf(gpsData, sizeof(gpsData), "Latitude: %f, Longitude: %f", gps.location.lat(), gps.location.lng());
      Serial.println(gpsData);
      client.publish("cat-gps", gpsData);
    }
    else
    {
      Serial.println("Waiting for valid GPS data...");
    }

    lastGpsUpdate = millis();
  }
}
