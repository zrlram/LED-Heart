#pragma once

// more: https://www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects/#LEDStripEffectBouncingBalls
// https://github.com/FastLED/FastLED/tree/master/examples


// TODO - Use the peak detector to advance the gHue? Or some variable that can be used as an overlay to shows

#include <heart.h>
#include <sound.h>
#include <FastLED.h>
#include "arduino-timer.h"

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

#define STATIC_PATTERN (254)

static uint16_t GHUE_SPEED = 50;       // ms to increase gHue   0 - 1000 in 50 steps

static uint8_t gHue = 0;
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

void white() {
    setColor(CRGB::White);
}

void blue() {
    setColor(CRGB::Blue);
}

void red() {
    setColor(CRGB::Red);
}

void green() {
    setColor(CRGB::Green);
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

void bpm_rings_beat()
{

  CRGBPalette16 palette = OceanColors_p;
  //uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  uint8_t beat = beatsin8( 60, 64, 255);

  for (int rows = 0; rows < NUMELEMENTS(circleMatrix); rows++) {
    for (int cols=0; cols < circleMatrix[rows].numElements; cols++) {
      leds[circleMatrix[rows].circles[cols]] = ColorFromPalette(palette, gHue+(rows*4), beat-gHue+(rows*20));
    }
  }
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

// Helper Function
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
} // sndwave()

void sound_wave_color() {
  // uses sound_wave, but stays with one palette based on color
  EVERY_N_MILLIS(1000) {                                        // Update every second
    for (int i = 0; i < 16; i++) {
      targetPalette[i] = color;
    }
  }
  
  EVERY_N_MILLISECONDS(100) {                                 // This controls the fade speed to the new color
    uint8_t maxChanges = 24; 
    nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);
  }

  EVERY_N_MILLIS_I(thistimer,20) {                            // For fun, let's make the animation have a variable rate.
    uint8_t timeval = beatsin8(10,20,50);                     // Use a sinewave for the line below. Could also use peak/beat detection.
    thistimer.setPeriod(timeval);                             // Allows you to change how often this routine runs.
    // fadeToBlackBy(leds, NUMPIXELS, 16);                    // 1 = slow, 255 = fast fade. Depending on the faderate, the LED's further away will fade out.
    fadeToBlackBy(leds, NUMPIXELS, GHUE_SPEED/3);             // 1 = slow, 255 = fast fade. Depending on the faderate, the LED's further away will fade out.
    getSample();
    agcAvg();
    sndwave();
  }
}

void sound_wave() {

  // TODO: Try - this was every 5 seconds
  // EVERY_N_SECONDS(5) {                                     // Change the palette every 5 seconds.
  EVERY_N_MILLIS(GHUE_SPEED*10) {                             
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
    //fadeToBlackBy(leds, NUMPIXELS, 16);                     // 1 = slow, 255 = fast fade. Depending on the faderate, the LED's further away will fade out.
    fadeToBlackBy(leds, NUMPIXELS, GHUE_SPEED/3);             // 1 = slow, 255 = fast fade. Depending on the faderate, the LED's further away will fade out.
    getSample();
    agcAvg();
    sndwave();
  }

  FastLED.show();

} 

// COOLING: How much does the air cool as it rises?
// Less cooling = taller flames.  More cooling = shorter flames.
// Default 50, suggested range 20-100 
static uint8_t COOLING = 55;
// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKING 140

void fire()
{
  // https://github.com/FastLED/FastLED/blob/master/examples/Fire2012/Fire2012.ino
  static uint8_t heat[NUMPIXELS];
  COOLING = GHUE_SPEED;    // TODO: Trying - was 55

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

void outside_sound() {

  EVERY_N_MILLIS(40) {
    getSample();                                                // Sample the microphone.
    agcAvg();                                                   // Calculate the adjusted value as sampleAvg.
    //Serial.print("S: ");
    //Serial.println(sampleAgc);

    FastLED.clear();
    for (int i=0; i<ARRAY_SIZE(outer_ring); i++) {
      CHSV c = rgb2hsv_approximate( color );
      c.v = sampleAgc;
      leds[outer_ring[i]] = c;
    }
    FastLED.show();
  }
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

void sound_sample()  {
  //  agcAvg_Pal * By: Andrew Tuline

  EVERY_N_MILLISECONDS(GHUE_SPEED*10) {
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
  
  if (samplePeak == 1) { leds[0] = CRGB::Gray; samplePeak = 0;}
  leds[(millis() % (NUMPIXELS-1)) +1] = ColorFromPalette(currentPalette, sampleAgc, sampleAgc, currentBlending);
 
  FastLED.show();

}

// https://wokwi.com/projects/289979978497393160
// Quite boring
byte i,t,u;
void sindots() {
    t=millis()/15;
    u=t-10;
    for(i=4;i--;)   // how thick the line is 
      leds[XY(sin8(t+i*2)/16,sin8(u+i*2)/32)].setHue(t);  // sin = [0,255] divide by 16 and 32 to fill the heart
    for(i=4;i--;)
      leds[XY(WIDTH-(sin8(t+i*2)/16),sin8(u+i*2)/32)].setHue(t);

    blur2d(leds,32,32,32);
    LEDS.show();
}

//  https://wokwi.com/projects/288441747546046984
void mydrawLine (byte x, byte y, byte x1, byte y1, CRGB color, bool dot){   // my ugly line draw function )))
  byte xsteps = abs8(x-x1)+1;  
  byte ysteps = abs8(y-y1)+1;
  byte steps =  xsteps >= ysteps? xsteps:ysteps;

  for (byte i = 1; i <= steps; i++) {
    byte dx = lerp8by8 (x, x1, i*255/steps);
    byte dy = lerp8by8 (y, y1, i*255/steps);
    leds[XY(dx, dy)] += color;    // change to += for brightness look
  }

  if (dot) {     //add white point at the ends of line 
    leds[XY(x,y)] += CRGB ::White;
    leds[XY(x1,y1)] += CRGB ::White;
  }
}

void flyingcircular() {

  byte x1 = beatsin8 (18, 0, (WIDTH-1));
  byte x2 = beatsin8 (23, 0, (WIDTH-1)); 
  byte x3 = beatsin8 (27, 0, (WIDTH-1)); 
  byte x4 = beatsin8 (31, 0, (WIDTH-1)); 

  byte y1 = beatsin8 (20, 0, (HEIGHT-1)); 
  byte y2 = beatsin8 (26, 0, (HEIGHT-1));
  byte y3 = beatsin8 (15, 0, (HEIGHT-1));
  byte y4 = beatsin8 (27, 0, (HEIGHT-1));

  CRGB color = CHSV (gHue,255,BRIGHTNESS);

  fadeToBlackBy (leds, NUMPIXELS, 30 );

  mydrawLine(x1, y1,  x2, y2, color,1);
  mydrawLine(x2, y2,  x3, y3, color,1);
  mydrawLine(x2, y2,  x4, y4, color,1);
  mydrawLine(x3, y3,  x4, y4, color,1);
  mydrawLine(x3, y3,  x1, y1, color,1);
  mydrawLine(x4, y4,  x1, y1, color,1);

  blur2d (leds, WIDTH, HEIGHT, 64 );
  
  FastLED.show();

} 


// https://wokwi.com/projects/287675911209222664
// TODO - there are more where this came from - check the simulator
static float fmap(const float x, const float in_min, const float in_max, const float out_min, const float out_max) {
  return (out_max - out_min) * (x - in_min) / (in_max - in_min) + out_min;
}
uint8_t speed = GHUE_SPEED / 20;
uint8_t scale = 55;
uint8_t amplitude = 255;
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


  getSample();                                                // Sample the microphone.
  agcAvg();                                                   // Calculate the adjusted value as sampleAvg.

  FastLED.clear();
  for (int i=0; i<ARRAY_SIZE(outer_ring); i++) {
    if (is_sender) {
      leds[outer_ring[i]] = CHSV(HUE_BLUE, 100, sampleAgc);    // TODO - validate
    } else {
      leds[outer_ring[i]] = CHSV(HUE_RED, 100, sampleAgc);    // TODO - validate
    }
  }

  // TODO -- need to figure out how to either keep calling this or ... 
  // gCurrentPatternNumber = STATIC_PATTERN; 

  if (!is_sender)  { // no forget the baby and make it sound activated
    for (int i=0; i<ARRAY_SIZE(little_heart); i++) {
      leds[little_heart[i]] = CHSV(HUE_RED, 200, sampleAgc);    // TODO - validate
    }
  }

  FastLED.show();

}

void nervous() {
  nervous(false);
}

void sound_show() {
  blending_overlay();
  getSample();                                                // Sample the microphone.
  agcAvg();                                                   // Calculate the adjusted value as sampleAvg.
  if (samplePeak == 1) { leds[0] = CRGB::Gray; samplePeak = 0;}
  leds[(millis() % (NUMPIXELS-1)) +1] = ColorFromPalette(currentPalette, sampleAgc, sampleAgc, currentBlending);
} 

void sound_overlay() {
  // TODO
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
                                {outside_sound, "Outside Sound"},
                                {flyingcircular, "Flying Circular"},
                                {sound_sample, "Sound Sample"},
                                {nervous, "Nervous"},
                                {sound_wave_color, "Sound Wave Based on Color"},
                                {flyingcircular, "Flying Circular"},
                                {bpm_rings, "BMP with Rings"},
                                {fire, "Fire"},
                                {sindots, "Sin Dot"},
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
                                {bpm, "BPM"},
                                {draw_little_heart, "Little Heart"},
                                {sinusoid, "SinusOID"},
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

      EVERY_N_MILLIS_I( thistimer, 50 ) { // initial period = 50ms
        thistimer.setPeriod(GHUE_SPEED);
        gHue = (gHue+1) % 255; 
      }
    }
    if (gOverlayPattern != 255) {
      gOverlay[gOverlayPattern].functPtr();   // call the helper script
    }
  }
}
