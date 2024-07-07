#include <ArduinoJson.h> // Include ArduinoJson library
#include <Arduino.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include "GPSTracker.h"
#include "MQTTSetup.h"
#include "MQTTSetup.h"

// Create an instance of the TinyGPS++ object
TinyGPSPlus gps;

// SoftwareSerial for GPS module
SoftwareSerial gpsSerial(D1, D2); // TX, RX

// Timer for GPS data processing
unsigned long lastGpsUpdate = 0;
const unsigned long gpsUpdateInterval = 20000; // 1 second

void setupGPS()
{
  gpsSerial.begin(9600); // Initialize GPS serial port
  Serial.println("GPS Module initialized.");
  Serial.println(device_id);
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
    StaticJsonDocument<200> jsonDoc;
    jsonDoc["id"] = device_id;
    jsonDoc["type"] = "gps";
    char jsonString[200];

    if (gps.location.isValid())

    {
      jsonDoc["message"] = "GPS data valid.";
      jsonDoc["status"] = true;
      JsonObject data = jsonDoc.createNestedObject("data");
      data["lat"] = gps.location.lat();
      data["lng"] = gps.location.lng();

      // Serialize the JSON object to a string
      serializeJson(jsonDoc, jsonString);
    }
    else
    {
      digitalWrite(LED_BUILTIN, LOW);
      delay(100);
      digitalWrite(LED_BUILTIN, HIGH);
      delay(100);
      Serial.println("Waiting for valid GPS data...");
      jsonDoc["message"] = "GPS data invalid.";
      jsonDoc["status"] = false;
      JsonObject data = jsonDoc.createNestedObject("data");
      serializeJson(jsonDoc, jsonString);
      digitalWrite(LED_BUILTIN, LOW);
      delay(100);
      digitalWrite(LED_BUILTIN, HIGH);
      delay(100);
    }
    client.publish("cat-gps", jsonString);
    digitalWrite(LED_BUILTIN, HIGH);

    lastGpsUpdate = millis();
  }
}
