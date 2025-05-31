#pragma once

// --- WiFi Configuration ---
#define WIFI_SSID       "VOTRE_SSID"
#define WIFI_PASSWORD   "VOTRE_MDP"
#define WIFI_HOSTNAME   "gateway-lora"

// --- MQTT Configuration ---
#define MQTT_BROKER     "192.168.1.99"
#define MQTT_PORT       1883
#define MQTT_USER       "mqtt_user"
#define MQTT_PASSWORD   "mqtt_pass"
#define MQTT_CLIENT_ID  "gateway_lora_01"
#define MQTT_BASE_TOPIC "oliveraie/capteurs/"

// --- LoRa SX1278 Pins ---
#define LORA_SCK   18
#define LORA_MISO  19
#define LORA_MOSI  23
#define LORA_SS     5
#define LORA_RST   14
#define LORA_DIO0   2
#define LORA_DIO1   4

#define LORA_FREQUENCY  433E6 // 433 MHz

// --- Gateway Network ---
#define GATEWAY_IP     192,168,1,100
#define GATEWAY_MASK   255,255,255,0
#define GATEWAY_GW     192,168,1,1

// --- Diagnostics Web Server ---
#define HTTP_PORT  80

// --- Mode Simulation Capteur (décommenter pour tester sans LSE01 réel) ---
// #define SIMULATE_LSE01

// --- Sécurité ---
#define MAX_WIFI_RETRIES 10
#define MAX_MQTT_RETRIES 5