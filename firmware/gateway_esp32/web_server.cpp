#include "web_server.h"
#include "config.h"
#include <WebServer.h>
#include <WiFi.h>

WebServer server(HTTP_PORT);
const char* version = nullptr;

void handleRoot() {
    String html = "<h2>Gateway LoRaWAN ESP32</h2>";
    html += "<ul>";
    html += "<li>Firmware: " + String(version) + "</li>";
    html += "<li>IP: " + WiFi.localIP().toString() + "</li>";
    html += "<li>MQTT broker: " + String(MQTT_BROKER) + "</li>";
    html += "</ul>";
    server.send(200, "text/html", html);
}

void handleNotFound() {
    server.send(404, "text/plain", "404 Not found");
}

void webServerSetup(const char* fw_version) {
    version = fw_version;
    server.on("/", handleRoot);
    server.onNotFound(handleNotFound);
    server.begin();
    Serial.printf("[OK] Serveur HTTP diagnostics sur port %d\n", HTTP_PORT);
}

void webServerLoop() {
    server.handleClient();
}