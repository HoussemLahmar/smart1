#include "pump_control.h"
#include "config.h"

static bool etat_pompe = false;
static unsigned long stop_time = 0;

void pumpSetup() {
    pinMode(PUMP_PIN, OUTPUT);
    ledcSetup(PWM_CHANNEL, PWM_FREQ, 8); // 8 bits
    ledcAttachPin(PUMP_PIN, PWM_CHANNEL);
    pumpStop();
}

void pumpStart(uint8_t duree_minutes) {
    ledcWrite(PWM_CHANNEL, 200); // PWM moyen
    etat_pompe = true;
    stop_time = millis() + duree_minutes * 60000UL;
}

void pumpStop() {
    ledcWrite(PWM_CHANNEL, 0);
    etat_pompe = false;
    stop_time = 0;
}

bool pumpIsActive() {
    if (etat_pompe && stop_time > 0 && millis() > stop_time) {
        pumpStop();
        return false;
    }
    return etat_pompe;
}