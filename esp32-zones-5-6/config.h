#pragma once

#define ZONE_A "zone5"
#define ZONE_B "zone6"

#define WIFI_SSID      "TonWifi"
#define WIFI_PASSWORD  "TonMotDePasse"
#define WIFI_HOSTNAME  "esp32-zone5-6"
#define FIXED_IP       192,168,1,103

#define MQTT_BROKER    "192.168.1.99"
#define MQTT_PORT      1883
#define MQTT_USER      "mqtt_user"
#define MQTT_PASSWORD  "mqtt_pass"

#define API_PORT       80

#define LDR_A_PIN      34
#define LDR_B_PIN      35
#define DHT_PIN        27
#define POMPE_A_PIN    25  // Relais zone5
#define POMPE_B_PIN    26  // Relais zone6

#define USE_DHT22      true