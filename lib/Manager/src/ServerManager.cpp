#include "ServerManager.hh"
#include "IPAddress.h"
#include <WiFi.h> 
#include "webpage/MainPageAP.hh"

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
    return true;
}

IPAddress ServerManager::getIP() const {
    return WiFi.softAPIP();
}

