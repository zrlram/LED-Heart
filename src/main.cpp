#include <Arduino.h>

#include <heart.h>
#include <patterns.h>
#include <ir.h>            // will only exeucte if USE_IR is set in heart.h
#include <button.h>         // will only exeucte if USE_BUTTON is set in heart.h

#include <ble_distance.h>
// #include <wifi_ota.h>            // TBD - needs to be back on later for OTA, but not for now
// #include <mesh_wifi.h>
// #include <input.h>

/* TODO - Next steps
    - Button to enable / disable WiFi
    - Shows
      - Pong
      - Magic eightball
      - Fireworks
    --> Use phone as a display for variables / functions / etc
    --> Fix indicator for pattern selector  (show them all initially and highlight in red / purple where we are)
*/

#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PATTERN_RUNTIME_MAX (100)          // seconds -- used for manual change
#define DEfAULT_PATTERN_RUNTIME (100)      // seconds

bool random_pattern = true;                               // default is false
uint8_t pattern_runtime = DEfAULT_PATTERN_RUNTIME;        // seconds 

void setup() {
  
  Serial.begin(9600);

  setup_heart();
  setup_leds();
  #ifdef __USE_IR
      setup_ir();
  #endif
  #ifdef __USE_BUTTON
      setup_button();
  #endif
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

  uint32_t action = 0;
  #ifdef __USE_IR
     action = read_ir();
  #endif
  #ifdef __USE_BUTTON
    button_loop();
    if (button_pressed(0)) {
      Serial.println("button 1 pressed - decrease value");
      if (menu_index == 0)           // 0 - select show (Next_Show, Prev_Show)
           action = Prev_Show; 
      else if (menu_index == 1)      //  1 - brightness
           action = Bright_Down; 
      else if (menu_index == 2)      //  2 - speed
           action = Speed_Dec;
      else if (menu_index == 3)      //  3 - runtime
           action = Runtime_Dec;
      else if (menu_index == 4)      //  4 - random show toggle
           action = Next_Show;      // drops out of random
    }
    if (button_pressed(1)) {
      Serial.println("button 2 pressed - increase value");
      if (menu_index == 0)           // 0 - select show (Next_Show, Prev_Show)
           action = Next_Show; 
      else if (menu_index == 1)      //  1 - brightness
           action = Bright_Up; 
      else if (menu_index == 2)      //  2 - speed
           action = Speed_Inc;
      else if (menu_index == 3)      //  3 - runtime
           action = Runtime_Inc;
      else if (menu_index == 4)      //  4 - random show toggle
           action = Randomize_Pattern;
    }
    if (button_pressed(2)) {
      Serial.print("button 3 pressed - decrease menu item ");
      menu_index = ( menu_index - 1 + MENU_OPTIONS ) % MENU_OPTIONS ;
      Serial.println(menu_index);
    }
    if (button_pressed(3)) {
      Serial.print("button 4 pressed - increase menu item ");
      menu_index = ( menu_index + 1 ) % MENU_OPTIONS;
      Serial.println(menu_index);
    }
  #endif

  if (action == Next_Show) {
        Serial.println(F("Next Show"));
        // reset_bt_timeout();       // if we were nervous, we would timeout in 2 minutes, but if IR was used, reset that
        nextPattern();
        random_pattern = false;   // switch into manual mode until explicitly asked again
        set_overwrite_nervous();
  } else if (action == Prev_Show) {
        Serial.println(F("Previous Show")); 
        // reset_bt_timeout();       // if we were nervous, we would timeout in 2 minutes, but if IR was used, reset that
        previousPattern();
        random_pattern = false;   // switch into manual mode until explicitly asked again
        set_overwrite_nervous();
  } else if (action == Bright_Down) {
        Serial.println(F("Decrease Brightness")); 
        decreaseBrightness();
  } else if (action == Bright_Up) {
        Serial.println(F("Increase Brightness")); 
        increaseBrightness();
  } else if (action == Randomize_Pattern) {
        Serial.println(F("Randomize Pattern")); 
        random_pattern = true;
        set_overwrite_nervous();
  } else if (action == Color_White) {
        // this only sets the color for some of the shows, not changing show
        Serial.println(F("Color White")); 
        white();
        solid();
        random_pattern = false;   // switch into manual mode until explicitly asked again
        set_overwrite_nervous();
  } else if (action == Color_Red) {
        // this only sets the color for some of the shows, not changing show
        Serial.println(F("Color Red")); 
        red();
        set_overwrite_nervous();
  } else if (action == Color_Blue) {
        // this only sets the color for some of the shows, not changing show
        Serial.println(F("Color Blue")); 
  } else if (action == Blending_Overlay) {
        Serial.println(F("Blending Overlay")); 
        set_blending_overlay();
  } else if (action == Off) {
        Serial.println(F("Turning Off")); 
        // TODO
  } else if (action == Speed_Dec) {
        decrease_ghue_speed();
        Serial.println(F("Decrease Speed")); 
  } else if (action == Speed_Inc) {
        increase_ghue_speed();
        Serial.println(F("Increase Speed")); 
  } else if (action == Runtime_Dec) {
        decrease_pattern_runtime();
        Serial.print(F("Increase Pattern Runtime: ")); 
        Serial.println(pattern_runtime);
  } else if (action == Runtime_Inc) {
        increase_pattern_runtime();
        Serial.print(F("Increase Pattern Runtime: ")); 
        Serial.println(pattern_runtime);
  } else if (action == WiFi_On) {
        // TBD wifi_toggle();      // if on, turn off, if off, turn on
        Serial.print(F("Wifi On/Off")); 
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

}