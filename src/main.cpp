#include <Arduino.h>

#include <heart.h>
#include <patterns.h>
#include <ir.h>
#include <ble_distance.h>
// #include <mesh_wifi.h>
// #include <input.h>

#ifdef __AVR__
  #include <avr/power.h>
#endif


#define PATTERN_RUNTIME_MAX (100)      // seconds

bool random_pattern = true;       // default is false
uint8_t pattern_runtime = 10;        // seconds

void setup() {
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
  #endif

  Serial.begin(9600);

  setup_heart();

  setup_leds();
  setup_ir();
  
  setup_ble();

}

void decrease_pattern_runtime(bool show_indicator = true) {
  if (pattern_runtime < PATTERN_RUNTIME_MAX - 10)
    pattern_runtime += 10;
  if (show_indicator) 
    indicator(pattern_runtime, PATTERN_RUNTIME_MAX, CRGB::Green);
}

void increase_pattern_runtime(bool show_indicator = true) {
  if (pattern_runtime > 10)
    pattern_runtime -= 10;
  if (show_indicator) 
    indicator(pattern_runtime, PATTERN_RUNTIME_MAX, CRGB::Green);
}

void loop() {

  // when we are on random pattern, keep advancing
  if (random_pattern) {
    EVERY_N_SECONDS_I(patternTimer,10) { 
      uint8_t timeval = pattern_runtime;
      patternTimer.setPeriod(timeval);                             
      nextPattern();
    }
  }

  uint32_t ir = read_ir();
  if (ir == Next_Show) {
        Serial.println(F("RED - Next Show"));
        nextPattern();
        random_pattern = false;   // switch into manual mode until explicitly asked again
  } else if (ir == Prev_Show) {
        Serial.println(F("Green - Previous Show")); 
        previousPattern();
        random_pattern = false;   // switch into manual mode until explicitly asked again
  } else if (ir == Bright_Down) {
        Serial.println(F("Decrease Brightness")); 
        decreaseBrightness();
  } else if (ir == Bright_Up) {
        Serial.println(F("Increase Brightness")); 
        increaseBrightness();
  } else if (ir == Randomize_Pattern) {
        Serial.println(F("Randomize Pattern")); 
        random_pattern = true;
  } else if (ir == Color_White) {
        // this only sets the color for some of the shows, not changing show
        Serial.println(F("Color White")); 
        white();
  } else if (ir == Color_Red) {
        // this only sets the color for some of the shows, not changing show
        Serial.println(F("Color Red")); 
        red();
  } else if (ir == Color_Blue) {
        // this only sets the color for some of the shows, not changing show
        Serial.println(F("Color Blue")); 
  } else if (ir == Blending_Overlay) {
        Serial.println(F("Blending Overlay")); 
        set_blending_overlay();
  } else if (ir == Off) {
        Serial.println(F("Turning Off")); 
        // TODO
  } else if (ir == Speed_Dec) {
        decrease_ghue_speed();
        Serial.println(F("Decrease Speed")); 
  } else if (ir == Speed_Inc) {
        increase_ghue_speed();
        Serial.println(F("Increase Speed")); 
  } else if (ir == Runtime_Dec) {
        decrease_pattern_runtime();
        Serial.print(F("Increase Pattern Runtime: ")); 
        Serial.println(pattern_runtime);
  } else if (ir == Runtime_Inc) {
        increase_pattern_runtime();
        Serial.print(F("Increase Pattern Runtime: ")); 
        Serial.println(pattern_runtime);
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