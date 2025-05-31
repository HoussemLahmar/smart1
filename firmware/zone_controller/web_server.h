#pragma once
#include <Arduino.h>
struct CapteursZone;
void webServerSetup(const char* fw_ver, CapteursZone* capteurs, int* mode);
void webServerLoop();