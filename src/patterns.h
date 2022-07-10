#pragma once

// more: https://www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects/#LEDStripEffectBouncingBalls
// https://github.com/FastLED/FastLED/tree/master/examples

#include <heart.h>
#include <sound.h>
#include <FastLED.h>

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

uint8_t gHue = 0;
uint8_t brightness = STARTING_BRIGHTNESS;
CRGB color = CRGB::Red; 

CRGB leds[NUMPIXELS];

// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePattern)();

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gOverlayPattern = 255;      // 255 is off

// Palette definitions
CRGBPalette16 currentPalette = PartyColors_p;
// CRGBPalette16 currentPalette = OceanColors_p;
CRGBPalette16 targetPalette = OceanColors_p;
TBlendType    currentBlending = LINEARBLEND;                  // NOBLEND or LINEARBLEND


void setColor(CRGB col) {
  color = col;
}

void solid_noShow() {
    for (int i=0; i<NUMPIXELS; i++) {
      leds[i] = color;
    }
}

void solid() {
    for (int i=0; i<NUMPIXELS; i++) {
      leds[i] = color;
    }
    FastLED.show();
}

void blue() {

    setColor(CRGB::Blue);
    solid();
    gCurrentPatternNumber = 254; // static pattern

}

void red() {

    setColor(CRGB::Red);
    solid();
    gCurrentPatternNumber = 254; // static pattern

}

void green() {

    setColor(CRGB::Green);
    solid();
    gCurrentPatternNumber = 254; // static pattern

}


void nervous() {
  // what happens when flirted with

  FastLED.setBrightness(brightness);
  brightness = (brightness + 10) % 255;

}

void rainbowCircles() {

    for (int i=0; i<NUMPIXELS; i++) {
       leds[i] = CHSV(gHue*5 + (circles[i] * 20), 255, 255);
    }

}

void rainbowLines() {

    for (int i=0; i<NUMPIXELS; i++) {
      leds[i] = CHSV(gHue + (row[i] * 20), 255, 255);
    }

}

void rainbow() 
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUMPIXELS, gHue, 7);
}

void addGlitter(fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUMPIXELS) ] += CRGB::White;
  }
}

void rainbowWithGlitter() 
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}

void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUMPIXELS, 10);
  int pos = random16(NUMPIXELS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUMPIXELS, 20);
  int pos = beatsin16( 13, 0, NUMPIXELS-1 );
  leds[pos] += CHSV( gHue, 255, 192);
}

void bpm_rings()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);

  for (int rows = 0; rows < NUMELEMENTS(circleMatrix); rows++) {
    for (int cols=0; cols < circleMatrix[rows].numElements; cols++) {
      leds[circleMatrix[rows].circles[cols]] = ColorFromPalette(palette, gHue+(rows*4), beat-gHue+(rows*20));
    }
  }
}

void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUMPIXELS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUMPIXELS, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16( i+7, 0, NUMPIXELS-1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}

// This function draws rainbows with an ever-changing,
// widely-varying set of parameters.
void pride() 
{
  static uint16_t sPseudotime = 0;
  static uint16_t sLastMillis = 0;
  static uint16_t sHue16 = 0;
 
  uint8_t sat8 = beatsin88( 87, 220, 250);
  uint8_t brightdepth = beatsin88( 341, 96, 224);
  uint16_t brightnessthetainc16 = beatsin88( 203, (25 * 256), (40 * 256));
  uint8_t msmultiplier = beatsin88(147, 23, 60);

  uint16_t hue16 = sHue16;//gHue * 256;
  uint16_t hueinc16 = beatsin88(113, 1, 3000);
  
  uint16_t ms = millis();
  uint16_t deltams = ms - sLastMillis ;
  sLastMillis  = ms;
  sPseudotime += deltams * msmultiplier;
  sHue16 += deltams * beatsin88( 400, 5,9);
  uint16_t brightnesstheta16 = sPseudotime;
  
  for( uint16_t i = 0 ; i < NUMPIXELS; i++) {
    hue16 += hueinc16;
    uint8_t hue8 = hue16 / 256;

    brightnesstheta16  += brightnessthetainc16;
    uint16_t b16 = sin16( brightnesstheta16  ) + 32768;

    uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);
    
    CRGB newcolor = CHSV( hue8, sat8, bri8);
    
    uint16_t pixelnumber = i;
    pixelnumber = (NUMPIXELS-1) - pixelnumber;
    
    nblend( leds[pixelnumber], newcolor, 64);
  }
}


void decreaseBrightness() {
  uint8_t b = FastLED.getBrightness();
  if (b > 10)
    FastLED.setBrightness(b - 10);
}

void increaseBrightness() {
  uint8_t b = FastLED.getBrightness();
  if (b < 245)
    FastLED.setBrightness(b + 10);
}

void set_blending_verlay() {
  if (gOverlayPattern == 0) {
    gOverlayPattern = 255;      // turn off if it was on
  } else 
    gOverlayPattern = 0;
}

void blending_overal_off() {
  gOverlayPattern = 255;
}

void sndwave() {
  // https://github.com/atuline/FastLED-SoundReactive/blob/master/sound_wave/sound_wave.ino

  // try to do it by rings ...  

  for (int i=0; i<circleMatrix[0].numElements; i++) {
    // init the innter circle
    leds[circleMatrix[0].circles[i]] = ColorFromPalette(currentPalette, sampleAgc, sampleAgc, currentBlending); // Put the sample into the center
  }

  for (int rows = NUMELEMENTS(circleMatrix)-1; rows > 0; rows--) {
    for (int cols=0; cols < circleMatrix[rows].numElements; cols++) {
      leds[circleMatrix[rows].circles[cols]] = leds[circleMatrix[rows-1].circles[0]];
    }
  }

  /*
  leds[NUMPIXELS/2] = ColorFromPalette(currentPalette, sampleAgc, sampleAgc, currentBlending); // Put the sample into the center
 
  for (int i = NUMPIXELS - 1; i > NUMPIXELS/2; i--) {       //move to the left      // Copy to the left, and let the fade do the rest.
    leds[i] = leds[i - 1];
  }

  for (int i = 0; i < NUMPIXELS/2; i++) {                  // move to the right    // Copy to the right, and let the fade to the rest.
    leds[i] = leds[i + 1];
  }
  */
  
} // sndwave()

void sound_wave() {

  EVERY_N_SECONDS(5) {                                        // Change the palette every 5 seconds.
    for (int i = 0; i < 16; i++) {
      targetPalette[i] = CHSV(random8(), 255, 255);
    }
  }
  
  EVERY_N_MILLISECONDS(100) {                                 // AWESOME palette blending capability once they do change.
    uint8_t maxChanges = 24; 
    nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);
  }


  EVERY_N_MILLIS_I(thistimer,20) {                            // For fun, let's make the animation have a variable rate.
    uint8_t timeval = beatsin8(10,20,50);                     // Use a sinewave for the line below. Could also use peak/beat detection.
    thistimer.setPeriod(timeval);                             // Allows you to change how often this routine runs.
    fadeToBlackBy(leds, NUMPIXELS, 16);                        // 1 = slow, 255 = fast fade. Depending on the faderate, the LED's further away will fade out.
    getSample();
    agcAvg();
    sndwave();
  }

  FastLED.show();

} 

// COOLING: How much does the air cool as it rises?
// Less cooling = taller flames.  More cooling = shorter flames.
// Default 50, suggested range 20-100 
#define COOLING  55
// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKING 120

void fire()
{
  // https://github.com/FastLED/FastLED/blob/master/examples/Fire2012/Fire2012.ino
  static uint8_t heat[NUMPIXELS];

  // Step 1.  Cool down every cell a little
    for( int i = 0; i < NUMPIXELS; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUMPIXELS) + 2));
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= NUMPIXELS - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8() < SPARKING ) {
      int y = random8(7);
      heat[y] = qadd8( heat[y], random8(160,255) );
    }

    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < NUMPIXELS; j++) {
      leds[j] = HeatColor( heat[j]);
    }
}

// Really not exciting by itself
void blending_overlay() {

  // Serial.println( "Blending _ Overlay Active");
  EVERY_N_MILLISECONDS(100) {
    uint8_t maxChanges = 24;
    nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);   // AWESOME palette blending capability.
  }

  EVERY_N_MILLIS(10) {
    fadeToBlackBy(leds, NUMPIXELS, 4);                     // 8 bit, 1 = slow, 255 = fast
    fadeToBlackBy(leds, 1, 32);    
  }

  EVERY_N_SECONDS(5) {                                                                      // Change the target palette to a random one every 5 seconds.
    // static uint8_t baseC = random8();                                                       // You can use this as a baseline colour if you want similar hues in the next line.
    targetPalette = CRGBPalette16(CHSV(random8(), 255, random8(128, 255)), CHSV(random8(), 255, random8(128, 255)), CHSV(random8(), 192, random8(128, 255)), CHSV(random8(), 255, random8(128, 255)));
  }
  FastLED.show();

}

void runningOutside() {
    setColor(CRGB::Black);
    solid_noShow();
    leds[outer_ring_chaser[gHue % ARRAY_SIZE(outer_ring_chaser)]] = CRGB::White;
    FastLED.show();
}

void outside() {
    setColor(CRGB::Black);
    solid_noShow();
    for (int i=0; i<ARRAY_SIZE(outer_ring); i++) {
      leds[outer_ring[i]] = CRGB::White;
    }
    FastLED.show();
  
}

void sound_show() {
  
  blending_overlay();
  getSample();                                                // Sample the microphone.
  agcAvg();                                                   // Calculate the adjusted value as sampleAvg.
  if (samplePeak == 1) { leds[0] = CRGB::Gray; samplePeak = 0;}
  leds[(millis() % (NUMPIXELS-1)) +1] = ColorFromPalette(currentPalette, sampleAgc, sampleAgc, currentBlending);
 
} // ledShow()

void sound_overlay() {
  // TBD
}

void setup_leds() {

  FastLED.addLeds<WS2812B, LED_PIN,GRB>(leds, NUMPIXELS).setCorrection( TypicalLEDStrip );
  FastLED.setMaxPowerInVoltsAndMilliamps( 5, MAX_POWER_MILLIAMPS);
  FastLED.setBrightness(BRIGHTNESS);

}

struct SimplePatternList {
  SimplePattern functPtr;
  const char * const name;
};

SimplePatternList gPatterns[] = { 
                                {bpm_rings, "BMP with Rings"},
                                {fire, "Fire"},
                                {sound_wave, "Sound Wave"},
                                {sound_show, "Sound Show"},
                                {outside, "Outside Only"},
                                {runningOutside, "Running Dot Outside"},
                                {rainbowCircles, "Rainbow Circles"},
                                {pride, "Pride"},
                                {solid, "Solid"},
                                {rainbowLines, "Rainbow Lines"},
                                {rainbow, "Rainbow"},
                                {sinelon, "Sinelon"},
                                {juggle, "Juggle"},
                                {bpm, "BPM"}
                                };

SimplePatternList gOverlay[] = { 
                                {blending_overlay, "Blending for sound_show"}
                               };

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
  Serial.println(gPatterns[gCurrentPatternNumber].name);
}

void previousPattern()
{
  // add one to the current pattern number, and wrap around at the end
  if (gCurrentPatternNumber > 0) gCurrentPatternNumber = (gCurrentPatternNumber - 1); 
  else gCurrentPatternNumber = ARRAY_SIZE(gPatterns) - 1;
  Serial.println(gPatterns[gCurrentPatternNumber].name);
}

void setFlirtPattern()
{
  gCurrentPatternNumber = 0;
}

/*
void setRed() {
  red();
}
*/

void updatePattern() {

  if (gCurrentPatternNumber != 254) {
    gPatterns[gCurrentPatternNumber].functPtr();   // call the helper script
    // FastLED.setBrightness(BRIGHTNESS);
    FastLED.show();

    EVERY_N_MILLISECONDS(100) { gHue = (gHue+1) % 255; }
  }
  if (gOverlayPattern != 255) {
    gOverlay[gOverlayPattern].functPtr();   // call the helper script
  }
}
