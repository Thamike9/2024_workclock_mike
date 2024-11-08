#include "Time.h"

int isSummerTimeInEurope(int year, int month, int day, int hour);

Time::Time(uint16_t year, int8_t month, int8_t day, int8_t hours, int8_t minutes)  {
  this->year_ = year;
  this->month_ = month;
  this->day_ = day;
  this->hours_ = hours;
  this->minutes_ = minutes;
  this->date_set_ = true;
  Wire.begin();
}

Time::Time(int8_t hours, int8_t minutes)  {
  Time((uint32_t) 0, (int8_t) 0, (int8_t) 0, hours, minutes);
  this->date_set_ = false;
}

void Time::setTime(uint16_t year, int8_t month, int8_t day, int8_t hours, int8_t minutes){
    while(hours>23){
        hours = hours - 24;
        day = day + 1;
    } 
    while(minutes>59){
        minutes = minutes - 60;
        hours = hours + 1;
    } 
    Serial.printf("Set RTC to %d:%d\n", hours, minutes);
    myRTC.setClockMode(false);  // set to 24h
    myRTC.setYear(year);
    myRTC.setMonth(month);
    myRTC.setDate(day);
    myRTC.setHour(hours);     // Set the time to 12:00:00 (24hr format)
    myRTC.setMinute(minutes);
    this->saved_millis_ = millis();
    this->year_ = year;
    this->month_ = month;
    this->day_ = day;
    this->hours_ = hours;
    this->minutes_ = minutes;
    this->date_set_ = true;
}


void Time::setTime(int8_t hours, int8_t minutes){
    while(hours>23){
        hours = hours - 24;
    } 
    while(minutes>59){
        minutes = minutes - 60;
        hours = hours + 1;
    } 
    Serial.printf("Set RTC to %d:%d\n", hours, minutes);
    myRTC.setClockMode(false);  // set to 24h
    myRTC.setHour(hours);     // Set the time to 12:00:00 (24hr format)
    myRTC.setMinute(minutes);
    this->saved_millis_ = millis();
    this->year_ = 0;
    this->month_ = 0;
    this->day_ = 0;
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
        this->saved_millis_ = millis();
        this->hours_ = hours;
    }
    if(this->date_set_){
        if(isSummerTimeInEurope(this->year_, this->month_, this->day_, hours)){
            hours = hours + 1;
        }
    }
    return hours;
}

int8_t Time::getDay(){
    int8_t day;
    if(millis() < this->saved_millis_+1000){
        //Serial.println("Return cached time");
        day = this->day_;
    }
    else{
        day = myRTC.getDate(); //returns if 12/24h mode and AM/PM
        //Serial.println("Return fresh");
        this->saved_millis_ = millis();
        this->day_ = day;
    }
    return day;
}

int8_t Time::getMonth(){
    int8_t month;
    bool centuryBit;
    if(millis() < this->saved_millis_+1000){
        //Serial.println("Return cached time");
        month = this->month_;
    }
    else{
        month = myRTC.getMonth(centuryBit); //returns if 12/24h mode and AM/PM
        //Serial.println("Return fresh");
        this->saved_millis_ = millis();
        this->month_ = month;
    }
    return month;
}

uint16_t Time::getYear(){
    uint16_t year;
    if(millis() < this->saved_millis_+1000){
        year = this->year_;
    }
    else{
        year = myRTC.getYear(); 
        this->year_ = year;
    }
    return year;
}

// Calculates the day of the week for a given 
int dow(int y, int m, int d)
{
	static int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
	y -= m < 3;
	return (y + y/4 - y/100 + y/400 + t[m-1] + d ) % 7;
}

int isSummerTimeInEurope(int year, int month, int day, int hour)
{
  if(month <3 || month >= 11)
	{
		return false;
	}
	if(month >3 && month <11)
	{
		return true;
	}
	if(month == 3)
	{
		int lastSundayOfMonth = 31-dow(year,month,31);
		if(day >= lastSundayOfMonth && hour >= 2)
		  return true;
		else
      return false;
	}
	if(month == 10)
	{
    int lastSundayOfMonth = 31-dow(year,month,31);
		if(day >= lastSundayOfMonth && hour >= 2)
		  return false;
		else
      return true;
	}
  //Cry as this should never happen
  return false;
}
