#include <ArduinoJson.h> // Include ArduinoJson library
#include <Arduino.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include "GPSTracker.h"
#include "MQTTSetup.h"

// Create an instance of the TinyGPS++ object
TinyGPSPlus gps;

// SoftwareSerial for GPS module
SoftwareSerial gpsSerial(D1, D2); // TX, RX

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
      // Create a JSON object
      StaticJsonDocument<200> jsonDoc;

      // Populate the JSON object
      jsonDoc["id"] = "meong";
      jsonDoc["type"] = "gps";
      JsonObject data = jsonDoc.createNestedObject("data");
      data["lat"] = gps.location.lat();
      data["lng"] = gps.location.lng();

      // Serialize the JSON object to a string
      char jsonString[200];
      serializeJson(jsonDoc, jsonString);

      // Publish the JSON string to the MQTT topic 'cat-gps'
      client.publish("cat-gps", jsonString);
      digitalWrite(LED_BUILTIN, HIGH);
    }
    else
    {
      digitalWrite(LED_BUILTIN, LOW);
      delay(100);
      digitalWrite(LED_BUILTIN, HIGH);
      delay(100);
      Serial.println("Waiting for valid GPS data...");
      digitalWrite(LED_BUILTIN, LOW);
      delay(100);
      digitalWrite(LED_BUILTIN, HIGH);
      delay(100);
    }

    lastGpsUpdate = millis();
  }
}
