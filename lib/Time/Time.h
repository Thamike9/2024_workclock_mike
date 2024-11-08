#ifndef MY_TIME_H
#define MY_TIME_H
#include <Arduino.h>
#include <Wire.h>
#include <DS3231.h>


class Time {
  private:
    unsigned long saved_millis_;
    uint16_t year_;
    int8_t month_;
    int8_t day_;
    int8_t hours_;
    int8_t minutes_;
    bool  date_set_;
    DS3231 myRTC;
    
  public:
    Time(uint16_t year, int8_t month, int8_t day, int8_t hours, int8_t minutes);
    Time(int8_t hours, int8_t minutes);
    void setTime(uint16_t year, int8_t month, int8_t day, int8_t hours, int8_t minutes);
    void setTime(int8_t hours, int8_t minutes);
    int8_t getHours();
    int8_t getMinutes();
    int8_t getDay();
    int8_t getMonth();
    uint16_t getYear();
};


#endif