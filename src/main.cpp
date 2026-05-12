#include <Arduino.h>
#include "DiodeManager.hh"
#include "SensorManager.hh"
#include "ServerManager.hh"

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

    server.setDataProvider([]() -> std::string {
      return sensor.measure();
    });
}

void loop() {
    server.startAP();
    // Serial.println(sensor.measure().c_str());
    Serial.println(server.getIP());
    while(server.loopAP()) {
      //Serial.println("Jestem tu");
        yield();
    }
  server.stopAP();
  server.startSTA();

  while (true) {
    server.loopRestSTA();
  }
}
