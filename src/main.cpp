#include <Arduino.h>

#include <patterns.h>
#include <ir.h>
#include <ble_distance.h>
// #include <mesh_wifi.h>
// #include <input.h>
// #include <bluetooth.h>

#ifdef __AVR__
  #include <avr/power.h>
#endif

#define DELAYVAL 500

bool random_pattern = true;       // default is false

void setup() {
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
  #endif

  Serial.begin(9600);

  setup_leds();
  setup_ir();
  
  setup_ble();

}

void loop() {

  // when we are on random pattern, keep advancing
  if (random_pattern) {
    EVERY_N_SECONDS(5) {nextPattern();}
  }

  switch(read_ir()) {
      case Button_Red: 
        Serial.println(F("RED - Next Show"));
        nextPattern();
        random_pattern = false;   // switch into manual mode until explicitly asked again
      break;
      case Button_Green: 
        Serial.println(F("Green - Previous Show")); 
        previousPattern();
        random_pattern = false;   // switch into manual mode until explicitly asked again
      break;
      case Button_BrightDown: 
        Serial.println(F("Decrease Brightness")); 
        decreaseBrightness();
      break;
      case Button_BrightUp: 
        Serial.println(F("Increase Brightness")); 
        increaseBrightness();
      break;
      case Button_On:
        Serial.println(F("Randomize Pattern")); 
        random_pattern = true;
      break;
      case Button_LightRed:
        random_pattern = false;
        red();
      break;
      case Button_LightGreen:
        random_pattern = false;
        green();
      break;
      case Button_LightBlue:
        random_pattern = false;
        blue();
      break;
      case Button_Flash:
        Serial.println(F("Blending Overlay")); 
        set_blending_verlay();
      break; 

  }

  ble_loop();

  updatePattern();

  // for input button
  //void touchAttachInterrupt(uint8_t pin, void (*userFunc)(void), uint16_t threshold);

  /* Update Display 
  EVERY_N_MILLISECONDS(200) {
    char buf[5];
    snprintf (buf, 5, "%2d", gCurrentPatternNumber);
    u8x8.drawString(8, 2, buf);
    snprintf (buf, 5, "%4d", gHue);
    u8x8.drawString(5, 3, buf);
    //snprintf (buf, 5, "%4d", read_touch());
    //u8x8.drawString(5, 4, buf);
  }
  */

  /*

  for(int i=0; i<NUMPIXELS; i++) {

    leds[i] = CRGB::Blue;
    //pixels.setPixelColor(i-2, pixels.Color(150, 0, 0));
    //pixels.setPixelColor(i-1, pixels.Color(0, 0, 0));
  }
  FastLED.show();
  delay(DELAYVAL);


  for(int i=0; i<NUMPIXELS; i++) {

    leds[i] = CRGB::LightBlue;
    //pixels.setPixelColor(i-2, pixels.Color(150, 0, 0));
    //pixels.setPixelColor(i-1, pixels.Color(0, 0, 0));
  }
  FastLED.show();
  delay(DELAYVAL);
  */
}