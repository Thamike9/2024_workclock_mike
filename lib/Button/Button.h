#ifndef MY_BUTTON_H
#define MY_BUTTON_H
#include <Arduino.h>


class Button {
  private:
    int8_t pin;
    
  public:
    Button(int8_t);
    uint8_t get();
};


#endif