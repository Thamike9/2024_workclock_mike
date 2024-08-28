#include "eepromWrapper.h"

eepromWrapper::eepromWrapper()  {
 EEPROM.begin(512);
}

void eepromWrapper::writeWord(String word) {
  delay(10);

  for (int i = 0; i < word.length(); ++i) {
    //Serial.println(word[i]);
    EEPROM.write(i, word[i]);
  }

  EEPROM.write(word.length(), '\0');
  EEPROM.commit();
}

String eepromWrapper::readWord() {
  String word;
  char readChar = 'A';
  int i = 0;

  while (readChar != '\0') {
    readChar = char(EEPROM.read(i));
    delay(10);
    i++;

    if (readChar != '\0') {
      word += readChar;
    }
  }
  mqtt_url_eeprom = word;
  return word;
}