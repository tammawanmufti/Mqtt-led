#ifndef SIM_SETUP_H
#define SIM_SETUP_H

#define TINY_GSM_MODEM_SIM800

#include <TinyGsmClient.h>

extern TinyGsm modem;

void setupSIM800L();

#endif