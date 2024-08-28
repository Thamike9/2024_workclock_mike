#ifndef AHT20_H
#define AHT20_H
#include <Arduino.h>
#include <Adafruit_AHTX0.h>




class aht20 {
  private:
    Adafruit_AHTX0 aht;
    float temp;
    float humi;
    
  public:
    aht20();
    void takeMeasurement();
    float getLastTemp();
    float getLastHumi();
};


#endif