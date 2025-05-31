#pragma once
#include <Arduino.h>

void pumpSetup();
void pumpStart(uint8_t duree_minutes);
void pumpStop();
bool pumpIsActive();