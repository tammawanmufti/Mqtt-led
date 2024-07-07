#include <ESP8266WiFi.h>
#include "WifiSetup.h"

// const char *ssid = "Oppo";
// const char *password = "Evo12345";

const char *ssid = "Haihai";
const char *password = "12341234";

void setupWifi()
{
  Serial.print("Connecting to :");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    digitalWrite(LED_BUILTIN, LOW);
    delay(200);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(300);

    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
