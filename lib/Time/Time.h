#ifndef MY_TIME_H
#define MY_TIME_H
#include <Arduino.h>
#include <Wire.h>
#include <DS3231.h>


class Time {
  private:
    unsigned long saved_millis_;
    int8_t hours_;
    int8_t minutes_;
    DS3231 myRTC;
    
  public:
    Time(int8_t hours, int8_t minutes);
    void setTime(int8_t hours, int8_t minutes);
    int8_t getHours();
    int8_t getMinutes();
};


#endif