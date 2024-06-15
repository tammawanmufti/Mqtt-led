#include <Arduino.h>
#include "GPSTracker.h"
#include "WifiSetup.h"
#include "MQTTSetup.h"
#include "SimSetup.h"

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  setupWifi();
  // setupSIM800L();
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