#ifndef SERVER_MANAGER_HH
#define SERVER_MANAGER_HH

#include <Arduino.h>
#include <WebServer.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>

class ServerManager {
  WebServer _server;
  const char* _ap_ssid = "esp32_ms_al";
  const char* _ap_password = "hefajstos";

  // std::string _sta_ssid = "Lab104IoT";
  // std::string _sta_password = "SuperTrudneHaslo1";
  std::string _sta_ssid = "KoNaR_T3";
  std::string _sta_password = "konarpany_down";

  /**************************************
  *********DATA FROM EMQX****************
  **************************************/
  const char* mqtt_server = "uea5aa71.ala.eu-central-1.emqxsl.com";
  const int mqtt_port = 8883;
  const char * mqtt_user = "esp32_mic";
  const char * mqtt_pass = "Hefajstos123";
  WiFiClientSecure esp_client;
  PubSubClient mqtt_client;

  // std::string _sta_ssid = "";
  // std::string _sta_password = "";
  // std::function<std::string()> _measure;
  // std::string parseDataToJson(const std::string& data);
  // void sendPostRequest(const std::string& body);
  void connect_to_mqtt();

  // const char* _url_rest = "http://156.17.45.171:3000/api/v1/iot/data";
public:
  ServerManager() {mqtt_client.setClient(esp_client);}
  void startAP();
  void stopAP();
  bool loopAP();
  IPAddress getIP() const;
  void startSTA();
  // void loopRestSTA();
  void loop_mqtt();
  void publish_data(const char* topic, const String& payload);
  // void setDataProvider(std::function<std::string()> measure);
};

#endif
