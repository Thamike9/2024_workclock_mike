#ifndef MY_LED_H
#define MY_LED_H
#include <Arduino.h>
#define FASTLED_INTERRUPT_RETRY_COUNT 0
#define FASTLED_ALLOW_INTERRUPTS 0
#include <FastLED.h>
#include "LED_mapping.h"

#define LED_PIN     13
#define NUM_LEDS    110
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB



class LedArray {
  private:
    uint8_t brightness;
    uint8_t old_minutes;
    uint8_t old_hours;
    CRGB leds[NUM_LEDS];
    uint8_t LEDcolor[3];
    uint8_t LEDcolor_last;

    
  public:
    LedArray(uint8_t brightness);
    void init();
    void off();
    void testMatrix();
    void setTime(int8_t hours, int8_t minutes, uint8_t fade_delay);
    void setHours(int8_t hours, int8_t minutes);
    void setPrimer(int8_t minutes);
    void setMinutes(int8_t minutes);
    void setColor(uint8_t red, uint8_t green, uint8_t blue);
    void setNextColor();
    void setBrightness(uint8_t brightness);
};
#endif