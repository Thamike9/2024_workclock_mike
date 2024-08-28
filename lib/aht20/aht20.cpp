#include "aht20.h"



aht20::aht20()  {
  if (! aht.begin()) {
    Serial.println("Could not find AHT? Check wiring");
    while (1) delay(10);
  }
}

void aht20::takeMeasurement(){
  sensors_event_t humidity, temperature;
  
  aht.getEvent(&humidity, &temperature); // populate temp and humidity objects with fresh data
  temp = temperature.temperature;
  humi = humidity.relative_humidity;

}

float aht20::getLastTemp(){
  return temp;
}
float aht20::getLastHumi(){
  return humi;
}