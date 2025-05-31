#pragma once
#include <Arduino.h>

struct LoraPacket {
    String raw;
    uint8_t from_addr;
    long rssi;
    float snr;
};

bool loraSetup();
bool loraReceive(LoraPacket& pkt);
bool decodeAndPublishLSE01(const LoraPacket& pkt);