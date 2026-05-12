#include "SensorManager.hh"
#include <Arduino.h>

std::string SensorManager::measure() const {
    return std::to_string(esp_random() % 100);
}
