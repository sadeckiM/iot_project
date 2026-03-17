#include "ServerManager.hh"
#include "IPAddress.h"
#include <WiFi.h> 

void ServerManager::startAP() {
    WiFi.softAP(_ap_ssid, _ap_password);

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

