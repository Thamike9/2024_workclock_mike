#ifndef WIFIMNANAGERWRAPPER_H
#define WIFIMNANAGERWRAPPER_H
#include <Arduino.h>
#include "CustomWiFiManager.h"

class WiFiManagerWrapper {
  private:
    int8_t portalRunning;
    int8_t manualTimeSet;
    int8_t MQTTSet;
    unsigned int  startTime;
    unsigned int  timeout;

    WiFiManager wfm;

    
  public:
    WiFiManagerWrapper();
    void doWiFiManager();
    void init();
    void openPortal();
    int8_t getHours();
    int8_t getMinutes();
    void getMQTT(char *mqtt_url);
    int8_t isManualTimeSet();
    int8_t isMQTTSet();
    void closePortal();
};


#endif