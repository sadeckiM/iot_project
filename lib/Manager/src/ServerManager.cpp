#include "ServerManager.hh"
#include "IPAddress.h"
#include <WiFi.h> 
#include "webpage/MainPageAP.hh"
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
}

void ServerManager::loopRestSTA() {
  delay(1000);

  std::string data = _measure();
  Serial.println(data.c_str());

  Serial.println(data.c_str());
  sendPostRequest(parseDataToJson(data));
}

void ServerManager::setDataProvider(std::function<std::string()> measure) {
  _measure = measure;
}

std::string ServerManager::parseDataToJson(const std::string& payload) {
  return "{\"device\": \"********\", \"sensor\": \"random\", \"payload\": \"" + payload + "\"}";
}

void ServerManager::sendPostRequest(const std::string& body) {
  HTTPClient http;

  http.begin(_url_rest);
  http.addHeader("Content-Type", "application/json");

  int code = http.POST(body.c_str());

  if(code == 200) {
    Serial.println(http.getString());
  } else {
    Serial.println(code);
  }

  http.end();
}