
#define TINY_GSM_MODEM_SIM800
#define MODEM_RX D5
#define MODEM_TX D6

#include <SoftwareSerial.h>
#include <TinyGsmClient.h>

SoftwareSerial serialModem(MODEM_RX, MODEM_TX);

TinyGsm modem(serialModem);

const char apn[] = "internet";
const char user[] = "";
const char pass[] = "";

void setupSIM800L()
{
  serialModem.begin(9600);
  Serial.println("Initializing modem...");
  modem.restart();

  Serial.print("Waiting for network...");
  if (!modem.waitForNetwork())
  {

    Serial.println("gprs connection failed");
    return;
  }
  Serial.println("Network connected");
}
