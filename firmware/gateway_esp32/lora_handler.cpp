#include "lora_handler.h"
#include "config.h"
#include "mqtt_client.h"
#include <SPI.h>
#include <LoRa.h> // Librairie Sandeep Mistry

bool loraSetup() {
    LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);
    if (!LoRa.begin(LORA_FREQUENCY)) {
        Serial.println("[ERREUR] LoRa non détecté.");
        return false;
    }
    Serial.println("[OK] LoRa SX1278 initialisé.");
    return true;
}

bool loraReceive(LoraPacket& pkt) {
    int packetSize = LoRa.parsePacket();
    if (packetSize == 0) return false;

    pkt.raw = "";
    pkt.from_addr = 0;
    pkt.rssi = LoRa.packetRssi();
    pkt.snr = LoRa.packetSnr();

    while (LoRa.available()) {
        pkt.raw += (char)LoRa.read();
    }
    // Optionnel: extraire adresse (ex: LSE01-1 = 1)
    if (pkt.raw.startsWith("LSE01-")) {
        pkt.from_addr = pkt.raw.substring(6,7).toInt();
    }
    return true;
}

// Décodage basique LSE01 LoRaWAN (Paquet ex: "LSE01-1;H:34;EC:1.10;T:23.5")
bool decodeAndPublishLSE01(const LoraPacket& pkt) {
    String topic_base = String(MQTT_BASE_TOPIC) + "lse01_" + String(pkt.from_addr) + "/";
    float humidite = NAN, ec = NAN, temp = NAN;
    int idxH = pkt.raw.indexOf("H:");
    int idxEC = pkt.raw.indexOf("EC:");
    int idxT = pkt.raw.indexOf("T:");

    if (idxH > 0) humidite = pkt.raw.substring(idxH+2, pkt.raw.indexOf(';', idxH)).toFloat();
    if (idxEC > 0) ec = pkt.raw.substring(idxEC+3, pkt.raw.indexOf(';', idxEC)).toFloat();
    if (idxT > 0) temp = pkt.raw.substring(idxT+2).toFloat();

    bool ok = true;
    if (!isnan(humidite)) ok &= mqttPublish(topic_base + "humidite", String(humidite,2));
    if (!isnan(ec))       ok &= mqttPublish(topic_base + "ec", String(ec,2));
    if (!isnan(temp))     ok &= mqttPublish(topic_base + "temperature", String(temp,2));
    return ok;
}