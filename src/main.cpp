#include <Arduino.h>
#include "GPSTracker.h"
#include "WifiSetup.h"
#include "MQTTSetup.h"
#include "SimSetup.h"

// ID Harus Unique atau akan terjadi conflict
// yang menyebabkan mqtt tidak bisa konek
char *device_id = "meong";

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  setupWifi(); // Eksekusi Fungsi setupWifi
  // setupSIM800L(); // Eksekusi Fungsi setupSIM800L
  setupMQTT(); // Eksekusi Fungsi setupMQTT
  setupGPS();  // Eksekusi Fungsi setupGPS
}

void loop()
{
  // Jika koneksi ke Server MQTT terputus
  if (!client.connected())
  {
    // Lakukan koneksi ulang
    reconnect();
  }
  // Looping untuk mendapatkan data dari Server MQTT
  client.loop();
  // Looping untuk mendapatkan data GPS
  processGPS();
}