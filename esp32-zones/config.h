#pragma once

// === Zones gérées par cet ESP ===
#define ZONE_A "zone1"
#define ZONE_B "zone2"

#define WIFI_SSID      "TonWifi"
#define WIFI_PASSWORD  "TonMotDePasse"
#define WIFI_HOSTNAME  "esp32-zone1-2"
#define FIXED_IP       192,168,1,101   // Adapter pour chaque ESP

#define MQTT_BROKER    "192.168.1.99"
#define MQTT_PORT      1883
#define MQTT_USER      "mqtt_user"
#define MQTT_PASSWORD  "mqtt_pass"

#define API_PORT       80

// === Brochage ===
#define LDR_A_PIN      34  // ADC1_CH6
#define LDR_B_PIN      35  // ADC1_CH7
#define DHT_PIN        27  // DHT22, adapter selon montage
#define POMPE_A_PIN    25  // Relais sortie zone A
#define POMPE_B_PIN    26  // Relais sortie zone B

#define USE_DHT22      true