#include <ESP8266WiFi.h>
#include "WifiSetup.h"

const char *ssid = "Avenger";
const char *password = "123aka321";

void setupWifi()
{
  Serial.print("Connecting to :");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
