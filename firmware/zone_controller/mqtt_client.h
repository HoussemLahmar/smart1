#pragma once
#include <Arduino.h>
typedef void (*MQTTCallback)(const String&, const String&);

bool mqttSetup(MQTTCallback cb);
void mqttLoop();
void publishStatus(const CapteursZone& capteurs, int mode);