#include <Arduino.h>
#include "WifiSetup.h"
#include "MQTTSetup.h"
#include "GPSTracker.h"

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  setupWifi();
  setupMQTT();
  setupGPS();
}

void loop()
{
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();
  processGPS();
}