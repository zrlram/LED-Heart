#pragma once
/*
https://github.com/Arduino-IRremote/Arduino-IRremote
*/

#include <Arduino.h>

#define IR_RECEIVE_PIN          (17)
#define IR_SEND_PIN             (4)
#define IR_ROLE_PIN             (16)

#if defined(ESP32) || defined(ARDUINO_ARCH_RP2040) || defined(PARTICLE) || defined(ARDUINO_ARCH_MBED)
#define SEND_PWM_BY_TIMER // We do not have pin restrictions for this CPU's, so lets use the hardware PWM for send carrier signal generation
#else
# if defined(SEND_PWM_BY_TIMER)
#undef IR_SEND_PIN // SendPin is determined by timer! This avoids warning in IRTimer.hpp
#  endif
#endif

#if !defined (FLASHEND)
#define FLASHEND 0xFFFF // Dummy value for platforms where FLASHEND is not defined
#endif

// Optimizations
#define EXCLUDE_UNIVERSAL_PROTOCOLS // Saves up to 1000 bytes program memory.
#define EXCLUDE_EXOTIC_PROTOCOLS

//#include <IRremote.h>  
// IRrecv irrecv(IR_RECEIVE_PIN);

#define Flirt 0x11111111
#define Button_BrightUp 0xFA05FF00
#define Button_BrightDown 0xFB04FF00
#define Button_Red 0xF609FF00
#define Button_Green 0xF708FF00

extern boolean is_ir_sender;

void setup_ir();
uint32_t read_ir();
void send_flirt();

