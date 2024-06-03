#ifndef SIM800L_H
#define SIM800L_H

#include <Arduino.h>
#include <SoftwareSerial.h>

class SIM800L {
  public:
    SIM800L(int rxPin, int txPin);
    void begin(long baudRate);
    bool sendSMS(const char* phoneNumber, const char* message);
    bool init();
    bool isReady();
    bool connectGPRS(const char* apn, const char* user, const char* pass);
    bool disconnectGPRS();
    String httpGET(const char* url);
    String httpPOST(const char* url, const char* data);
    
  private:
    SoftwareSerial* sim800l;
    int _rxPin;
    int _txPin;
    bool sendATCommand(const char* command, const char* response, unsigned long timeout);
};

#endif