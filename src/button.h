#pragma once

#include <heart.h>
#include <ezButton.h>

#ifdef __USE_BUTTON

#define DEBOUNCE_TIME 50    // the debounce time in millisecond, increase this time if it still chatters

extern ezButton buttons[4];

void setup_button();
void button_loop();
boolean button_pressed(int button);         // 1 = button 1         binary encoded
                                            // 2 = button 2
                                            // 3 = button 1 + 2
                                            // 4 = button 3

#define Flirt 0x1
#define Button_BrightUp 0x2
#define Button_BrightDown 0x3
#define Button_Off 0x4
#define Button_On 0x5

#define Button_Red 0x6
#define Button_Green 0x7
#define Button_Blue 0x8
#define Button_White 0x9

#define Button_Orange 0xA
#define Button_LightGreen 0xB
#define Button_LightBlue 0xC
#define Button_Flash 0xD

#define Button_LightRed 0xE
#define Button_LightLightBlue 0xF
#define Button_Purple 0x10
#define Button_Strobe 0x11

#define Button_LightOrange 0x12
#define Button_LightTurk 0x13
#define Button_DarkPink 0x14
#define Button_Fade 0x15

#define Button_Yellow 0x16
#define Button_Turk 0x17
#define Button_Pink 0x18
#define Button_Smooth 0x19

#endif // __USE_BUTTON