#ifndef heart_h
#define heart_h

#include <FastLED.h>

#define LED_PIN (12)
#define MIC_PIN   (32)                                           // Analog port for microphone

// For sound
uint8_t squelch = 7;                                          // Anything below this is background noise, so we'll make it '0'.
int sample;                                                   // Current sample.
float sampleAvg = 0;                                          // Smoothed Average.
float micLev = 0;                                             // Used to convert returned value to have '0' as minimum.
uint8_t maxVol = 11;                                          // Reasonable value for constant volume for 'peak detector', as it won't always trigger.
bool samplePeak = 0;                                          // Boolean flag for peak. Responding routine must reset this flag.
int sampleAgc, multAgc;
uint8_t targetAgc = 60;                                       // This is our setPoint at 20% of max for the adjusted output.


#define NUMPIXELS (52)
#define BRIGHTNESS (200)

CRGB leds[NUMPIXELS];

// Palette definitions
CRGBPalette16 currentPalette = PartyColors_p;
CRGBPalette16 targetPalette = PartyColors_p;
TBlendType    currentBlending = LINEARBLEND;                  // NOBLEND or LINEARBLEND

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

/* 
static const unsigned char circles[][6] = { { 27, 28 }, // 1
                                        { 16, 17, 18, 26, 29, 36, 37 },  //2
                                        { 7, 8, 15, 19, 25, 30, 35, 38, 43, 44 },  // 3
                                        { 6, 9, 14, 20, 24, 31, 34, 39, 42, 45, 48, 49 },  // 4
                                        { 1, 2, 5, 10, 13, 21, 23, 32, 33, 40, 41, 46, 47, 50, 51}, // 5
                                        { 0, 3, 4, 11, 12, 22} // 6
                                      };
                                      */

static const unsigned char circles[] = { 6, 5, 5, 6, 6, 5, 4, 3, 3, 4, 5, 6, 6, 5, 4, 3, 2, 2, 2, 3, 4, 
                                          5, 6, 5, 4, 3, 2, 1, 1, 2, 3, 
                                          4, 5, 5, 4, 3, 2, 2, 3, 4, 5,
                                          5, 4, 3, 3, 4, 5, 5, 4, 4, 5, 5 
                                        };

#endif