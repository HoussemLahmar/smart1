#include <WiFi.h>
#include <PubSubClient.h>
#include <ESPAsyncWebServer.h>
#include "config.h"

// Zones gérées par cet ESP
const char* zones[] = {ESP_ZONE_1, ESP_ZONE_2};
AsyncWebServer server(API_PORT);

// Variables d'état
float humidite[2]    = {0, 0};
float temperature[2] = {0, 0};
bool arrosage[2]     = {false, false};

// --- WiFi ---
void setupWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.setHostname(WIFI_HOSTNAME);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) delay(500);
}

// --- MQTT ---
WiFiClient espClient;
PubSubClient mqtt(espClient);

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  String t = String(topic);
  String p = "";
  for (unsigned int i = 0; i < length; i++) p += (char)payload[i];
  // Gestion des commandes d'irrigation
  for (int i = 0; i < 2; i++) {
    String topicStart = String("oliveraie/zones/") + zones[i] + "/irrigation/start";
    if (t == topicStart) {
      int duree = p.toInt();
      arrosage[i] = true;
      // Démarre l'irrigation sur la zone i (matériel)
      // ... code à compléter ...
      // Arrêt automatique après la durée
      unsigned long fin = millis() + duree * 60000UL;
      while (millis() < fin) { delay(100); }
      arrosage[i] = false;
      mqtt.publish((String("oliveraie/zones/") + zones[i] + "/status").c_str(), "arret");
    }
  }
}

void reconnectMQTT() {
  while (!mqtt.connected()) {
    if (mqtt.connect(WIFI_HOSTNAME, MQTT_USER, MQTT_PASSWORD)) {
      // Souscrire aux topics d'irrigation
      for (int i = 0; i < 2; i++)
        mqtt.subscribe((String("oliveraie/zones/") + zones[i] + "/irrigation/start").c_str());
    } else {
      delay(2000);
    }
  }
}

void mqttLoop() {
  if (!mqtt.connected()) reconnectMQTT();
  mqtt.loop();
}

// --- API REST locale ---
void setupAPI() {
  server.on("/api/status", HTTP_GET, [](AsyncWebServerRequest *request){
    String json = "[";
    for (int i=0; i<2; i++) {
      if (i>0) json += ",";
      json += "{";
      json += "\"zone\":\"" + String(zones[i]) + "\",";
      json += "\"humidite\":" + String(humidite[i]) + ",";
      json += "\"temperature\":" + String(temperature[i]) + ",";
      json += "\"arrosage\":" + String(arrosage[i] ? "true" : "false");
      json += "}";
    }
    json += "]";
    request->send(200, "application/json", json);
  });

  // Commande d'irrigation locale
  server.on("/api/irrigate", HTTP_POST, [](AsyncWebServerRequest *request){
    if (!request->hasParam("zone", true) || !request->hasParam("minutes", true)) {
      request->send(400, "application/json", "{\"error\":\"zone et minutes requis\"}");
      return;
    }
    String zone = request->getParam("zone", true)->value();
    int idx = -1;
    for (int i=0; i<2; i++) if(zone == zones[i]) idx = i;
    if(idx<0){ request->send(404, "application/json", "{\"error\":\"zone inconnue\"}"); return; }
    int minutes = request->getParam("minutes", true)->value().toInt();
    arrosage[idx] = true;
    // ... Démarre l'irrigation matérielle ici ...
    request->send(200, "application/json", "{\"ok\":true,\"msg\":\"Irrigation local démarrée\"}");
    // Arrêt asynchrone à implémenter
  });

  server.begin();
}

// --- Simulation mesures et publication MQTT ---
void publierMesures(int i) {
  // ... Remplacer par lecture réelle capteur ...
  humidite[i] = random(20, 70);
  temperature[i] = random(15, 35);

  String topicH = String("oliveraie/capteurs/lse01_") + String(i+1) + "/humidite";
  String topicT = String("oliveraie/capteurs/lse01_") + String(i+1) + "/temperature";
  String payloadH = String(humidite[i]);
  String payloadT = String(temperature[i]);
  mqtt.publish(topicH.c_str(), payloadH.c_str());
  mqtt.publish(topicT.c_str(), payloadT.c_str());
}

void setup() {
  Serial.begin(115200);
  setupWiFi();
  mqtt.setServer(MQTT_BROKER, MQTT_PORT);
  mqtt.setCallback(mqttCallback);
  setupAPI();
}

void loop() {
  mqttLoop();
  // Simulation : publier mesures toutes les 30s
  static unsigned long last = 0;
  if (millis() - last > 30000) {
    last = millis();
    for (int i=0; i<2; i++) publierMesures(i);
  }
}