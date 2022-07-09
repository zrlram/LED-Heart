#include <Arduino.h>
//#include <U8x8lib.h>

#include <heart.h>
#include <patterns.h>
#include <ir.h>
#include <ble_distance.h>
// #include <mesh_wifi.h>
// #include <input.h>
// #include <bluetooth.h>

#ifdef __AVR__
  #include <avr/power.h>
#endif

#define MAX_POWER_MILLIAMPS 1000

//U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ 15, /* data=*/ 4, /* reset=*/ 16);
#define DELAYVAL 500

/* for WiFi 
flirt_message_struct flirt_data;

flirt_message_struct myData;
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  setFlirtPattern();
  */
  /*
  Serial.print("Look! Bytes received: ");
  Serial.println(len);
  Serial.print("Bool: ");
  Serial.println(myData.flirt);
  Serial.println();
  */
/*
}
*/


void setup() {
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
  #endif

  Serial.begin(9600);


  FastLED.addLeds<WS2812B, LED_PIN,GRB>(leds, NUMPIXELS).setCorrection( TypicalLEDStrip );
  FastLED.setMaxPowerInVoltsAndMilliamps( 5, MAX_POWER_MILLIAMPS);
  FastLED.setBrightness(BRIGHTNESS);

  /*
  u8x8.begin();
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.drawString(0, 0, "LED Heart ");
  u8x8.drawString(0, 1, "Beating ...");
  u8x8.drawString(0, 2, "Pattern: ");
  u8x8.drawString(0, 3, "Hue: ");
  */

  setup_ir();
  //setup_wifi();
  //set_callback(OnDataRecv);

  //ble_setup();

  // TBD just for now:::
  nextPattern();

 

}

void ledShow() {
  
  if (samplePeak == 1) { leds[0] = CRGB::Gray; samplePeak = 0;}
  leds[(millis() % (NUMPIXELS-1)) +1] = ColorFromPalette(currentPalette, sampleAgc, sampleAgc, currentBlending);
 
} // ledShow()



void getSample() {
  
  int16_t micIn;                                              // Current sample starts with negative values and large values, which is why it's 16 bit signed.
  static long peakTime;
  
  micIn = analogRead(MIC_PIN);                                // Poor man's analog Read.
  micLev = ((micLev * 31) + micIn) / 32;                      // Smooth it out over the last 32 samples for automatic centering.
  micIn -= micLev;                                            // Let's center it to 0 now.
  micIn = abs(micIn);                                         // And get the absolute value of each sample.
  sample = (micIn <= squelch) ? 0 : (sample + micIn)/2;       // Using a ternary operator, the resultant sample is either 0 or it's a bit smoothed out with the last sample.
  sampleAvg = ((sampleAvg * 31) + sample) / 32;               // Smooth it out over the last 32 samples.

  if (sample > (sampleAvg+maxVol) && millis() > (peakTime + 50)) {    // Poor man's beat detection by seeing if sample > Average + some value.
    samplePeak = 1;
    peakTime=millis();
  }                                                           // Then we got a peak, else we don't. Display routines need to reset the samplepeak value in case they miss the trigger.
  
}  // getSample()



void agcAvg() {                                                   // A simple averaging multiplier to automatically adjust sound sensitivity.

  // https://github.com/atuline/FastLED-SoundReactive/blob/master/agcAvg_Pal/agcAvg_Pal.ino

  multAgc = (sampleAvg < 1) ? targetAgc : targetAgc / sampleAvg;  // Make the multiplier so that sampleAvg * multiplier = setpoint
  sampleAgc = sample * multAgc;
  if (sampleAgc > 255) sampleAgc = 255;

//------------ Oscilloscope output ---------------------------
//  Serial.print(targetAgc); Serial.print(" ");
//  Serial.print(multAgc); Serial.print(" ");
//  Serial.print(sampleAgc); Serial.print(" ");

//  Serial.print(sample); Serial.print(" ");
//  Serial.print(sampleAvg); Serial.print(" ");
//  Serial.print(micLev); Serial.print(" ");
//  Serial.print(samplePeak); Serial.print(" "); samplePeak = 0;
//  Serial.print(100); Serial.print(" ");
//  Serial.print(0); Serial.print(" ");

} // agcAvg()


void loop() {

  // EVERY_N_SECONDS(15) {nextPattern();}

  /*
  if (pressed()) {
    nextPattern();
  }
  */

  switch(read_ir()) {
      case Button_Red: 
        Serial.println(F("RED - Next Show"));
        nextPattern();
      break;
      case Button_Green: 
        Serial.println(F("Green - Previous Show")); 
        previousPattern();
      break;
      case Button_BrightDown: 
        Serial.println(F("Decrease Brightness")); 
        decreaseBrightness();
      break;
      case Button_BrightUp: 
        Serial.println(F("Increase Brightness")); 
        increaseBrightness();
      break;
  }

  EVERY_N_MILLISECONDS(100) {
    uint8_t maxChanges = 24;
    nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);   // AWESOME palette blending capability.
  }

  EVERY_N_MILLIS(10) {
    fadeToBlackBy(leds, NUMPIXELS, 4);                     // 8 bit, 1 = slow, 255 = fast
    fadeToBlackBy(leds, 1, 32);    
  }

  EVERY_N_SECONDS(5) {                                                                      // Change the target palette to a random one every 5 seconds.
    static uint8_t baseC = random8();                                                       // You can use this as a baseline colour if you want similar hues in the next line.
    targetPalette = CRGBPalette16(CHSV(random8(), 255, random8(128, 255)), CHSV(random8(), 255, random8(128, 255)), CHSV(random8(), 192, random8(128, 255)), CHSV(random8(), 255, random8(128, 255)));
  }

  getSample();                                                // Sample the microphone.
  agcAvg();                                                   // Calculate the adjusted value as sampleAvg.
  ledShow();
  FastLED.show();

  //ble_loop();

  //void touchAttachInterrupt(uint8_t pin, void (*userFunc)(void), uint16_t threshold);

  //updatePattern();

  /*
  EVERY_N_MILLISECONDS(200) {
    char buf[5];
    snprintf (buf, 5, "%2d", gCurrentPatternNumber);
    u8x8.drawString(8, 2, buf);
    snprintf (buf, 5, "%4d", gHue);
    u8x8.drawString(5, 3, buf);
    //snprintf (buf, 5, "%4d", read_touch());
    //u8x8.drawString(5, 4, buf);
  }
  */

  /* For WIFI
  EVERY_N_SECONDS(5) {
      if (wifi_sender()) {
          // Set values to send
          flirt_data.flirt = false;
          
          // Send message via ESP-NOW
          if (wifi_send (flirt_data)) {
            Serial.println("Sent with success");
          } else {
            Serial.println("Error sending the data");
          }
      }
  }
  */

  /*
  EVERY_N_SECONDS(5) {
    if (is_ir_sender) {
      // start to flirt
      send_flirt();
    }
  }
  */

  /*

  for(int i=0; i<NUMPIXELS; i++) {

    leds[i] = CRGB::Blue;
    //pixels.setPixelColor(i-2, pixels.Color(150, 0, 0));
    //pixels.setPixelColor(i-1, pixels.Color(0, 0, 0));
  }
  FastLED.show();
  delay(DELAYVAL);


  for(int i=0; i<NUMPIXELS; i++) {

    leds[i] = CRGB::LightBlue;
    //pixels.setPixelColor(i-2, pixels.Color(150, 0, 0));
    //pixels.setPixelColor(i-1, pixels.Color(0, 0, 0));
  }
  FastLED.show();
  delay(DELAYVAL);
  */
}