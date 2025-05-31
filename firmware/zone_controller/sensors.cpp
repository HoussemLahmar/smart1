#include "sensors.h"
#include "config.h"
#ifdef SIMULATE_SENSORS
#include <stdlib.h>
#endif

void sensorsSetup() {
    pinMode(LDR1_PIN, INPUT);
    pinMode(LDR2_PIN, INPUT);
    pinMode(DS18B20_PIN, INPUT);
}

// Lecture MQTT : à intégrer selon réception topic capteurs LSE01
void lireCapteursMQTT(float& humidite, float& ec) {
#ifdef SIMULATE_SENSORS
    humidite = 20.0 + rand() % 60;
    ec = 0.5 + ((float)(rand() % 10))/10.0;
#else
    // TODO : récupérer valeur reçue sur topic MQTT
    humidite = 35.0; ec = 1.0; // Dummy
#endif
}

// Lecture capteurs locaux
void lireCapteursLocaux(float& temp, float& ldr1, float& ldr2) {
#ifdef SIMULATE_SENSORS
    temp = 15.0 + rand()%15;
    ldr1 = 400 + rand()%200;
    ldr2 = 400 + rand()%200;
#else
    // TODO : lire DS18B20 (OneWire), analogRead LDR
    temp = 22.0; ldr1 = analogRead(LDR1_PIN); ldr2 = analogRead(LDR2_PIN);
#endif
}