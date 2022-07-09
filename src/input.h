#ifndef input_h
#define input_h

#include <arduino.h>

// https://learn.adafruit.com/adafruit-capacitive-touch-sensor-breakouts/build-a-capacitive-touch-control-panel 

#define TOUCH_PIN       (27)
#define TOUCH_THRESHOLD (20)
#define DEBOUNCE_MS     (50)

boolean previous = false;
boolean previous_steady = false;
long t = 0;

boolean pressed() {
    boolean press;
    boolean ret = false;
    int touch = touchRead(TOUCH_PIN);

    press = (touch < TOUCH_THRESHOLD);

    // If the switch/button changed, due to noise or pressing:
    if (press != previous) {
        // reset the debouncing timer
        t = millis();
        // save the the last flickerable state
        previous = press;
    }

    if ((millis() - t) > DEBOUNCE_MS) {
        // whatever the reading is at, it's been there for longer than the debounce
        // delay, so take it as the actual current state:

        // if the button state has changed:
        if (previous_steady && !press) {
            ret = false;
        }
        else if (!previous_steady && press) {
            ret = true;
        }

        previous_steady = press;
        // save the the last steady state
        return ret;
    }
    return false;

    /*

    // make sure we don't press multiple times
    return (touchRead(TOUCH_PIN) < TOUCH_THRESHOLD);

    if (touchRead(TOUCH_PIN) < TOUCH_THRESHOLD) {
        // make sure it's still pressed after some time
        Serial.println("one");
        delay(DEBOUNCE_MS);
        return (touchRead(TOUCH_PIN) < TOUCH_THRESHOLD);
    }
    return false;
    */
}

int read_touch() {
    return touchRead(TOUCH_PIN);
}

#endif