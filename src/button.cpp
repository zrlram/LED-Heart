#include <button.h>

#ifdef __USE_BUTTON

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))


extern ezButton buttons[4] = {
  ezButton(BUTTON_1_PIN),
  ezButton(BUTTON_2_PIN),
  ezButton(BUTTON_3_PIN),
  ezButton(BUTTON_4_PIN),
};

void setup_button() {
  for (byte i = 0; i < ARRAY_SIZE(buttons); i++) {
    buttons[i].setDebounceTime(DEBOUNCE_TIME); 
  }
}

void button_loop() {
    for (byte i = 0; i < ARRAY_SIZE(buttons); i++)
      buttons[i].loop(); 
}

boolean button_pressed(int button) {
    if (buttons[button].isPressed()) {  
        return true;
    } else 
        return false;
}

#endif