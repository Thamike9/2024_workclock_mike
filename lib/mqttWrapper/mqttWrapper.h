#ifndef MQTT_WRAPPER_H
#define MQTT_WRAPPER_H
#include <Arduino.h>
#include <MQTT.h>
#include <ESP8266WiFi.h>



class mqttWrapper {
  private:
    const char* mqtt_server = "192.168.111.99";
    const char* client_id = "WordClock";
    WiFiClient net;
    MQTTClient client;

    
  public:
    mqttWrapper();
    uint8_t init(const char* mqtt_server, const char* client_id);
    uint8_t sendMQTT(float temp, float humi);
};


#endif