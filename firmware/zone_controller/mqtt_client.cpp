#include <WiFi.h>
#include <PubSubClient.h>
#include "config.h"

// --- WiFi ---
void setupWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.setHostname(WIFI_HOSTNAME);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("[WiFi] Connexion à ");
  Serial.println(WIFI_SSID);

  int tries = 0;
  while (WiFi.status() != WL_CONNECTED && tries < MAX_WIFI_RETRIES) {
    delay(1000);
    Serial.print(".");
    tries++;
  }
  Serial.println();
  if (WiFi.status() == WL_CONNECTED)
    Serial.println("[WiFi] Connecté !");
  else {
    Serial.println("[WiFi] Échec connexion, reboot...");
    ESP.restart();
  }
}

// --- MQTT ---
WiFiClient espClient;
PubSubClient mqtt(espClient);

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  String t = String(topic);
  String p = "";
  for (unsigned int i = 0; i < length; i++) p += (char)payload[i];
  Serial.printf("[MQTT] Message reçu sur %s: %s\n", topic, p.c_str());

  // Exemple : gestion de commandes
  if (t.endsWith("irrigation/start")) {
    // TODO : déclencher irrigation sur réception
    Serial.println("[MQTT] Commande d'irrigation reçue !");
  }
}

void setupMQTT() {
  mqtt.setServer(MQTT_BROKER, MQTT_PORT);
  mqtt.setCallback(mqttCallback);
}

void reconnectMQTT() {
  while (!mqtt.connected()) {
    Serial.print("[MQTT] Connexion au broker...");
    if (mqtt.connect(MQTT_CLIENT_ID, MQTT_USER, MQTT_PASSWORD)) {
      Serial.println("OK");
      // Souscrire aux commandes distantes
      mqtt.subscribe(MQTT_BASE_TOPIC "irrigation/#");
      mqtt.subscribe(MQTT_BASE_TOPIC "mode/#");
    } else {
      Serial.print("Échec, rc=");
      Serial.print(mqtt.state());
      Serial.println(" nouvelle tentative dans 2s.");
      delay(2000);
    }
  }
}

void mqttLoop() {
  if (!mqtt.connected()) {
    reconnectMQTT();
  }
  mqtt.loop();
}

// Publication d'un message sur un topic
bool mqttPublish(const String& topic, const String& payload) {
  if (!mqtt.connected()) reconnectMQTT();
  bool ok = mqtt.publish(topic.c_str(), payload.c_str());
  Serial.printf("[MQTT] Pub %s : %s (%s)\n", topic.c_str(), payload.c_str(), ok ? "OK" : "FAIL");
  return ok;
}

// --- Exemple d'utilisation dans main.cpp ---
/*
void setup() {
  Serial.begin(115200);
  setupWiFi();
  setupMQTT();
}

void loop() {
  mqttLoop();

  // Exemple de publication toutes les 10 secondes
  static unsigned long lastPub = 0;
  if (millis() - lastPub > 10000) {
    lastPub = millis();
    mqttPublish(MQTT_BASE_TOPIC "status", "{\"etat\":\"OK\"}");
  }

  delay(10);
}
*/