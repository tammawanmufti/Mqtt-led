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

// Definisikan variabel untuk menyimpan waktu terakhir update GPS
unsigned long lastGpsUpdate = 0;

// Definisikan interval waktu untuk update GPS
const unsigned long gpsUpdateInterval = 20000; // 20 detik

// Definisikan fungsi setupGPS
void setupGPS()
{
  // Set gps Serial dengan baudrate 9600 yang ado di pin D1 dan D2
  // Komunikasi Module GPS dengan NodeMCU menggunakan Komunikasi Serial
  gpsSerial.begin(9600);

  // Tampilkan pesan "GPS Module initialized." pada Serial Monitor
  Serial.println("GPS Module initialized.");

  // Tampilkan pesan "Device ID: " pada Serial Monitor
  Serial.println(device_id);
}

// Definisikan fungsi processGPS
// Fungsi ini digunakan untuk mengupdate data GPS
// Data GPS yang diupdate akan dikirimkan ke Server MQTT
void processGPS()
{
  // Cek apakah ada data yang masuk dari GPS Serial
  while (gpsSerial.available() > 0)
  {
    // Jika ada data yang masuk, lakukan baca data GPS dari gps Serial
    gps.encode(gpsSerial.read());
  }

  // Jika waktu sekarang dikurangi waktu terakhir update GPS lebih besar dari interval waktu update GPS
  // Maka lakukan update GPS
  if (millis() - lastGpsUpdate >= gpsUpdateInterval)
  {
    // Buat JSON Object untuk menyimpan data GPS
    // JSON(Javascript Object Notation) adalah format data yang digunakan untuk pertukaran data antar aplikasi
    // JSON Object terdiri dari key dan value
    StaticJsonDocument<200> jsonDoc;
    // Tambahkan data id dan type pada JSON Object
    // id adalah key
    // device_id adalah value
    // {"id":"meong"}
    jsonDoc["id"] = device_id;
    // Tambahkan data type pada JSON Object
    // type adalah key
    // gps adalah value
    // {
    //   "id":"meong"
    //   "type":"gps"
    // }
    jsonDoc["type"] = "gps";

    // Buat variabel jsonString untuk menampung JSON Object yang akan menjadi String agar bisa dikirimkan ke Server MQTT
    char jsonString[200];

    // Jika data GPS valid
    if (gps.location.isValid())
    {
      // Tambahkan data "message":"GPS data valid." pada jsonDoc
      jsonDoc["message"] = "GPS data valid.";
      //  Tambahkan data "status":true pada jsonDoc
      jsonDoc["status"] = true;
      // Buat JSON Object data untuk menyimpan data latitude dan longitude
      JsonObject data = jsonDoc.createNestedObject("data");
      // Tambahkan data latitude dan longitude pada JSON Object data
      data["lat"] = gps.location.lat();
      data["lng"] = gps.location.lng();

      // Contoh JSON Object yang akan dikirimkan ke Server MQTT
      // {
      //   "id":"meong",
      //   "type":"gps",
      //   "message":"GPS data valid.",
      //   "status":true,
      //   "data":{
      //     "lat":-6.123456,
      //     "lng":106.123456
      //   }
      // }

      // Ubah JSON Object menjadi String dan di simpan kedalam variabel jsonString
      serializeJson(jsonDoc, jsonString);
    }
    else
    {
      // Jika data GPS tidak valid LED akan Kedip Cepat
      digitalWrite(LED_BUILTIN, LOW);
      delay(100);
      digitalWrite(LED_BUILTIN, HIGH);
      delay(100);
      // Tampilkan pesan "Waiting for valid GPS data..." pada Serial Monitor
      Serial.println("Waiting for valid GPS data...");

      // Tambahkan data "message":"GPS data invalid." pada jsonDoc
      jsonDoc["message"] = "GPS data invalid.";
      // Tambahkan data "status":false pada jsonDoc
      jsonDoc["status"] = false;
      // Buat JSON Object data untuk menyimpan data latitude dan longitude tapi di isi dengan {}
      JsonObject data = jsonDoc.createNestedObject("data");

      // Contoh JSON Object yang akan dikirimkan ke Server MQTT
      // {
      //   "id":"meong",
      //   "type":"gps",
      //   "message":"GPS data invalid.",
      //   "status":false,
      //   "data":{}
      // }

      // Ubah JSON Object menjadi String dan di simpan kedalam variabel jsonString
      serializeJson(jsonDoc, jsonString);
      // LED akan Kedip Cepat
      digitalWrite(LED_BUILTIN, LOW);
      delay(100);
      digitalWrite(LED_BUILTIN, HIGH);
      delay(100);
    }
    // Kirimkan data GPS yang sudah di ubah ke bentuk JSON String tadi ke Server MQTT
    client.publish("cat-gps", jsonString);

    // Matikan LED
    digitalWrite(LED_BUILTIN, HIGH);

    // Update waktu terakhir update GPS
    lastGpsUpdate = millis();
  }
}
