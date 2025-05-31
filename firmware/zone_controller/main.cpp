#include <Arduino.h>
#include "config.h"
#include "sensors.h"
#include "ml_inference.h"
#include "pump_control.h"
#include "mqtt_client.h"
#include "web_server.h"

const char* FW_VERSION = "1.0.0";

// Etats
enum Mode { AUTOMATIQUE, MANUEL };
Mode mode = AUTOMATIQUE;

struct CapteursZone {
    float humidite_sol, ec_sol, temp_sol, ldr1, ldr2;
    unsigned long derniere_maj;
} capteurs;

// MQTT callback pour commandes distantes
void onMQTTMessage(const String& topic, const String& payload) {
    if (topic.endsWith("irrigation/start")) {
        uint8_t mins = payload.toInt();
        pumpStart(mins);
        mode = MANUEL;
    } else if (topic.endsWith("irrigation/stop")) {
        pumpStop();
        mode = MANUEL;
    } else if (topic.endsWith("mode/auto")) {
        mode = AUTOMATIQUE;
    }
}

// Boucle principale
void loopAutomatique() {
    // Lecture capteurs distants (MQTT) et locaux
    lireCapteursMQTT(capteurs.humidite_sol, capteurs.ec_sol);
    lireCapteursLocaux(capteurs.temp_sol, capteurs.ldr1, capteurs.ldr2);

    // Exécution IA
    if (IA_ENABLED) {
        InferenceResult result = runModelIA(capteurs);
        if (result.irriguer) {
            pumpStart(result.duree_minutes);
        } else {
            pumpStop();
        }
    }
    // Publier statut sur MQTT
    publishStatus(capteurs, mode);
}

void loopManuel() {
    // Rien, attendre instructions utilisateur (web/MQTT)
}

void setup() {
    Serial.begin(115200);
    printBanner();

    if (!setupWiFi()) ESP.restart();
    mqttSetup(onMQTTMessage);
    sensorsSetup();
    pumpSetup();
    webServerSetup(FW_VERSION, &capteurs, &mode);

    Serial.println("[OK] Zone controller prêt.");
}

void loop() {
    mqttLoop();
    webServerLoop();

    static unsigned long last_cycle = 0;
    if (millis() - last_cycle > 15 * 1000) { // toutes les 15s
        last_cycle = millis();
        if (mode == AUTOMATIQUE) loopAutomatique();
        else loopManuel();
    }
    delay(10);
}