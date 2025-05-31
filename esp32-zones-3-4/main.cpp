#include <WiFi.h>
#include <PubSubClient.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include "config.h"
#if USE_DHT22
  #include <DHT.h>
  #define DHTTYPE DHT22
  DHT dht(DHT_PIN, DHTTYPE);
#endif

IPAddress local_IP(FIXED_IP);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

const char* zones[] = {ZONE_A, ZONE_B};
const char* ldr_names[] = {"ldr1", "ldr2"};

AsyncWebServer server(API_PORT);
WiFiClient espClient;
PubSubClient mqtt(espClient);

struct ZoneState {
  float ldr;
  float temp;
  bool pump;
  unsigned long pumpUntil;
} zoneStates[2];

void setupWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.setHostname(WIFI_HOSTNAME);
  WiFi.config(local_IP, gateway, subnet);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) delay(500);
  Serial.print("WiFi connected. IP: ");
  Serial.println(WiFi.localIP());
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  String t = String(topic);
  String p((char*)payload, length);
  for (int i = 0; i < 2; i++) {
    String topicStart = String("oliveraie/zones/") + zones[i] + "/irrigation/start";
    String topicStop = String("oliveraie/zones/") + zones[i] + "/irrigation/stop";
    if (t == topicStart) {
      int duree = p.toInt();
      Serial.printf("[MQTT] Start %s: %d min\n", zones[i], duree);
      digitalWrite(i == 0 ? POMPE_A_PIN : POMPE_B_PIN, HIGH);
      zoneStates[i].pump = true;
      zoneStates[i].pumpUntil = millis() + duree * 60000UL;
    }
    if (t == topicStop) {
      digitalWrite(i == 0 ? POMPE_A_PIN : POMPE_B_PIN, LOW);
      zoneStates[i].pump = false;
      zoneStates[i].pumpUntil = 0;
      Serial.printf("[MQTT] Stop %s\n", zones[i]);
    }
  }
}

void reconnectMQTT() {
  while (!mqtt.connected()) {
    if (mqtt.connect(WIFI_HOSTNAME, MQTT_USER, MQTT_PASSWORD)) {
      for (int i = 0; i < 2; i++) {
        String base = String("oliveraie/zones/") + zones[i];
        mqtt.subscribe((base + "/irrigation/start").c_str());
        mqtt.subscribe((base + "/irrigation/stop").c_str());
      }
    } else {
      delay(2000);
    }
  }
}
void mqttLoop() {
  if (!mqtt.connected()) reconnectMQTT();
  mqtt.loop();
}

void readSensors() {
  zoneStates[0].ldr = analogRead(LDR_A_PIN);
  zoneStates[1].ldr = analogRead(LDR_B_PIN);
#if USE_DHT22
  float t = dht.readTemperature();
  for (int i = 0; i < 2; i++) zoneStates[i].temp = t;
#endif
}

void publishSensors() {
  for (int i = 0; i < 2; i++) {
    String capteur = String("lse01_") + String(i+3); // Attention : zones 3-4 → lse01_3, lse01_4
    mqtt.publish((String("oliveraie/capteurs/") + capteur + "/temperature").c_str(),
                 String(zoneStates[i].temp, 1).c_str(), true);
    mqtt.publish((String("oliveraie/capteurs/") + capteur + "/" + ldr_names[i]).c_str(),
                 String(zoneStates[i].ldr, 0).c_str(), true);
  }
}

void handlePumps() {
  for (int i = 0; i < 2; i++) {
    if (zoneStates[i].pump && zoneStates[i].pumpUntil > 0 && millis() > zoneStates[i].pumpUntil) {
      digitalWrite(i == 0 ? POMPE_A_PIN : POMPE_B_PIN, LOW);
      zoneStates[i].pump = false;
      zoneStates[i].pumpUntil = 0;
      mqtt.publish((String("oliveraie/zones/") + zones[i] + "/status").c_str(), "arret");
    }
  }
}

void setupAPI() {
  server.on("/api/status", HTTP_GET, [](AsyncWebServerRequest *request){
    DynamicJsonDocument doc(256);
    JsonArray arr = doc.to<JsonArray>();
    for (int i = 0; i < 2; i++) {
      JsonObject z = arr.createNestedObject();
      z["zone"] = zones[i];
      z["ldr"] = zoneStates[i].ldr;
      z["temperature"] = zoneStates[i].temp;
      z["pump"] = zoneStates[i].pump;
    }
    String out;
    serializeJson(doc, out);
    request->send(200, "application/json", out);
  });

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
    digitalWrite(idx == 0 ? POMPE_A_PIN : POMPE_B_PIN, HIGH);
    zoneStates[idx].pump = true;
    zoneStates[idx].pumpUntil = millis() + minutes * 60000UL;
    request->send(200, "application/json", "{\"ok\":true,\"msg\":\"Irrigation locale démarrée\"}");
  });

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    String html = "<h2>ESP32 Irrigation Zones</h2><ul>";
    for(int i=0;i<2;i++){
      html += "<li><b>" + String(zones[i]) + "</b> | LDR: " + String(zoneStates[i].ldr)
            + " | Temp: " + String(zoneStates[i].temp)
            + " | Pompe: " + (zoneStates[i].pump ? "ON" : "OFF")
            + " <form method='post' action='/manual' style='display:inline'>"
            + "<input type='hidden' name='zone' value='" + String(zones[i]) + "'>"
            + "<input type='number' name='minutes' min='1' max='60' value='5'>"
            + "<button type='submit'>Irriguer</button></form></li>";
    }
    html += "</ul>";
    request->send(200, "text/html", html);
  });

  server.on("/manual", HTTP_POST, [](AsyncWebServerRequest *request){
    if (!request->hasParam("zone", true) || !request->hasParam("minutes", true)) {
      request->send(400, "text/html", "Zone/minutes requis");
      return;
    }
    String zone = request->getParam("zone", true)->value();
    int idx = -1;
    for (int i=0; i<2; i++) if(zone == zones[i]) idx = i;
    int minutes = request->getParam("minutes", true)->value().toInt();
    if(idx>=0 && minutes>0){
      digitalWrite(idx == 0 ? POMPE_A_PIN : POMPE_B_PIN, HIGH);
      zoneStates[idx].pump = true;
      zoneStates[idx].pumpUntil = millis() + minutes*60000UL;
    }
    request->sendHeader("Location", "/");
    request->send(303);
  });

  server.begin();
}

void setup() {
  Serial.begin(115200);
  pinMode(POMPE_A_PIN, OUTPUT); digitalWrite(POMPE_A_PIN, LOW);
  pinMode(POMPE_B_PIN, OUTPUT); digitalWrite(POMPE_B_PIN, LOW);
  pinMode(LDR_A_PIN, INPUT);
  pinMode(LDR_B_PIN, INPUT);
#if USE_DHT22
  dht.begin();
#endif
  setupWiFi();
  mqtt.setServer(MQTT_BROKER, MQTT_PORT);
  mqtt.setCallback(mqttCallback);
  setupAPI();
}

unsigned long lastSensors = 0;
unsigned long lastPub = 0;
void loop() {
  mqttLoop();
  handlePumps();

  unsigned long now = millis();
  if (now - lastSensors > 5000) {
    readSensors();
    lastSensors = now;
  }
  if (now - lastPub > 10000) {
    publishSensors();
    lastPub = now;
  }
}