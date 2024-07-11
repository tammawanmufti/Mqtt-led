#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "MQTTSetup.h"

WiFiClient espClient;
// Definisikan PubSubClient MQTT Client
PubSubClient client(espClient);

// Definisikan Alamat MQTT Server
const char *mqtt_server = "broker.mqtt-dashboard.com";
// Definisikan Port MQTT Server
const int mqtt_port = 1883;

// Defiinisikan Fungsi Callback untuk menerima pesan dari MQTT Server
void callback(char *topic, byte *payload, unsigned int length)
{
  // Tampilkan Pesan Masuk pada Serial Monitor
  // Pesan Masuk [cat-gps]
  Serial.print("Pesan Masuk [");
  Serial.print(topic);
  Serial.println("] ");

  // Tampilkan Pesan yang diterima pada Serial Monitor
  for (unsigned int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

// Definisikan Fungsi reconnect untuk koneksi ulang jika terputus dari Server MQTT
void reconnect()
{
  // Loop sampai terhubung ke Server MQTT
  while (!client.connected())
  {
    Serial.println("Attempting MQTT connection...");
    // Jika terhubung ke Server MQTT
    if (client.connect(device_id))
    {
      // Tampilkan pesan "connected" pada Serial Monitor
      Serial.println("connected");

      // Buat Message dengan format JSON agar bisa dibaca oleh API dan Aplikasi
      // Contoh: Pesan ack yang dikirimkan ke Server MQTT
      // {"id":"meong","type":"ack"}
      int msgLength = sniprintf(nullptr, 0, "{\"id\":\"%s\",\"type\":\"ack\"}", device_id);
      char *msg = (char *)malloc(msgLength + 1);
      if (msg != nullptr)
      {
        // Buat Message dengan format JSON agar bisa dibaca oleh API dan Aplikasi
        // Contoh: Pesan ack yang dikirimkan ke Server MQTT
        sniprintf(msg, msgLength + 1, "{\"id\":\"%s\",\"type\":\"ack\"}", device_id);

        // Kirimkan Pesan ke Server MQTT
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
      // Jika gagal terhubung ke Server MQTT
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

// Definisikan Fungsi setupMQTT
void setupMQTT()
{
  // Menghubungkan ESP8266 ke Server MQTT
  client.setServer(mqtt_server, mqtt_port);
  // Set Fungsi Callback
  client.setCallback(callback);
}
