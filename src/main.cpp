#include "esp32-hal-gpio.h"
#include <Arduino.h>

const uint8_t LED = 2;

void setup() {
    Serial.begin(115200);
    Serial.println("ESP32 gotowe!");
    pinMode(LED, OUTPUT);
}

void loop() {
    digitalWrite(LED, HIGH);
    delay(500);
    digitalWrite(LED, LOW);
    delay(500);
}
