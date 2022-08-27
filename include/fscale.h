#pragma once
#include <Arduino.h>

float fscale(float originalMin, float originalMax, float newBegin, float newEnd, float inputValue, float curve) {

    float OriginalRange = 0;
    float NewRange = 0;
    float zeroRefCurVal = 0;
    float normalizedCurVal = 0;
    float rangedValue = 0;
    bool invFlag = 0; //Invert Flag

    if (curve > 10)
        curve = 10;
    if (curve < -10)
        curve = -10;

    curve = curve * (-.1);
    curve = pow(10, curve);

    if (inputValue < originalMin) {
    inputValue = originalMin;
    }
    if (inputValue > originalMax) {
    inputValue = originalMax;
    }

    //Zero reference the values
    OriginalRange = originalMax - originalMin;
    if (newEnd > newBegin) {
    NewRange = newEnd - newBegin;
    } else {
    NewRange = newBegin - newEnd;
    invFlag = 1; //Invert Flag
    }

    zeroRefCurVal = inputValue - originalMin;
    normalizedCurVal = zeroRefCurVal / OriginalRange; //Normalize to 0-1 float

    //Check for originalMin > orignalMax
    if (originalMin > originalMax) {
    return 0;
    }

    if (invFlag == 0) {
    rangedValue = (pow(normalizedCurVal, curve) * NewRange) + newBegin;
    } else //invert range
    {
    rangedValue = newBegin - (pow(normalizedCurVal, curve) * NewRange);
    }

    return rangedValue;

}