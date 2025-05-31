#include <Arduino.h>
#include "config.h"
#include "lora_handler.h"
#include "mqtt_client.h"
#include "web_server.h"
#include "utils.h"

// --- Infos système ---
const char* FW_VERSION = "1.0.0";

void setup() {
    Serial.begin(115200);
    delay(1000);
    printBanner();

    // Setup WiFi
    if (!setupWiFi()) {
        Serial.println("[ERREUR] Impossible de connecter le WiFi. Redémarrage...");
        ESP.restart();
    }

    // Setup MQTT
    if (!mqttSetup()) {
        Serial.println("[ERREUR] Impossible de connecter MQTT. Redémarrage...");
        ESP.restart();
    }

    // Setup LoRa
    if (!loraSetup()) {
        Serial.println("[ERREUR] Initialisation LoRa échouée.");
        while(true) delay(1000);
    }

    // Setup HTTP Server
    webServerSetup(FW_VERSION);

    Serial.println("[OK] Démarrage terminé.");
}

void loop() {
    mqttLoop();
    webServerLoop();

    // Réception LoRa -> Décodage -> MQTT
    LoraPacket pkt;
    if (loraReceive(pkt)) {
        Serial.printf("[LoRa] Trame reçue : %s\n", pkt.raw.c_str());
        if (decodeAndPublishLSE01(pkt)) {
            Serial.println("[MQTT] Données capteur publiées.");
        } else {
            Serial.println("[ERREUR] Décodage ou publication échouée.");
        }
    }

    delay(10);
}