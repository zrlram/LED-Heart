#include <button.h>

#ifdef __USE_BUTTON

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))


InterruptButton buttons[4] = {
  InterruptButton(BUTTON_1_PIN, HIGH),
  InterruptButton(BUTTON_2_PIN, HIGH),
  InterruptButton(BUTTON_3_PIN, HIGH),
  InterruptButton(BUTTON_4_PIN, HIGH),
};

void setup_button() {
}

#endif