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

#define PATTERN_RUNTIME_MAX (100)          // seconds -- used for manual change
#define DEfAULT_PATTERN_RUNTIME (100)      // seconds

bool random_pattern = true;                               // default is false
uint8_t pattern_runtime = DEfAULT_PATTERN_RUNTIME;        // seconds 

void setup() {
  
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
  #endif

  Serial.begin(9600);

  setup_heart();

  setup_leds();
  setup_ir();
  setup_ble();
  setup_sound();

}

void increase_pattern_runtime(bool show_indicator = true) {
  if (pattern_runtime < PATTERN_RUNTIME_MAX - 10)
    pattern_runtime += 10;
  if (show_indicator) 
    indicator(pattern_runtime, PATTERN_RUNTIME_MAX, CRGB::Green);
}

void decrease_pattern_runtime(bool show_indicator = true) {
  if (pattern_runtime > 10)
    pattern_runtime -= 10;
  if (show_indicator) 
    indicator(pattern_runtime, PATTERN_RUNTIME_MAX, CRGB::Green);
}

void loop() {

  // when we are on random pattern, keep advancing
  if (random_pattern) {
    EVERY_N_SECONDS_I(patternTimer, DEfAULT_PATTERN_RUNTIME) {   // change show every n seconds 
      patternTimer.setPeriod(pattern_runtime);                             
      nextPattern();
    }
  }

  uint32_t ir = read_ir();
  if (ir == Next_Show) {
        Serial.println(F("Next Show"));
        // reset_bt_timeout();       // if we were nervous, we would timeout in 2 minutes, but if IR was used, reset that
        nextPattern();
        random_pattern = false;   // switch into manual mode until explicitly asked again
        set_overwrite_nervous();
  } else if (ir == Prev_Show) {
        Serial.println(F("Previous Show")); 
        // reset_bt_timeout();       // if we were nervous, we would timeout in 2 minutes, but if IR was used, reset that
        previousPattern();
        random_pattern = false;   // switch into manual mode until explicitly asked again
        set_overwrite_nervous();
  } else if (ir == Bright_Down) {
        Serial.println(F("Decrease Brightness")); 
        decreaseBrightness();
  } else if (ir == Bright_Up) {
        Serial.println(F("Increase Brightness")); 
        increaseBrightness();
  } else if (ir == Randomize_Pattern) {
        Serial.println(F("Randomize Pattern")); 
        random_pattern = true;
        set_overwrite_nervous();
  } else if (ir == Color_White) {
        // this only sets the color for some of the shows, not changing show
        Serial.println(F("Color White")); 
        white();
        solid();
        random_pattern = false;   // switch into manual mode until explicitly asked again
        set_overwrite_nervous();
  } else if (ir == Color_Red) {
        // this only sets the color for some of the shows, not changing show
        Serial.println(F("Color Red")); 
        red();
        set_overwrite_nervous();
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

  timer.tick();   // pattern_timer

  // we only scan if we are not the server, but the ble_lib will check for that
  EVERY_N_SECONDS(5) {
    ble_scan();
  }

  if (ble_loop()) {               // true if connected and not within certain time of having been nervous already
    nervous(get_sender());        // whether sender or recipient, aka raffy or whit
    random_pattern = false;
  } else {
    updatePattern();
  }

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