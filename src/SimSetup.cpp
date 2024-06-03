#include <Arduino.h>
#include "SimSetup.h"

SIM800L sim800l(D5, D6); // RX, TX

void setupSIM800L() {
  sim800l.begin(9600);

  if (sim800l.init()) {
    Serial.println("SIM800L Initialized");

    const char* apn = ""; 
    const char* user = "";
    const char* pass = "";

    if (sim800l.connectGPRS(apn, user, pass)) {
      Serial.println("GPRS Connected");

    } else {
      Serial.println("Failed to connect GPRS");
    }
  } else {
    Serial.println("Failed to initialize SIM800L");
  }
}
