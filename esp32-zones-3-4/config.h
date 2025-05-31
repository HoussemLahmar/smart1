#pragma once

#define ZONE_A "zone3"
#define ZONE_B "zone4"

#define WIFI_SSID      "TonWifi"
#define WIFI_PASSWORD  "TonMotDePasse"
#define WIFI_HOSTNAME  "esp32-zone3-4"
#define FIXED_IP       192,168,1,102

#define MQTT_BROKER    "192.168.1.99"
#define MQTT_PORT      1883
#define MQTT_USER      "mqtt_user"
#define MQTT_PASSWORD  "mqtt_pass"

#define API_PORT       80

#define LDR_A_PIN      34
#define LDR_B_PIN      35
#define DHT_PIN        27
#define POMPE_A_PIN    25  // Relais zone3
#define POMPE_B_PIN    26  // Relais zone4

#define USE_DHT22      true