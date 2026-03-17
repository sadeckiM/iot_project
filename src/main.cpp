#include <Arduino.h>
#include "../lib/Manager/src/DiodeManager.hh"
#include "../lib/Manager/src/SensorManager.hh"
#include "../lib/Manager/src/ServerManager.hh"
const uint8_t LED = 2;
DiodeManager diode;
SensorManager sensor;
ServerManager server;

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
    server.startAP();
    Serial.println(sensor.measure().c_str());
    Serial.println(server.getIP());
    while(server.loopAP()) {
        yield();
    }
}
