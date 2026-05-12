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


  std::string _sta_ssid = "IoT";
  std::string _sta_password = "supersecret";

  // std::string _sta_ssid = "";
  // std::string _sta_password = "";


  std::function<std::string()> _measure;

  std::string parseDataToJson(const std::string& data);
  void sendPostRequest(const std::string& body);

  const char* _url_rest = "http://10.237.211.124:3000/api/v1/iot/data";
};


