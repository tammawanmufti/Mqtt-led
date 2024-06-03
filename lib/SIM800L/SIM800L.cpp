#include "SIM800L.h"

SIM800L::SIM800L(int rxPin, int txPin) {
  _rxPin = rxPin;
  _txPin = txPin;
  sim800l = new SoftwareSerial(_rxPin, _txPin);
}

void SIM800L::begin(long baudRate) {
  sim800l->begin(baudRate);
}

bool SIM800L::sendATCommand(const char* command, const char* response, unsigned long timeout) {
  sim800l->println(command);
  unsigned long start = millis();
  while (millis() - start < timeout) {
    if (sim800l->available()) {
      if (sim800l->find(response)) {
        return true;
      }
    }
  }
  return false;
}

bool SIM800L::sendSMS(const char* phoneNumber, const char* message) {
  if (!sendATCommand("AT+CMGF=1", "OK", 1000)) return false; // Set SMS mode to text
  sim800l->print("AT+CMGS=\"");
  sim800l->print(phoneNumber);
  sim800l->println("\"");
  delay(100); // Wait for '>'
  sim800l->print(message);
  delay(100);
  sim800l->write(26); // ASCII code for Ctrl+Z
  return sendATCommand("", "OK", 10000); // Wait for SMS to be sent
}

bool SIM800L::init() {
  return sendATCommand("AT", "OK", 1000); // Check if module is responding
}

bool SIM800L::isReady() {
  return sendATCommand("AT+CPAS", "+CPAS: 0", 1000); // Check if module is ready
}

bool SIM800L::connectGPRS(const char* apn, const char* user, const char* pass) {
  if (!sendATCommand("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"", "OK", 2000)) return false;
  String cmd = "AT+SAPBR=3,1,\"APN\",\"";
  cmd += apn;
  cmd += "\"";
  if (!sendATCommand(cmd.c_str(), "OK", 2000)) return false;
  if (user && pass) {
    cmd = "AT+SAPBR=3,1,\"USER\",\"";
    cmd += user;
    cmd += "\"";
    if (!sendATCommand(cmd.c_str(), "OK", 2000)) return false;
    cmd = "AT+SAPBR=3,1,\"PWD\",\"";
    cmd += pass;
    cmd += "\"";
    if (!sendATCommand(cmd.c_str(), "OK", 2000)) return false;
  }
  if (!sendATCommand("AT+SAPBR=1,1", "OK", 3000)) return false;
  return sendATCommand("AT+SAPBR=2,1", "OK", 3000);
}

bool SIM800L::disconnectGPRS() {
  return sendATCommand("AT+SAPBR=0,1", "OK", 3000);
}

String SIM800L::httpGET(const char* url) {
  String cmd = "AT+HTTPPARA=\"URL\",\"";
  cmd += url;
  cmd += "\"";
  if (!sendATCommand(cmd.c_str(), "OK", 2000)) return "";
  if (!sendATCommand("AT+HTTPACTION=0", "OK", 10000)) return "";
  if (!sendATCommand("AT+HTTPREAD", "+HTTPREAD:", 10000)) return "";
  
  String response = "";
  while (sim800l->available()) {
    response += (char)sim800l->read();
  }
  return response;
}

String SIM800L::httpPOST(const char* url, const char* data) {
  String cmd = "AT+HTTPPARA=\"URL\",\"";
  cmd += url;
  cmd += "\"";
  if (!sendATCommand(cmd.c_str(), "OK", 2000)) return "";
  cmd = "AT+HTTPDATA=";
  cmd += strlen(data);
  cmd += ",10000";
  if (!sendATCommand(cmd.c_str(), "DOWNLOAD", 2000)) return "";
  sim800l->print(data);
  delay(100);
  if (!sendATCommand("", "OK", 10000)) return "";
  if (!sendATCommand("AT+HTTPACTION=1", "OK", 10000)) return "";
  if (!sendATCommand("AT+HTTPREAD", "+HTTPREAD:", 10000)) return "";
  
  String response = "";
  while (sim800l->available()) {
    response += (char)sim800l->read();
  }
  return response;
}
