#ifndef MQTTSETUP_H
#define MQTTSETUP_H

#include <PubSubClient.h>

extern PubSubClient client;

void setupMQTT();
void reconnect();
void callback(char* topic, byte* payload, unsigned int length);

#endif // MQTTSETUP_H
