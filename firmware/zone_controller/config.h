#pragma once

// --- Zone ID (modifier selon la carte flashée) ---
#define ZONE_ID           1     // 1: Zone 1-2, 2: Zone 3-4, etc.
#define ZONE_NAME         "zone1"

// --- WiFi ---
#define WIFI_SSID         "VOTRE_SSID"
#define WIFI_PASSWORD     "VOTRE_MDP"
#define WIFI_HOSTNAME     "zone-controller-1"

// --- MQTT ---
#define MQTT_BROKER       "192.168.1.99"
#define MQTT_PORT         1883
#define MQTT_USER         "mqtt_user"
#define MQTT_PASSWORD     "mqtt_pass"
#define MQTT_CLIENT_ID    "zone_controller_1"
#define MQTT_BASE_TOPIC   "oliveraie/zones/" ZONE_NAME "/"

// --- Capteurs locaux ---
#define LDR1_PIN          32
#define LDR2_PIN          33
#define DS18B20_PIN       27

// --- Pompe (PWM/Relay) ---
#define PUMP_PIN          25     // PWM ou relay
#define PWM_FREQ          1000
#define PWM_CHANNEL       0

// --- Mode simulation capteurs (pour dev/test) ---
// #define SIMULATE_SENSORS

// --- HTTP ---
#define HTTP_PORT         80

// --- IA ---
#define IA_ENABLED        1
#define IRRIGATION_MAX_MINUTES  120
#define IRRIGATION_MIN_THRESHOLD  25.0 // % humidité sol
#define IRRIGATION_OPTIMAL_MIN   40.0 // zone sans irrigation
#define IRRIGATION_OPTIMAL_MAX   60.0

// --- Sécurité ---
#define MAX_WIFI_RETRIES  10
#define MAX_MQTT_RETRIES  5