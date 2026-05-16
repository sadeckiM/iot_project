#include <Arduino.h>
#include "DiodeManager.hh"
#include "SensorManager.hh"
#include "ServerManager.hh"
#include "microphone.hh"

DiodeManager diode;
SensorManager sensor;
ServerManager server;
Microphone mic(12, 13, 14);

void setup() {
  Serial.begin(SERIAL_BAUD);
  Serial.println("ESP32 gotowe!");
  diode.setup();

  // server.setDataProvider([]() -> std::string {
  //   return sensor.measure();
  // });
  server.startAP();
  Serial.println(server.getIP());

  while (server.loopAP()) {
    yield();
  }
  server.stopAP();
  server.startSTA();
}

void loop() {
  // server.loopRestSTA();
  server.loop_mqtt();
  int32_t samples = mic.read_raw_data_to_buffer();
  if (samples > 0) {
    String payload = mic.get_json_packet("esp32_ms_al");
    server.publish_data("sensor/audio", payload);
  }
  delay(1000);
}
