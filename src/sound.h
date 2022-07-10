#pragma once

extern bool samplePeak;                                          // Boolean flag for peak. Responding routine must reset this flag.
extern int sampleAgc, multAgc;

void getSample();
void agcAvg();
