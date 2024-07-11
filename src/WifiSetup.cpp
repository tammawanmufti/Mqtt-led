#include <ESP8266WiFi.h>
#include "WifiSetup.h"

// Definisikan SSID(SSID adalah nama jaringan WiFi)
const char *ssid = "Haihai";
// Definisikan Password(Password adalah kata sandi jaringan WiFi)
const char *password = "12341234";

// Definisikan Fungsi setupWifi
void setupWifi()
{
  // Setelah Serial terbuka, tampilkan pesan "Connecting to :"
  Serial.print("Connecting to :");
  // Tampilkan SSID
  Serial.println(ssid);

  // Set mode WiFi ke STA (Station)
  // (Mode ini digunakan untuk menghubungkan ESP8266 ke jaringan WiFi)
  WiFi.mode(WIFI_STA);
  // Mulai koneksi ke WiFi
  WiFi.begin(ssid, password);

  // Cek apakah ESP8266 terhubung ke WiFi atau tidak
  while (WiFi.status() != WL_CONNECTED)
  {
    // Jika belum terhubung, tampilkan Led kedip
    digitalWrite(LED_BUILTIN, LOW);
    delay(200);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(300);

    // Tampilkan pesan "......." pada Serial Monitor
    Serial.print(".");
  }

  // Setelah terhubung, tampilkan pesan "WiFi connected"
  Serial.println("");
  Serial.println("WiFi connected");
  // Tampilkan pesan "IP address: "
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
