#include <Arduino.h>

void setup() {
    Serial.begin(115200);
    Serial.println("ESP32 gotowe!");
}

void loop() {
    Serial.println("Zyje...");
    delay(1000);
}
