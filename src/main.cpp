/*
 * 2023, wordclock
 * made for and with Michael Wolf: https://michaelwolf.at/wordclock
 *
 * by peter@traunmueller.net
 *
 */

#include <Arduino.h>
#include "LedArray.h"
#include "Time.h"
#include "Button.h"
#include "aht20.h"
#include <ESP8266WiFi.h>
#include "mqttWrapper.h"
#include "CustomWiFiManager.h"
#include "WiFiManagerWrapper.h"
#include "ESP8266mDNS.h"
#include "eepromWrapper.h"
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <EEPROM.h>
#include <TimeLib.h>

LedArray ledArr(25); // Initialize the LED brighness to 25
Time RTime(12, 0);   // Initialize the time to 12:00
aht20 AHT;
Button hourButton(10);
Button minuteButton(14);
Button modeButton(16);
WiFiManagerWrapper wifiWrap;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

uint8_t r, g, b;
uint8_t buttons_last[3] = {1, 1, 1};
uint8_t hour_last, minute_last;

unsigned int next_ntp_update;
unsigned int next_brightness_update;
unsigned int next_aht20_update;

WiFiClient wifiClient;
uint8_t MQTTenabled = false;
mqttWrapper mqttwrap;
String MQTT_url_eeprom = "192.168.111.99"; // broker.emqx.io
eepromWrapper eeprom;

void setup()
{
  // ledArr.setBrightness(30); // set LED brighness to 30
  // RTime.setTime(16, 45); // set time to 16:45
  Serial.begin(115200);

  wifiWrap.init();
  if (!modeButton.get())
  {
    wifiWrap.openPortal();
  }

  r = 255;
  g = 233;
  b = 0;

  ledArr.testMatrix();
  ledArr.setColor(r, g, b);

  MQTT_url_eeprom = eeprom.readWord();
  if (MQTT_url_eeprom.length() > 0 && MQTT_url_eeprom.length() < 500)
  {
    Serial.print("MQTT enabled: ");
    Serial.println(MQTT_url_eeprom);
    MQTTenabled = 1;
  }
  else
  {
    Serial.println("MQTT disabled.");
  }
}

void loop()
{

  if (hour_last != RTime.getHours() || minute_last != RTime.getMinutes())
  {
    uint8_t fade_delay = 50;
    ledArr.setTime(RTime.getHours(), RTime.getMinutes(), fade_delay);
    Serial.printf("Time: %d:%d\n", RTime.getHours(), RTime.getMinutes());
    hour_last = RTime.getHours();
    minute_last = RTime.getMinutes();
  }

  if (millis() > next_brightness_update)
  {
    // set brightness
    uint8_t set_brightness = 255; //(analogRead(A0)/4)-1; // 8-255, 8==darkness //ACHTUNG! Deaktiviert weil LDR scheisse ist.
    // Serial.printf("Brightness set to: %d\n", set_brightness);
    // AnalogRead braucht auch ewig viel Zeit
    ledArr.setBrightness(set_brightness);
    next_brightness_update = millis() + 1000;
  }

  if (WiFi.status() == WL_CONNECTED && millis() > next_aht20_update && MQTTenabled)
  {
    AHT.takeMeasurement();
    float temp = AHT.getLastTemp();
    float humi = AHT.getLastHumi();
    Serial.printf("MQTT; Temp: %.2f°C, Humi: %.2f%% \n", temp, humi);
    next_aht20_update = millis() + 5 * 60 * 1000; // min * sec * millisec = 5 min

    char buff[25];
    snprintf(buff, sizeof(buff), "%d", ESP.getChipId());
    mqttwrap.init(MQTT_url_eeprom.c_str(), buff);
    mqttwrap.sendMQTT(temp, humi);
  }

  if (WiFi.status() == WL_CONNECTED && millis() > next_ntp_update)
  {
    timeClient.begin();
    // GMT +1 = 3600
    timeClient.setTimeOffset(3600);
    timeClient.update();
    
    uint16_t currentYear = year(timeClient.getEpochTime());
    uint8_t  currentMonth = month(timeClient.getEpochTime());
    uint8_t  currentDay = day(timeClient.getEpochTime());
    Serial.print("Grabbing NTP update. ");
    int8_t currentHour = hour(timeClient.getEpochTime());
    Serial.print("Hour: ");
    Serial.println(currentHour);

    int currentMinute = minute(timeClient.getEpochTime());
    Serial.print("Minutes: ");
    Serial.println(currentMinute);
  
    RTime.setTime(currentYear, currentMonth, currentDay, currentHour, currentMinute);

    next_ntp_update = millis() + 24 * 60 * 60 * 1000; // grab a new update every 24 hours
  }

  uint8_t buttons[3] = {hourButton.get(), minuteButton.get(), modeButton.get()};

  if (!buttons[0] || !buttons[1] || !buttons[2])
  {
    // Serial.println("Button pressed: ");
    // Serial.print(buttons[0]);
    // Serial.print(buttons[1]);
    // Serial.print(buttons[2]);

    if (!hourButton.get())
    {
      // Serial.println("hour++");
      RTime.setTime(RTime.getHours() + 1, RTime.getMinutes());
      delay(100); // debunce
    }
    if (!minuteButton.get())
    {
      // Serial.println("minute++");
      RTime.setTime(RTime.getHours(), RTime.getMinutes() + 1);
      delay(100); // debunce
    }
    if (!modeButton.get())
    {
      ledArr.setNextColor();
      delay(100); // debunce
    }
    uint8_t fade_delay = 0;
    ledArr.setTime(RTime.getHours(), RTime.getMinutes(), fade_delay);
    Serial.printf("Button pressed: %d:%d\n", RTime.getHours(), RTime.getMinutes());
    hour_last = RTime.getHours();
    minute_last = RTime.getMinutes();
  }

  // was something manually updated via the web interface?
  if (wifiWrap.isManualTimeSet() || wifiWrap.isMQTTSet())
  {
    // update time if it was set manually
    if (wifiWrap.isManualTimeSet())
    {
      Serial.print("Time set manually via Wifi: ");
      Serial.print(wifiWrap.getHours());
      Serial.print(":");
      Serial.println(wifiWrap.getMinutes());

      RTime.setTime(wifiWrap.getHours(), wifiWrap.getMinutes());
      wifiWrap.closePortal();
    }

    // update MQTT if it was set manually
    if (wifiWrap.isMQTTSet())
    {
      Serial.print("MQTT set manually via Wifi: ");
      char *MQTT_url = new char[30 + 1];
      wifiWrap.getMQTT(MQTT_url);
      Serial.println(MQTT_url);
      eeprom.writeWord(MQTT_url);
      MQTT_url_eeprom = eeprom.readWord();
      Serial.println(MQTT_url_eeprom);
      MQTTenabled = true;

      wifiWrap.closePortal();
    }
  }

  // uint16_t LDR_val = analogRead(A0); // 100-1023, 100==totaldarkness
  // Serial.printf("LDR_val: %d\n", LDR_val);
  // uint8_t brightness = (LDR_val/4)-1; //max 255
  // Serial.printf("Brightness set to: %d\n", brightness);
  // ledArr.setBrightness(brightness);

  MDNS.update();
  wifiWrap.doWiFiManager();
  delay(100);
}
