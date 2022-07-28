#include <Arduino.h>

#include <patterns.h>
#include <ir.h>
#include <ble_distance.h>
// #include <mesh_wifi.h>
// #include <input.h>

#ifdef __AVR__
  #include <avr/power.h>
#endif

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
    EVERY_N_SECONDS(10) {nextPattern();}
  }

  switch(read_ir()) {
      // Serial.println(F("Overwrite Nervous"))
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
      case Button_LightBlue:
        random_pattern = false;
        blue();
      break;
      case Button_Flash:
        Serial.println(F("Blending Overlay")); 
        set_blending_overlay();
      break; 
      case Button_Off:
        Serial.println(F("Turning Off")); 
        // TODO
      break;
      case Button_Orange:
        decrease_ghue_speed();
        Serial.println(F("Decrease Speed")); 
      break;
      case Button_LightGreen:
        increase_ghue_speed();
        Serial.println(F("Increase Speed")); 
      break;
  }

  timer.tick();

  // we only scan if we are not the server, but the ble_lib will check for that
  EVERY_N_SECONDS(5) {
    ble_scan();
  }

  if (ble_loop()) {
    // go nervous
    nervous(get_sender());      // whether sender or recipient, aka raffy or whit
  } else {
    updatePattern();
  }

  /*
  EVERY_N_MILLISECONDS(500) {
    Serial.println("500 msec");
  }
  */

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

}