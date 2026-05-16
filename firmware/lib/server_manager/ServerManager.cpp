#include "ServerManager.hh"
#include "IPAddress.h"
#include <WiFi.h> 
#include "MainPageAP.hh"
#include <HTTPClient.h>

void ServerManager::startAP() {
    WiFi.softAP(_ap_ssid, _ap_password);
    _server.on("/", [this]() {
        _server.send(200, "text/html", CONFIG_PAGE);
    });

  _server.on("/save", HTTP_POST, [this]() {
    _sta_ssid = _server.arg("ssid").c_str();
    _sta_password = _server.arg("password").c_str();

    _server.send(200, "text/plain", "Credentials received");
  });
    _server.begin();
}

void ServerManager::stopAP() {
    _server.stop();
    WiFi.softAPdisconnect(true);
}

bool ServerManager::loopAP() {
    _server.handleClient();
    return _sta_ssid.empty();
}

IPAddress ServerManager::getIP() const {
    return WiFi.softAPIP();
}

void ServerManager::startSTA() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(_sta_ssid.c_str(), _sta_password.c_str());

  delay(10000);

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connection failed (STA). Rebooting...");

    delay(5000);

    ESP.restart();
  }
  Serial.println("WiFi connected.");
  esp_client.setInsecure();
  mqtt_client.setServer(mqtt_server, mqtt_port);
}

void ServerManager::connect_to_mqtt() {
  while (!mqtt_client.connected()) {
    Serial.println("Trying to connect to MQTT..");
    String client_id = "ESP32_MS_AL-";
    client_id += String(random(0xffff), HEX);

    if (mqtt_client.connect(client_id.c_str(), mqtt_user, mqtt_pass))
      Serial.println("MQTT: connected");
    else {
      Serial.print("MQTT Error code");
      Serial.print(mqtt_client.state());
      Serial.println(". Next attempt in 5 seconds.");
      delay(5000);
    }
  }
}

void ServerManager::loop_mqtt() {
  if (!mqtt_client.connected())
    connect_to_mqtt();
  mqtt_client.loop();
}

void ServerManager::publish_data(const char* topic, const String& payload) {
  if (mqtt_client.connected()) {
    mqtt_client.publish(topic, payload.c_str());
    Serial.println("MQTT: sent packet: " + payload);
  }
}

// void ServerManager::loopRestSTA() {
//   delay(1000);
//
//   std::string data = _measure();
//   Serial.println(data.c_str());
//
//   Serial.println(data.c_str());
//   sendPostRequest(parseDataToJson(data));
// }
//
// void ServerManager::setDataProvider(std::function<std::string()> measure) {
//   _measure = measure;
// }

// std::string ServerManager::parseDataToJson(const std::string& payload) {
//   return "{\"device\": \"********\", \"sensor\": \"random\", \"payload\": \"" + payload + "\"}";
// }
//
// void ServerManager::sendPostRequest(const std::string& body) {
//   HTTPClient http;
//
//   http.begin(_url_rest);
//   http.addHeader("Content-Type", "application/json");
//
//   int code = http.POST(body.c_str());
//
//   if(code == 200) {
//     Serial.println(http.getString());
//   } else {
//     Serial.println(code);
//   }
//
//   http.end();
// }
