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
  
  void startSTA();
  void loopRestSTA();


  void setDataProvider(std::function<std::string()> measure);

private:
    WebServer _server;
    const char* _ap_ssid = "esp32_best";
    const char* _ap_password = "supersecret";


  std::string _sta_ssid = "Lab104IoT";
  std::string _sta_password = "SuperTrudneHaslo1";

  // std::string _sta_ssid = "";
  // std::string _sta_password = "";


  std::function<std::string()> _measure;

  std::string parseDataToJson(const std::string& data);
  void sendPostRequest(const std::string& body);

  const char* _url_rest = "http://156.17.45.171:3000/api/v1/iot/data";
};


