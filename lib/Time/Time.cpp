#include "Time.h"



Time::Time(int8_t hours, int8_t minutes)  {
  this->hours_ = hours;
  this->minutes_ = minutes;
  Wire.begin();
}

void Time::setTime(int8_t hours, int8_t minutes){
    if(hours>23){
        hours = hours - 24;
    } 
    if(minutes>59){
        minutes = minutes - 60;
        hours = hours + 1;
    } 
    Serial.printf("Set RTC to %d:%d\n", hours, minutes);
    myRTC.setClockMode(false);  // set to 24h
    myRTC.setHour(hours);     // Set the time to 12:00:00 (24hr format)
    myRTC.setMinute(minutes);
    this->saved_millis_ = millis();
    this->hours_ = hours;
    this->minutes_ = minutes;
}

int8_t Time::getMinutes(){
    int8_t minutes;
    if(millis() < this->saved_millis_+1000){
        minutes = this->minutes_;
    }
    else{
        minutes = myRTC.getMinute();
        //Serial.printf("minutes: %d", minutes);
        this->saved_millis_ = millis();
        this->minutes_ = minutes;
    }
    
    return minutes;
}

int8_t Time::getHours(){
    //unsigned long delta_millis = millis() - this->saved_millis_;
    //int8_t hours = (this->hours_ + delta_millis/(1000*60*60))%24;
    int8_t hours;
    bool h12Flag;
    bool pmFlag;
    if(millis() < this->saved_millis_+1000){
        //Serial.println("Return cached time");
        hours = this->hours_;
    }
    else{
        hours = myRTC.getHour(h12Flag, pmFlag); //returns if 12/24h mode and AM/PM
        //Serial.println("Return fresh");
        //this->saved_millis_ = millis();
        this->hours_ = hours;
    }
    
    return hours;
}
