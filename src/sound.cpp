#include <Arduino.h>
#include <sound.h>
#include <heart.h>

/* 
  Other things to read about / look at:

  Different schematics and code for breat detection
    https://create.arduino.cc/projecthub/mamifero/arduino-beat-detector-d0a21f

*/

// TODO - play with squelch??? to suppress some more noise?
uint8_t squelch = 7;                                          // Anything below this is background noise, so we'll make it '0'. Was 7
int sample;                                                   // Current sample.
float sampleAvg = 0;                                          // Smoothed Average.
float micLev = 0;                                             // Used to convert returned value to have '0' as minimum.
uint8_t maxVol = 11;                                          // Reasonable value for constant volume for 'peak detector', as it won't always trigger.
uint8_t targetAgc = 60;                                       // This is our setPoint at 20% of max for the adjusted output.
bool samplePeak = 0;                                          // Boolean flag for peak. Responding routine must reset this flag.
int sampleAgc, multAgc;

void setup_sound() {
  // https://www.atomic14.com/2020/09/12/esp32-audio-input.html

  pinMode(MIC_PIN, INPUT);
}


void getSample() {
  
  int16_t micIn;                                              // Current sample starts with negative values and large values, which is why it's 16 bit signed.
  static long peakTime;
  
  micIn = analogRead(MIC_PIN)/4.0;                                // Poor man's analog Read. // div by 4 for ESP32 - [0, 1024]

  // Serial.print(micIn);
  // Serial.print(" ");
  micLev = ((micLev * 31) + micIn) / 32;                      // Smooth it out over the last 32 samples for automatic centering.
  micIn -= micLev;                                            // Let's center it to 0 now.
  micIn = abs(micIn);                                         // And get the absolute value of each sample.
  sample = (micIn <= squelch) ? 0 : (sample + micIn)/2;       // Using a ternary operator, the resultant sample is either 0 or it's a bit smoothed out with the last sample.
  sampleAvg = ((sampleAvg * 31) + sample) / 32;               // Smooth it out over the last 32 samples.

  if (sample > (sampleAvg+maxVol) && millis() > (peakTime + 50)) {    // Poor man's beat detection by seeing if sample > Average + some value.
    samplePeak = 1;
    peakTime=millis();
  }                                                           // Then we got a peak, else we don't. Display routines need to reset the samplepeak value in case they miss the trigger.
  // Serial.println();
  
}  // getSample()



void agcAvg() {                                                   // A simple averaging multiplier to automatically adjust sound sensitivity.

  // https://github.com/atuline/FastLED-SoundReactive/blob/master/agcAvg_Pal/agcAvg_Pal.ino

  multAgc = (sampleAvg < 1) ? targetAgc : targetAgc / sampleAvg;  // Make the multiplier so that sampleAvg * multiplier = setpoint
  sampleAgc = sample * multAgc;
  if (sampleAgc > 255) sampleAgc = 255;

//------------ Oscilloscope output ---------------------------
  //Serial.print(targetAgc); Serial.print(" ");
  //Serial.print(multAgc); Serial.print(" ");
   //Serial.print(sampleAgc); Serial.print(" ");

  //Serial.print(sample); Serial.print(" ");
  //Serial.print(sampleAvg); Serial.print(" ");
//  Serial.print(micLev); Serial.print(" ");
  //if (samplePeak) Serial.print(samplePeak*100); Serial.print(" "); 
//  Serial.print(100); Serial.print(" ");
//  Serial.print(0); Serial.print(" ");
if (DEBUG) Serial.println();

} // agcAvg()

