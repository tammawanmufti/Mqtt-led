#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

WiFiClient espClient;
PubSubClient client(espClient);

// Setup Wifi
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
// End Setup Wifi

// Setup MQTT
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

  if ((char)payload[0] == '1')
  {
    // Hidupkan
    digitalWrite(LED_BUILTIN, LOW);
  }
  if ((char)payload[0] == '0')
  {
    // Matikan
    digitalWrite(LED_BUILTIN, HIGH);
  }
}

void reconnect()
{
  while (!client.connected())
  {
    Serial.println("Attempting MQTT connection...");
    if (client.connect("Acing-1"))
    {
      Serial.println("connected");
      client.publish("cat-gps", "hello world");
      client.subscribe("cat-gps");
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

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  setupWifi();
  setupMQTT();
}

void loop()
{
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();
}
