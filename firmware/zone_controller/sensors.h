#pragma once
#include <Arduino.h>

void sensorsSetup();
void lireCapteursMQTT(float& humidite, float& ec);
void lireCapteursLocaux(float& temp, float& ldr1, float& ldr2);