#pragma once

#define LED_PIN   (12)
#define MIC_PIN   (32)                                           // Analog port for microphone
#define ROLE_PIN  (16)                  // for BLE flirting

#define NUMPIXELS (52)
#define BRIGHTNESS (200)
#define MAX_POWER_MILLIAMPS (700)
#define STARTING_BRIGHTNESS (150)

#define IR_RECEIVE_PIN          (17)
#define IR_SEND_PIN             (4)
#define IR_ROLE_PIN             (16)

static const unsigned char outer_ring_chaser[] = {0, 1, 7, 17, 8, 2, 3, 11, 
                                                  22, 32, 40, 46, 50, 51, 47, 41, 33, 23, 12, 4};

static const unsigned char outer_ring[] = {0, 1, 2, 3,
                                          4, 7, 8, 11,
                                          12, 17, 22,
                                          23, 32, 
                                          33, 40,
                                          41, 46,
                                          47, 50,
                                          51 };   
static const unsigned char ROWS[] = {4, 8, 11, 10, 8, 6, 4, 1};
static const unsigned char row[] = {
                                 0,0,0,0,
                                 1,1,1,1,1,1,1,1,
                                 2,2,2,2,2,2,2,2,2,2,2,
                                 3,3,3,3,3,3,3,3,3,3,
                                 4,4,4,4,4,4,4,4,
                                 5,5,5,5,5,5,
                                 6,6,6,6,
                                 7
                                 };

#define NUMELEMENTS(x) (sizeof(x) / sizeof(x[0]))

static const unsigned char circle_1[] = { 27, 28 }; 
static const unsigned char circle_2[] = { 16, 17, 18, 26, 29, 36, 37 };
static const unsigned char circle_3[] = { 7, 8, 15, 19, 25, 30, 35, 38, 43, 44 };
static const unsigned char circle_4[] = { 6, 9, 14, 20, 24, 31, 34, 39, 42, 45, 48, 49 };
static const unsigned char circle_5[] = { 1, 2, 5, 10, 13, 21, 23, 32, 33, 40, 41, 46, 47, 50, 51};
static const unsigned char circle_6[] = { 0, 3, 4, 11, 12, 22};
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

static const unsigned char circles[] = { 6, 5, 5, 6, 6, 5, 4, 3, 3, 4, 5, 6, 6, 5, 4, 3, 2, 2, 2, 3, 4, 
                                          5, 6, 5, 4, 3, 2, 1, 1, 2, 3, 
                                          4, 5, 5, 4, 3, 2, 2, 3, 4, 5,
                                          5, 4, 3, 3, 4, 5, 5, 4, 4, 5, 5 
                                        };


