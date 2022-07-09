#ifndef patterns_h
#define patterns_h

/*
    TODO
    - make patterns dependent on how many LEDs
    - come up with good 1 LED patterns
      - rainbow pulse in brightness
      - rainbow cycle through
      - alarm pulsing
      - strobe
    - go to sleep after a few minutes
      - havea a button to wake up the light
    - receive LED status changes
*/

// more: https://www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects/#LEDStripEffectBouncingBalls

#include <heart.h>

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

uint8_t gHue = 0;
uint8_t brightness = 80;
CRGB color = CRGB::Red; 



void setColor(CRGB col) {
  color = col;
}

void blue() {

    for (int i=0; i<NUMPIXELS; i++) {
      leds[i] = CRGB::Blue;
    }

}

void green() {

    for (int i=0; i<NUMPIXELS; i++) {
      leds[i] = CRGB::Green;
    }

}

void solid() {

    for (int i=0; i<NUMPIXELS; i++) {
      leds[i] = color;
    }

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

// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { nervous, rainbowCircles, pride, solid, green, blue, rainbowLines, rainbow, rainbowWithGlitter, confetti, sinelon, juggle, bpm };

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}

void previousPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber - 1) % ARRAY_SIZE( gPatterns);
}

void setFlirtPattern()
{
  gCurrentPatternNumber = 0;
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

void updatePattern() {

  gPatterns[gCurrentPatternNumber]();   // call the helper script
  // FastLED.setBrightness(BRIGHTNESS);
  FastLED.show();

  EVERY_N_MILLISECONDS(100) { gHue = (gHue+1) % 255; }

}

#endif



