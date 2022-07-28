#pragma once

// more: https://www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects/#LEDStripEffectBouncingBalls
// https://github.com/FastLED/FastLED/tree/master/examples

#include <heart.h>
#include <sound.h>
#include <FastLED.h>
#include "arduino-timer.h"

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

#define STATIC_PATTERN (254)

static uint16_t GHUE_SPEED = 50;       // ms to increase gHue   0 - 1000 in 50 steps

static uint8_t gHue = 0;
static uint8_t brightness = STARTING_BRIGHTNESS;
CRGB color = CRGB::Red; 

auto timer = timer_create_default();
bool no_updates = false;          // When we do not want to call update_pattern

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

bool reset_no_update(void *) {
  no_updates = false;
  return false; // repeat?
}

void indicator(uint16_t value, uint16_t maximum, CRGB c = CRGB::Blue) {
  timer.cancel();
  timer.in(1000, reset_no_update); 
  no_updates = true;

  // 10 steps
  FastLED.clear();
  for (int i=0; i<10.0/maximum*value; i++) {
    leds[i+23] = c;
  }
  FastLED.show();
}

void increase_ghue_speed(bool show_indicator = true) {
  if (GHUE_SPEED < 950)
    GHUE_SPEED += 50;
  if (show_indicator) 
    indicator(GHUE_SPEED, 1000, CRGB::Pink);
}

void decrease_ghue_speed(bool show_indicator = true) {
  if (GHUE_SPEED > 50)
    GHUE_SPEED -= 50;
  if (show_indicator) 
    indicator(GHUE_SPEED, 1000, CRGB::Pink);
}

void setColor(CRGB col) {
  color = col;
}

void solid_noShow(CRGB c) {
    for (int i=0; i<NUMPIXELS; i++) {
      leds[i] = c;
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
    gCurrentPatternNumber = STATIC_PATTERN; // static pattern
}

void red() {
    setColor(CRGB::Red);
    solid();
    gCurrentPatternNumber = STATIC_PATTERN; // static pattern
}

void green() {
    setColor(CRGB::Green);
    solid();
    gCurrentPatternNumber = STATIC_PATTERN; 
}

void draw_little_heart() {
  FastLED.clear();
  for (int i=0; i<ARRAY_SIZE(little_heart); i++) {
    leds[little_heart[i]] = CRGB::Red;
  }
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
  for( int i = 0; i < NUMPIXELS; i++) { 
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

  uint16_t hue16 = sHue16; //gHue * 256;
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


void decreaseBrightness(bool show_indicator = true) {
  uint8_t b = FastLED.getBrightness();
  if (show_indicator) indicator(b, 255);
  if (b > 10)
    FastLED.setBrightness(b - 10);
}

void increaseBrightness(bool show_indicator = true) {
  uint8_t b = FastLED.getBrightness();
  if (show_indicator) indicator(b, 255);
  if (b < 245)
    FastLED.setBrightness(b + 10);
}

void set_blending_overlay() {
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

  // TODO: Try - this was every 5 seconds
  // EVERY_N_SECONDS(5) {                                        // Change the palette every 5 seconds.
  EVERY_N_MILLIS(GHUE_SPEED*10) {                                        // Change the palette every 5 seconds.
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
// #define COOLING  55
static uint8_t COOLING = 55;
// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKING 120

void fire()
{
  // https://github.com/FastLED/FastLED/blob/master/examples/Fire2012/Fire2012.ino
  static uint8_t heat[NUMPIXELS];
  COOLING = GHUE_SPEED / 10;    // TODO: Trying - was 55

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
    FastLED.clear();
    leds[outer_ring_chaser[gHue % ARRAY_SIZE(outer_ring_chaser)]] = CRGB::White;
    FastLED.show();
}

void outside() {
    FastLED.clear();
    for (int i=0; i<ARRAY_SIZE(outer_ring); i++) {
      leds[outer_ring[i]] = color;
    }
    FastLED.show();
}

void outside(CRGB color) {
  setColor(color);
  outside();
}


// https://wokwi.com/projects/287675911209222664
// TODO - there are more where this came from - check the simulator
static float fmap(const float x, const float in_min, const float in_max, const float out_min, const float out_max) {
  return (out_max - out_min) * (x - in_min) / (in_max - in_min) + out_min;
}
// TODO: ORIGINAL: uint8_t speed = 15;
uint8_t speed = GHUE_SPEED / 20;
uint8_t scale = 55;
uint8_t amplitude = 255;
//const int8_t semiHeightMajor  =  HEIGHT / 2 + (HEIGHT % 2);
//const int8_t semiWidthMajor = WIDTH / 2  + (WIDTH % 2);
const int8_t semiHeightMajor  =  4;
const int8_t semiWidthMajor = 6;
float e_s3_speed = 0.004 * speed + 0.015; // speed of the movement along the Lissajous curves

void sinusoid() {

  float e_s3_size = fmap(amplitude, 1, 255, 3, 9); // amplitude of the curves
  float time_shift = millis();
  int8_t _scale = map8(scale, 50, 150);

  // case 5: //changed by stepko //colored sinusoid
  for (uint8_t y = 0; y < HEIGHT; y++) {
        for (uint8_t x = 0; x < WIDTH; x++) {
          
          float cx = (y - semiHeightMajor) + float(e_s3_size * (sin16(e_s3_speed * 98.301 * time_shift))) / 32767.0; // the 8 centers the middle on a 16x16
          float cy = (x - semiWidthMajor) + float(e_s3_size * (cos16(e_s3_speed * 72.0874 * time_shift))) / 32767.0;
          int8_t v = 127 * (1 + sin16(127 * _scale * sqrt((((float) cx * cx) + ((float) cy * cy))) + (time_shift * e_s3_speed * 5)) / 32767.0);
          leds[XY(x, y)].r = v;
          
          v = 127 * (1 + sin16(127 * _scale * sqrt((((float) cx * cx) + ((float) cy * cy))) + (time_shift * e_s3_speed * 10)) / 32767.0);
          /*
          Serial.print(v);
          Serial.print(", ");
          Serial.print(cx);
          Serial.print(";  ");
          */
          leds[XY(x, y)].b = v;
          
          v = 127 * (1 + sin16(127 * _scale * sqrt((((float) cx * cx) + ((float) cy * cy))) + (time_shift * e_s3_speed * 50)) / 32767.0);
          leds[XY(x, y)].g = v;
          
        }
  }

  FastLED.show();
  
}

void nervous(bool is_sender=false) {
  // what happens when flirted with
  // Serial.println("pretending to be nervous");

  if (is_sender) {
    outside(CRGB::LightBlue);
  } else {
    outside(CRGB::Pink);
  }

  // TBD - nice blinking 
  //FastLED.setBrightness(brightness);
  //brightness = (brightness + 10) % 255;

  gCurrentPatternNumber = STATIC_PATTERN; 

  /* TBD
  if (!is_sender) 
    draw_little_heart();     // no forget the baby
    */

  // TBD when getting out of nervous, reset the brightness


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

  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUMPIXELS).setCorrection( TypicalLEDStrip );
  FastLED.setMaxPowerInVoltsAndMilliamps( 5, MAX_POWER_MILLIAMPS);
  FastLED.setBrightness(BRIGHTNESS);

}

struct SimplePatternList {
  SimplePattern functPtr;
  const char * const name;
};

SimplePatternList gPatterns[] = { 
                                {sinusoid, "SinusOID"},
                                {draw_little_heart, "Little Heart"},
                                {bpm_rings, "BMP with Rings"},
                                {fire, "Fire"},
                                {sound_wave, "Sound Wave"},
                                {sound_show, "Sound Show"},
                                {outside, "Outside Only"},
                                {runningOutside, "Running Dot Outside"},
                                {rainbowCircles, "Rainbow Circles"},
                                {confetti, "Confetti"},
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

void updatePattern() {
  if (!no_updates) {
    if (gCurrentPatternNumber != STATIC_PATTERN) {
      gPatterns[gCurrentPatternNumber].functPtr();   // call the helper script
      // FastLED.setBrightness(BRIGHTNESS);
      FastLED.show();

      EVERY_N_MILLIS_I( thistimer, 50 ) { // initial period = 100ms
        thistimer.setPeriod(GHUE_SPEED);
        gHue = (gHue+1) % 255; 
      }
    }
    if (gOverlayPattern != 255) {
      gOverlay[gOverlayPattern].functPtr();   // call the helper script
    }
  }
}
