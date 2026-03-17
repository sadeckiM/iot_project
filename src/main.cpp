#include <Arduino.h>
#include "../lib/Manager/src/DiodeManager.hh"
#include "../lib/Manager/src/SensorManager.hh"
const uint8_t LED = 2;
DiodeManager diode;
SensorManager sensor;

void setup() {
    Serial.begin(115200);
    Serial.println("ESP32 gotowe!");
    diode.setup();

    while(!Serial) {
        delay(10);
    }

    delay(1000);
}

void loop() {
    Serial.println(sensor.measure().c_str());
    diode.setOn();
    delay(500);
    diode.setOff();
    delay(500);
}
