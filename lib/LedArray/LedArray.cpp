#include "LedArray.h"

LedArray::LedArray(uint8_t brightness)  {
  this->brightness = brightness;
  this->old_minutes = 0;
  this->old_hours = 0;
  init();
}
void LedArray::init() {
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness( brightness );
  this->LEDcolor[0] = 255;//R
  this->LEDcolor[1] = 255;//G
  this->LEDcolor[2] = 255;//B
  this->LEDcolor_last = 0;

  for(int i=0; i<NUM_LEDS; i++){
    leds[i].setRGB( LEDcolor[0] , LEDcolor[1] , LEDcolor[2] );
  }
  FastLED.show();
}

void LedArray::off() {
  for(int i=0; i<NUM_LEDS; i++){
    leds[i] = CRGB::Black;
  }
  FastLED.show();
}

void LedArray::testMatrix(){
  off();
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CRGB::White;
    delay(33);
    FastLED.show();
  }
  for(int i=NUM_LEDS; i>0; i--){
    leds[i] = CRGB::Black;
    delay(33);
    FastLED.show();
  }
  
}

// e.g. 10:46, 12:01, 16:44, 23:05 or 2:13 
void LedArray::setTime(int8_t hours, int8_t minutes, uint8_t fade_delay){

  if(minutes >= 23){ //20 nach --> 5 vor halb 
    hours++;
  }

  if(hours > 12){
    hours = hours - 12;
  }
  //Problem wenn es 0:05 ist, dann ist es theoretisch 5 nach 0!!!!
  if(hours == 0){
    hours = 12;
  }


  // do a fading effect while updating
  // display the old time again, fade out 
  for(int i=0; i<NUM_LEDS; i++){
    leds[i] = CRGB::Black;
  }
  setPrimer(this->old_minutes); // "VOR" or "NACH" and "UHR"
  setHours(this->old_hours, this->old_minutes);
  setMinutes(this->old_minutes);
  for(uint8_t i = brightness; i > 0; i--){
    unsigned int time_before = millis();
    FastLED.setBrightness( i );
    FastLED.show();
    //check how long the update took and delay for the rest
    int rest_delay = fade_delay - (millis() - time_before);
    //Serial.printf("rest delay: %d", rest_delay);
    if(rest_delay > 0){
      delay(rest_delay);
    }
    
  }

  //display the new time and fade in
  for(int i=0; i<NUM_LEDS; i++){
    leds[i] = CRGB::Black;
  }
  setPrimer(minutes); // "VOR" or "NACH" and "UHR"
  setHours(hours, minutes);
  setMinutes(minutes);
  for(uint8_t i = 0; i <= brightness; i++){
    FastLED.setBrightness( i );
    FastLED.show();
    delay(fade_delay);
  }

  this->old_hours = hours;
  this->old_minutes = minutes;
}

//"ES IST", "VOR" or "NACH" and "UHR"
/*
* nothing (punkt)
* fünf nach
* zehn nach
* viertel nach
* zwanzig nach
* fünf vor halb 
* halb
* fünf nach halb
* zwanzig vor
* dreiviertel
* zehn vor
* fünf vor
*/
void LedArray::setPrimer(int8_t minutes){
  // ES IST
  for(int i=0; i<5; i++){
    leds[ItIsAddr[i]].setRGB( LEDcolor[0] , LEDcolor[1] , LEDcolor[2] );
  }

  //NACH
  if(minutes >= 3 && minutes <= 22){ 
    for(int i=0; i<4; i++){
      if(NachAddr[i] != -1){
        leds[NachAddr[i]].setRGB( LEDcolor[0] , LEDcolor[1] , LEDcolor[2] );
      }
    }
  }
  //VOR
  else if(minutes >= 23 && minutes < 28) {
    for(int i=0; i<4; i++){
      if(VorAddr[i] != -1){
        leds[VorAddr[i]].setRGB( LEDcolor[0] , LEDcolor[1] , LEDcolor[2] );
      }
    }
  }
  //Nothing (halb)
  else if(minutes >= 28 && minutes < 33) {
    //nothing
  }
  //NACH
  else if(minutes >= 33 && minutes < 38) {
    for(int i=0; i<4; i++){
      if(NachAddr[i] != -1){
        leds[NachAddr[i]].setRGB( LEDcolor[0] , LEDcolor[1] , LEDcolor[2] );
      }
    }
  }
  //VOR
  else if(minutes >= 38 && minutes < 43) {
    for(int i=0; i<4; i++){
      if(VorAddr[i] != -1){
        leds[VorAddr[i]].setRGB( LEDcolor[0] , LEDcolor[1] , LEDcolor[2] );
      }
    }
  }
  //Nothing (dreiviertel)
  else if(minutes >= 43 && minutes < 48) {
    //nothing
  }
  //VOR
  else if (minutes >= 48 && minutes < 58){
    for(int i=0; i<4; i++){
      if(VorAddr[i] != -1){
        leds[VorAddr[i]].setRGB( LEDcolor[0] , LEDcolor[1] , LEDcolor[2] );
      }
    }
  }
  //Nothing (punkt)
  if (minutes >= 58 || minutes < 3){
    for(int i=0; i<3; i++){
      leds[UhrAddr[i]].setRGB( LEDcolor[0] , LEDcolor[1] , LEDcolor[2] );
    }
  }

  //UHR
  
  
}


void LedArray::setHours(int8_t hours, int8_t minutes){
  //Serial.printf("hours intern: %d, minutes intern: %d\n", hours, minutes);
  for(int i=0; i<6; i++){
    if((hours == 1  && minutes < 3) || (hours == 1 && minutes >= 58)){
       //Serial.printf("Sonderfall es ist 1 Uhr tritt auf: hours:  %d\n", hours);
      hours = 13;
    }
    
    if (HourAddr[hours - 1][i] != -1)
    {
      leds[HourAddr[hours-1][i]].setRGB( LEDcolor[0] , LEDcolor[1] , LEDcolor[2] );
    }
  }
}

void LedArray::setMinutes(int8_t minutes){
  for(int i=0; i<11; i++){
    if(MinAddr[minutes][i] != -1){
      leds[MinAddr[minutes][i]].setRGB( LEDcolor[0] , LEDcolor[1] , LEDcolor[2] );
    }
  }
}


void LedArray::setColor(uint8_t red, uint8_t green, uint8_t blue){
  this->LEDcolor[0] = red;//R
  this->LEDcolor[1] = green;//G
  this->LEDcolor[2] = blue;//B
  FastLED.show();
}

void LedArray::setNextColor(){
  
  if(LEDcolor_last < 7 ){ //number of combinations in ColorCombinations -1
    LEDcolor_last++;
  }
  else{
    LEDcolor_last = 0;
  }
  
  
  this->LEDcolor[0] = ColorCombinations[LEDcolor_last][0];//R
  this->LEDcolor[1] = ColorCombinations[LEDcolor_last][1];//G
  this->LEDcolor[2] = ColorCombinations[LEDcolor_last][2];//B
  Serial.printf("R: %d, G:%d, B:%d\n", this->LEDcolor[0], this->LEDcolor[1], this->LEDcolor[2]);
  setColor(this->LEDcolor[0], this->LEDcolor[1], this->LEDcolor[2]);
}



void LedArray::setBrightness(uint8_t set_brightness){
  if(set_brightness > 200){
    set_brightness = 200;
  }
  else if(set_brightness < 10){
    set_brightness = 10;
  }
  //Serial.printf("old->new %d->%d\n", this->brightness, set_brightness);
  if(this->brightness > set_brightness){
    for(int i = this->brightness; i>=set_brightness; i--){
      //Serial.printf(": %d\n", i);
      FastLED.setBrightness( i );
      FastLED.show();
      delay(5);
    }
  }
  else{
    for(int i = this->brightness; i<=set_brightness; i++){
      //Serial.printf(", %d\n", i);
      FastLED.setBrightness( i );
      FastLED.show();
      delay(5);
    }
  }
  
  this->brightness = set_brightness;
}

