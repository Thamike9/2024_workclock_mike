#ifndef EEPROM_WRAPPER_H
#define EEPROM_WRAPPER_H
#include <Arduino.h>
#include <EEPROM.h>


class eepromWrapper {
  private:
    String mqtt_url_eeprom = "MQTT.petersfictionalserver.com";

    
  public:
    eepromWrapper();
    void writeWord(String word);
    String readWord();
};


#endif