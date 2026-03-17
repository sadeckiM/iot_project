#pragma once

#include <Arduino.h>

class DiodeManager {
    public:
        DiodeManager() = default;
        
        void setup();
        void setOn();
        void setOff();
private:
    static constexpr uint8_t PIN = 2;
};
