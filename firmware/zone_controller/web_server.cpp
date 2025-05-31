#include "web_server.h"
#include "config.h"
#include <WebServer.h>

static WebServer server(HTTP_PORT);
static CapteursZone* capteurs_p = nullptr;
static int* mode_p = nullptr;
static const char* ver = nullptr;

void handleRoot() {
    String html = "<h2>Zone Controller ESP32-S2</h2>";
    html += "<ul>";
    html += "<li>Firmware: " + String(ver) + "</li>";
    html += "<li>Humidité sol: " + String(capteurs_p->humidite_sol,2) + "%</li>";
    html += "<li>EC: " + String(capteurs_p->ec_sol,2) + " mS/cm</li>";
    html += "<li>Temp: " + String(capteurs_p->temp_sol,2) + " °C</li>";
    html += "<li>LDR1: " + String(capteurs_p->ldr1,2) + "</li>";
    html += "<li>LDR2: " + String(capteurs_p->ldr2,2) + "</li>";
    html += "<li>Mode: " + String(*mode_p == 0 ? "Automatique" : "Manuel") + "</li>";
    html += "</ul>";
    html += "<form method='POST' action='/irrigate'><button type='submit'>Irrigation manuelle 5min</button></form>";
    server.send(200, "text/html", html);
}

void handleIrrigate() {
    // Action manuelle : start pump 5min
    pumpStart(5);
    *mode_p = 1;
    server.sendHeader("Location", "/", true);
    server.send(302, "text/plain", "");
}

void webServerSetup(const char* fw_ver, CapteursZone* capteurs, int* mode) {
    ver = fw_ver; capteurs_p = capteurs; mode_p = mode;
    server.on("/", handleRoot);
    server.on("/irrigate", HTTP_POST, handleIrrigate);
    server.begin();
}

void webServerLoop() {
    server.handleClient();
}