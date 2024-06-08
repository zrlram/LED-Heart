#pragma once
#include <Arduino.h>

#define DEBUG               0
#define DEBUG_IR            1

#undef  __USE_IR                        // use either InfraRed remote or buttons to control actions
#define __USE_BUTTON                    // to be used with #ifdef 

#define LED_PIN             (12)
#define MIC_PIN             (32)                  // Analog port for microphone
#define ROLE_PIN            (15)                  // for BLE flirting

#define NUMPIXELS           (54)
#define BRIGHTNESS          (100)
#define MAX_POWER_MILLIAMPS (700)
#define STARTING_BRIGHTNESS (150)
#define ERROR_LED           (NUMPIXELS + 1)

#ifdef __USE_IR
  #define IR_RECEIVE_PIN      (17)
  #define IR_SEND_PIN         (4)
  #define IR_ROLE_PIN         (16)
#endif

#ifdef __USE_BUTTON       
  #define BUTTON_1_PIN        (17)
  #define BUTTON_2_PIN        (16)
  #define BUTTON_3_PIN        (2)
  #define BUTTON_4_PIN        (21)
#endif

static const unsigned char outer_ring_chaser[] = {1, 0, 5, 17, 11, 6, 7, 8, 
                                                  22, 32, 40, 46, 50, 52, 54, 51, 47, 41, 33, 23, 12, 2};

static const unsigned char outer_ring[] = {
                                       /* 0, 1, 2, 3,
                                          4, 7, 8, 11,
                                          12, 17, 22,
                                          23, 32, 
                                          33, 40,
                                          41, 46,
                                          47, 50,
                                          51 };   */

                                          53, 51, 47, 41, 33, 23, 12, 2, 1, 0, 5, 17, 11, 6, 7, 8,
                                          22, 32, 40, 46, 50, 52 };

static const unsigned char little_heart[] = { 36, 37, 26, 27, 28, 29, 16, 18 };
static const unsigned char ROWS[] = {4, 8, 11, 10, 8, 6, 4, 2, 1};
static const unsigned char row[] = {
                                 0,0,0,0,
                                 1,1,1,1,1,1,1,1,
                                 2,2,2,2,2,2,2,2,2,2,2,
                                 3,3,3,3,3,3,3,3,3,3,
                                 4,4,4,4,4,4,4,4,
                                 5,5,5,5,5,5,
                                 6,6,6,6,
                                 7,7,
                                 8
                                 };

#define NUMELEMENTS(x) (sizeof(x) / sizeof(x[0]))

// these guys start from 0, not 1!
static const unsigned char circle_1[] = { 27, 28 }; 
static const unsigned char circle_2[] = { 16, 17, 18, 26, 29, 36, 37 };
static const unsigned char circle_3[] = { 11, 5, 15, 19, 25, 30, 35, 38, 43, 44 };
static const unsigned char circle_4[] = { 10, 4, 14, 20, 24, 31, 34, 39, 42, 45, 48, 49 };
static const unsigned char circle_5[] = { 6, 0, 3, 9, 13, 21, 23, 32, 33, 40, 41, 46, 47, 50, 51, 52};
static const unsigned char circle_6[] = { 7, 8, 1, 2, 12, 22, 53};
struct CIRCLES {
  const unsigned char* circles;
  uint16_t numElements;
};
CIRCLES const circleMatrix[] = {
  {circle_1, NUMELEMENTS(circle_1)},
  {circle_2, NUMELEMENTS(circle_2)},
  {circle_3, NUMELEMENTS(circle_3)},
  {circle_4, NUMELEMENTS(circle_4)},
  {circle_5, NUMELEMENTS(circle_5)},
  {circle_6, NUMELEMENTS(circle_6)},
};

// TBD
static const unsigned char circles[] = { 6, 5, 5, 6, 6, 5, 5, 4, 3, 3, 4, 5, 6, 6, 5, 4, 3, 2, 2, 2, 3, 4, 
                                          5, 6, 5, 4, 3, 2, 1, 1, 2, 3, 
                                          4, 5, 5, 4, 3, 2, 2, 3, 4, 5,
                                          5, 4, 3, 3, 4, 5, 5, 4, 4, 5, 5 ,
                                          0
                                        };

#define WIDTH  (11)
#define HEIGHT (9)
// the main grid and direction of how the heart is wired - these guys start from 1, not 0
static const uint8_t xy[HEIGHT][WIDTH] = {                  { ERROR_LED, ERROR_LED, 8, 7, ERROR_LED, ERROR_LED, ERROR_LED, 1, 2, ERROR_LED, ERROR_LED },
                                                            { ERROR_LED, 9, 10, 11, 12, ERROR_LED, 6, 5, 4, 3, ERROR_LED },
                                                            { 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13 },
                                                            { 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, ERROR_LED },
                                                            { ERROR_LED, 41, 40, 39, 38, 37, 36, 35, 34, ERROR_LED, ERROR_LED },
                                                            { ERROR_LED, ERROR_LED, 47, 46, 45, 44, 43, 42, ERROR_LED, ERROR_LED, ERROR_LED },
                                                            { ERROR_LED, ERROR_LED, ERROR_LED, 51, 50, 49, 48, ERROR_LED, ERROR_LED, ERROR_LED, ERROR_LED }, 
                                                            { ERROR_LED, ERROR_LED, ERROR_LED, ERROR_LED, 53, 52, ERROR_LED, ERROR_LED, ERROR_LED, ERROR_LED, ERROR_LED }, 
                                                            { ERROR_LED, ERROR_LED, ERROR_LED, ERROR_LED, 54, ERROR_LED, ERROR_LED, ERROR_LED, ERROR_LED, ERROR_LED, ERROR_LED } 
                                        };

uint8_t XY( uint8_t x, uint8_t y);
void setup_heart();
bool is_server();

#define MENU_OPTIONS  (5)     // there are a total of 5 options

extern int menu_index;

// button action code mappings
extern uint32_t Next_Show;
extern uint32_t Prev_Show;
extern uint32_t Bright_Down;
extern uint32_t Bright_Up;
extern uint32_t Randomize_Pattern;
extern uint32_t Color_White;
extern uint32_t Color_Red;
extern uint32_t Color_Blue;
extern uint32_t Blending_Overlay;
extern uint32_t Off;
extern uint32_t Speed_Dec;
extern uint32_t Speed_Inc;
extern uint32_t Runtime_Dec;
extern uint32_t Runtime_Inc;
extern uint32_t WiFi_On;