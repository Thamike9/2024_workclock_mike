#include "WiFiManagerWrapper.h"

// Define the custom inputs
WiFiManagerParameter custom_hour_box("hour_text", "Hour:", "12", 2);
WiFiManagerParameter custom_minute_box("minute_text", "Minute:", "00", 2);
WiFiManagerParameter custom_MQTT_box("mqtt_url", "MQTT:", "mqtt.michaelwolf.at", 30);

WiFiManagerWrapper::WiFiManagerWrapper()  {

  this->timeout           = 120; // seconds to run for
  this->startTime         = millis();
  this->portalRunning     = false;
  this->manualTimeSet     = false;
  this->MQTTSet           = false;

}

void WiFiManagerWrapper::init(){
  wfm.setHostname("WordClock");
  wfm.setEnableConfigPortal(false);
  wfm.setConfigPortalBlocking(false);
  wfm.autoConnect();
}


void WiFiManagerWrapper::doWiFiManager(){
  // is auto timeout portal running
  if(portalRunning){
    wfm.process(); // do processing

    if(getHours() != 12 || getMinutes() != 00){
      manualTimeSet = true;
    }

    char* MQTT_url = new char[30 + 1]; 
    getMQTT(MQTT_url);
    if(strcmp(MQTT_url, "mqtt.michaelwolf.at") != 0){
      MQTTSet = true;
    }

    // check for timeout
    if((millis()-startTime) > (timeout*1000)){
      Serial.println("portaltimeout");
      portalRunning = false;
      wfm.stopConfigPortal();
    }
  }
}

void WiFiManagerWrapper::openPortal(){
  wfm.setDarkMode(true);
  std::vector<const char *> menu = {"param", "wifi", "exit"};
  wfm.setMenu(menu);
  // Add custom parameter
  wfm.addParameter(&custom_hour_box);
  wfm.addParameter(&custom_minute_box);
  wfm.addParameter(&custom_MQTT_box);

  // is configuration portal requested?
  if(!portalRunning) {
    Serial.println("Button Pressed, Starting Config Portal");
    wfm.setConfigPortalBlocking(false);
    wfm.startConfigPortal("Wordclock_by_Mike");
    portalRunning = true;
    startTime = millis();
  }
}

int8_t WiFiManagerWrapper::getMinutes(){
  //Serial.print("custom_minute_box: ");
  //Serial.println(custom_minute_box.getValue());
  return atoi(custom_minute_box.getValue());
}

int8_t WiFiManagerWrapper::getHours(){
  //Serial.print("custom_hour_box: ");
  //Serial.println(custom_hour_box.getValue());
  return atoi(custom_hour_box.getValue());
}

void WiFiManagerWrapper::getMQTT(char *mqtt_url){
  //Serial.print("custom_MQTT_box: ");
  //Serial.println(custom_MQTT_box.getValue());
  String s = custom_MQTT_box.getValue();
  strcpy(mqtt_url, s.c_str()); 
}

int8_t WiFiManagerWrapper::isMQTTSet(){
  return MQTTSet;
}

int8_t WiFiManagerWrapper::isManualTimeSet(){
  return manualTimeSet;
}

void WiFiManagerWrapper::closePortal(){
  Serial.println("Closing Portal");

  if(portalRunning){
    Serial.println("Stop wfm");
    portalRunning = false;
    wfm.stopConfigPortal();
  }

  manualTimeSet = false; 
  MQTTSet = false; 
  custom_hour_box.setValue("12", 3);
  custom_minute_box.setValue("00", 3);
  custom_MQTT_box.setValue("mqtt.michaelwolf.at", 30);

}


