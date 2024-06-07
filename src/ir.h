#pragma once
/*
https://github.com/Arduino-IRremote/Arduino-IRremote
*/

#include <heart.h>

#ifdef __USE_IR

#include <Arduino.h>

#if defined(ESP32) || defined(ARDUINO_ARCH_RP2040) || defined(PARTICLE) || defined(ARDUINO_ARCH_MBED)
#define SEND_PWM_BY_TIMER // We do not have pin restrictions for this CPU's, so lets use the hardware PWM for send carrier signal generation
//#else
// #if defined(SEND_PWM_BY_TIMER) #undef IR_SEND_PIN // SendPin is determined by timer! This avoids warning in IRTimer.hpp #  endif
#endif

#if !defined (FLASHEND)
#define FLASHEND 0xFFFF // Dummy value for platforms where FLASHEND is not defined
#endif

// Optimizations
#define EXCLUDE_UNIVERSAL_PROTOCOLS // Saves up to 1000 bytes program memory.
#define EXCLUDE_EXOTIC_PROTOCOLS

//#include <IRremote.h>  
// IRrecv irrecv(IR_RECEIVE_PIN);

// Wide IR remote
#define Flirt 0x11111111
#define Button_BrightUp 0xFA05FF00
#define Button_BrightDown 0xFB04FF00
#define Button_Off 0xF906FF00
#define Button_On 0xF807FF00

#define Button_Red 0xF609FF00
#define Button_Green 0xF708FF00
#define Button_Blue 0xF50AFF00
#define Button_White 0xF40BFF00

#define Button_Orange 0xF20DFF00
#define Button_LightGreen 0xF30CFF00
#define Button_LightBlue 0xF10EFF00
#define Button_Flash 0xF00FFF00

#define Button_LightRed 0xEA15FF00
#define Button_LightLightBlue 0xEB14FF00
#define Button_Purple 0xE916FF00
#define Button_Strobe 0xE817FF00

#define Button_LightOrange 0xE619FF00
#define Button_LightTurk 0xE718FF00
#define Button_DarkPink 0xE51AFF00
#define Button_Fade 0xE41BFF00

#define Button_Yellow 0xEE11FF00
#define Button_Turk 0xEF10FF00
#define Button_Pink 0xED12FF00
#define Button_Smooth 0xEC13FF00

// Narrow IR remote
#define Button_Power 0xBA45FF00
#define Button_Return 0xB946FF00
#define Button_Mode 0xB847FF00
#define Button_CH_Minus 0xBB44FF00
#define Button_CH_Plus 0xBF40FF00
#define Button_EQ 0xBC43FF00
#define Button_Rew 0xF807FF00           // ON
#define Button_Forw 0xEA15FF00          // LightRed
#define Button_Play 0xF609FF00          // Red
#define Button_Silent 0xE916FF00        // Purple
#define Button_Loud 0xE619FF00
#define Button_0 0xF20DFF00             // Orange
#define Button_1 0xF30CFF00             // LightGreen
#define Button_2 0xE718FF00             // LightTurk
#define Button_3 0xA15EFF00
#define Button_4 0xF708FF00             // Green
#define Button_5 0xE31CFF00
#define Button_6 0xA55AFF00
#define Button_7 0xBD42FF00
#define Button_8 0xAD52FF00
#define Button_9 0xB54AFF00

extern boolean is_ir_sender;

void setup_ir();
uint32_t read_ir();
void send_flirt();
void debug_ir();

#endif