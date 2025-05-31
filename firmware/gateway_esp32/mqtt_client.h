#pragma once
#include <Arduino.h>

bool mqttSetup();
void mqttLoop();
bool mqttPublish(const String& topic, const String& payload);