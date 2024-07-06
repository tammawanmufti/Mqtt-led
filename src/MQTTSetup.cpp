#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "MQTTSetup.h"

WiFiClient espClient;
PubSubClient client(espClient);

const char *mqtt_server = "broker.mqtt-dashboard.com";
const int mqtt_port = 1883;

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Pesan Masuk [");
  Serial.print(topic);
  Serial.println("] ");

  for (unsigned int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void reconnect()
{
  while (!client.connected())
  {
    Serial.println("Attempting MQTT connection...");
    if (client.connect(device_id))
    {
      Serial.println("connected");

      int msgLength = sniprintf(nullptr, 0, "{\"id\":\"%s\",\"type\":\"ack\"}", device_id);

      char *msg = (char *)malloc(msgLength + 1);

      if (msg != nullptr)
      {
        sniprintf(msg, msgLength + 1, "{\"id\":\"%s\",\"type\":\"ack\"}", device_id);
        client.publish("cat-gps", msg);
        free(msg);
      }
      else
      {
        Serial.println("Memory allocation failed");
        return;
      }
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setupMQTT()
{
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}
