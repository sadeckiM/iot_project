#pragma once

#include <Arduino.h>
#include <WebServer.h>

class ServerManager {
public:
    ServerManager() = default;
    void startAP();
    void stopAP();
    bool loopAP();
    IPAddress getIP() const;
private:
    WebServer _server;
    const char* _ap_ssid = "esp32_best";
    const char* _ap_password = "supersecret";
};
