#pragma once
#include "driver/adc.h"

extern bool samplePeak;                                          // Boolean flag for peak. Responding routine must reset this flag.
extern int sampleAgc, multAgc;

void setup_sound();
void getSample();
void agcAvg();
