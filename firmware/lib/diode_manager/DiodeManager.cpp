#include "DiodeManager.hh"

void DiodeManager::setup() {
    pinMode(PIN, OUTPUT);
}

void DiodeManager::setOn() {
    digitalWrite(PIN, HIGH);
}

void DiodeManager::setOff() {
    digitalWrite(PIN, LOW);
}
