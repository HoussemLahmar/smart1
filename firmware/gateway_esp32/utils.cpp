#include "utils.h"
#include "config.h"
#include <WiFi.h>

bool setupWiFi() {
    WiFi.mode(WIFI_STA);
    WiFi.config(IPAddress(GATEWAY_IP), IPAddress(GATEWAY_GW),
                IPAddress(GATEWAY_MASK));
    WiFi.setHostname(WIFI_HOSTNAME);

    Serial.print("[WiFi] Connexion à ");
    Serial.println(WIFI_SSID);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    int tries = 0;
    while (WiFi.status() != WL_CONNECTED && tries < MAX_WIFI_RETRIES) {
        delay(1000);
        Serial.print(".");
        tries++;
    }
    Serial.println();
    return WiFi.status() == WL_CONNECTED;
}

void printBanner() {
    Serial.println("====== Gateway LoRaWAN ESP32 v1.0 ======");
    Serial.println("  (c) 2025 SmartIrrigation");
    Serial.println("========================================");
}