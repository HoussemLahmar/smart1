#pragma once
#include "config.h"

struct InferenceResult {
    bool irriguer;
    uint8_t duree_minutes;
    String priorite;
};

struct CapteursZone;
InferenceResult runModelIA(const CapteursZone& capteurs);