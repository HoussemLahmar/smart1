#include "mqtt_client.h"
#include "config.h"
#include <WiFi.h>
#include <PubSubClient.h>

WiFiClient espClient;
PubSubClient mqtt(espClient);

bool mqttSetup() {
    mqtt.setServer(MQTT_BROKER, MQTT_PORT);
    for (int i=0; i<MAX_MQTT_RETRIES; ++i) {
        if (mqtt.connect(MQTT_CLIENT_ID, MQTT_USER, MQTT_PASSWORD)) {
            Serial.println("[OK] Connecté au broker MQTT.");
            return true;
        }
        delay(1000);
        Serial.println("[MQTT] Tentative de connexion...");
    }
    return false;
}

void mqttLoop() {
    if (!mqtt.connected()) {
        mqttSetup();
    }
    mqtt.loop();
}

bool mqttPublish(const String& topic, const String& payload) {
    if (!mqtt.connected()) return false;
    return mqtt.publish(topic.c_str(), payload.c_str());
}