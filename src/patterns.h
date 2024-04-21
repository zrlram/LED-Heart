#pragma once

// more: https://www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects/#LEDStripEffectBouncingBalls
// https://github.com/FastLED/FastLED/tree/master/examples


// TODO - Use the peak detector to advance the gHue? Or some variable that can be used as an overlay to shows

#include <heart.h>
#include <sound.h>
#include <FastLED.h>
#include "arduino-timer.h"
#include <averagesContainer.h>
#include <fscale.h>

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

static uint16_t GHUE_SPEED = 50;       // ms to increase gHue   0 - 1000 in 50 steps

static uint8_t gHue = 0;
CRGB color = CRGB::Red; 

auto timer = timer_create_default();
static bool no_updates = false;          // When we do not want to call update_pattern
static bool show_initialized = false;     // some shows need to init themselves when they run again (like filling all LEDs with a color)

CRGB leds[NUMPIXELS+1];                 // we are using the overflow pixel for error handling (see XY(i,j))

// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePattern)();

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gOverlayPattern = 255;      // 255 is off

// Palette definitions
CRGBPalette16 currentPalette = PartyColors_p;
// CRGBPalette16 currentPalette = OceanColors_p;
CRGBPalette16 targetPalette = OceanColors_p;
TBlendType    currentBlending = LINEARBLEND;                  // NOBLEND or LINEARBLEND
CHSV distinct_colors[] = {
  CHSV(0, 255, 255), // Red
  CHSV(120, 255, 255), // Green
  CHSV(240, 255, 255), // Blue
  CHSV(30, 255, 255), // Purple
  CHSV(60, 255, 255), // Orange
  CHSV(90, 255, 255), // Yellow
};


bool reset_no_update(void *) {
  no_updates = false;
  return false; // no repeat of this timer
}

void indicator(uint16_t value, uint16_t maximum, CRGB c = CRGB::Blue) {
  timer.cancel();
  timer.in(600, reset_no_update); 
  no_updates = true;

  // 10 steps
  FastLED.clear();
  for (uint16_t i=0; i<10.0/maximum*value; i++) {
    leds[i+23] = c;
  }
  FastLED.show();
}

void increase_ghue_speed(bool show_indicator = true) {
  if (GHUE_SPEED <= 950)
    GHUE_SPEED += 50;
  if (show_indicator) 
    indicator(GHUE_SPEED, 1000, CRGB::Pink);
}

void decrease_ghue_speed(bool show_indicator = true) {
  if (GHUE_SPEED >= 50)
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
    no_updates = true;
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

// Helper Function
void sndwave() {
  // https://github.com/atuline/FastLED-SoundReactive/blob/master/sound_wave/sound_wave.ino

  // try to do it by rings ...  

  // TODO -- do we just only call this when there is a certain threshold??
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

  // EVERY_N_SECONDS(5) {                                     // Change the palette every 5 seconds.
  EVERY_N_MILLIS_I(sound_wave_timer, GHUE_SPEED*10) {
      sound_wave_timer.setPeriod(GHUE_SPEED*10);  
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

  // FastLED.show();

} 

/* https://wokwi.com/projects/288948170884383245 */
void matrix() {
  static byte rain[NUMPIXELS];

  if (!show_initialized) {
    // init - run once
    for (int i = 0; i < NUMPIXELS; i++) rain[i] = !random8(15)? 1:0; 
    show_initialized = true;
  }

  EVERY_N_MILLISECONDS(80) {
    static int speed = 10;

    for (byte j = 0; j < HEIGHT; j++) {       // 0..7
      int yindex = (j + speed) % WIDTH * HEIGHT;
      for (byte i = 0; i < WIDTH; i++) {      // 0..10
        if (rain[(yindex+i) % NUMPIXELS]) {
          // Serial.printf("%d, %d, %d\n", i, j, XY(i, j));
          //Serial.printf("%d, %d, %d\n", i, j, led);
          //Serial.println();
          // error handling is happening by returning ERROR_LED from XY and that's mapped in LED as an extra LED
          leds[XY((WIDTH - 1) - i, (HEIGHT - 1) - j)].setHue (100);
        }
      }
    }

    fadeToBlackBy(leds, NUMPIXELS, 30);
    speed ++;
  }

  EVERY_N_MILLISECONDS(15) {
    int rand1 = random16 (NUMPIXELS);
    int rand2 = random16 (NUMPIXELS);
    if (rain[rand1] && !rain[rand2]) {rain[rand1] = 0; rain[rand2] = 1;}
  }

  // FastLED.show();

} 

/* https://wokwi.com/projects/287764949564916236 */
const uint8_t kBorderWidth = 1;

void kriegsman_swirl()
{
  static uint8_t counter = 0 ;
  // Apply some blurring to whatever's already on the matrix
  // Note that we never actually clear the matrix, we just constantly
  // blur it repeatedly.  Since the blurring is 'lossy', there's
  // an automatic trend toward black -- by design.
  if( counter % 3 == 0 ) {
    uint8_t blurAmount = beatsin8(2, 100, 150);
    blur2d( leds, WIDTH, HEIGHT, blurAmount);
    // Serial.println(blurAmount);
  }
  counter++;

  // Use two out-of-sync sine waves
  uint8_t  i = beatsin8( 27, kBorderWidth, HEIGHT - kBorderWidth);
  uint8_t  j = beatsin8( 41, kBorderWidth, WIDTH - kBorderWidth);

  // Also calculate some reflections
  uint8_t ni = (WIDTH - 1) - i;
  uint8_t nj = (WIDTH - 1) - j;

  // The color of each point shifts over time, each at a different speed.
  uint16_t ms = millis();
  leds[XY( i, j)] += CHSV( ms / 11, 255, 255);
  leds[XY( j, i)] += CHSV( ms / 13, 255, 255);
  leds[XY(ni, nj)] += CHSV( ms / 17, 255, 255);
  leds[XY(nj, ni)] += CHSV( ms / 29, 255, 255);
  leds[XY( i, nj)] += CHSV( ms / 37, 255, 255);
  leds[XY(ni, j)] += CHSV( ms / 41, 255, 255);

  // FastLED.show();
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
  COOLING = GHUE_SPEED;    

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
  // FastLED.show();

}

void runningOutside() {
    FastLED.clear();
    leds[outer_ring_chaser[gHue % ARRAY_SIZE(outer_ring_chaser)]] = CRGB::White;
    // FastLED.show();
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
    // FastLED.show();
  }
}

void outside() {
    FastLED.clear();
    for (int i=0; i<ARRAY_SIZE(outer_ring); i++) {
      leds[outer_ring[i]] = color;
    }
    // FastLED.show();
}

void outside(CRGB color) {
  setColor(color);
  outside();
}

void sound_sample()  {
  // basically random dots sound animated 
  //  agcAvg_Pal * By: Andrew Tuline

  EVERY_N_MILLIS_I(sound_sample_timer, GHUE_SPEED*10) {
    sound_sample_timer.setPeriod(GHUE_SPEED*10);  
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
  
  // if (samplePeak == 1) { leds[0] = CRGB::Gray; samplePeak = 0;}
  leds[(millis() % (NUMPIXELS-1)) +1] = ColorFromPalette(currentPalette, sampleAgc, sampleAgc, currentBlending);
 
  // FastLED.show();

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

//  https://wokwi.com/projects/288441747546046984
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
  
  // FastLED.show();

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

  // FastLED.show();
  
}

// https://wokwi.com/projects/289623420597961224
// TODO - manipulated the code below quite a bit to use more built-in functions. Does it still look ok?
    //    test out on the wokwi first?!

#define RASTER_SPACING 2 // TODO - can we just set to 1?

CRGBPalette16 fadeFactorPalette = {
  0xAA0000, 0xAA0000, 0xAA0000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
  0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000
};

void qaddColors(CRGB *a, const CRGB *b) {
  a->red   = qadd8(a->red,b->red);
  a->green = qadd8(a->green,b->green);
  a->blue  = qadd8(a->blue,b->blue);
}

DEFINE_GRADIENT_PALETTE( Andy_GIlmore_2_gp ) {
    0,  12,  0,  1,
   36,  12,  0,  1,
   73,  39,  3,  3,
   98,  84, 13, 20,
  122, 153, 91, 98,
  128, 252,252,255,
  135, 173,187,205,
  158, 112,135,162,
  181,  61, 63, 92,
  218,  29, 21, 43,
  255,  29, 21, 43};

void sup(uint8_t rotationSpeed, uint8_t translationSpeed, uint8_t lineWidth, CRGBPalette16 palette, TBlendType blendType, boolean foreground, boolean enableCrossfade) {
  uint32_t yHueDelta ;
  uint32_t xHueDelta ;
  static uint32_t lastMillis = 16383; // int16_t 32767/2  (for rotationspeed=0 test)
  float rotationSpeedFloat = fmap((float)rotationSpeed,(float)0,(float)255,-6.0,6.0); // Between -6 and 6
  int16_t mappedTranslationSpeed = map(translationSpeed,0,255,-2<<12,2<<12); // Between -2^13 - 2^13 (higher is too fast)

  uint32_t ms = millis();

  if( rotationSpeedFloat != 0 ) {
    yHueDelta = (int32_t)sin16((int16_t)round(ms * rotationSpeedFloat)) * lineWidth;
    xHueDelta = (int32_t)cos16((int16_t)round(ms * rotationSpeedFloat)) * lineWidth;
    lastMillis = ms;
  } else {
    yHueDelta = (int32_t)sin16(lastMillis) * lineWidth;
    xHueDelta = (int32_t)cos16(lastMillis) * lineWidth;
  }
  int32_t startHue = ms * mappedTranslationSpeed;
  int32_t lineStartHue = startHue - (HEIGHT + 2) / 2 * yHueDelta;
  int16_t yd2 = sin16(ms * 3) / 4;
  int16_t xd2 = sin16(ms * 7) / 4;
  for (byte y = 0; y < HEIGHT; y++) {
    uint32_t pixelHue = lineStartHue - (WIDTH + 2) / 2 * xHueDelta;
    uint32_t xhd = xHueDelta;
    lineStartHue += yHueDelta;
    for (byte x = 0; x < WIDTH; x++) {
      if( y % RASTER_SPACING == 0 ) {
        int ledsY = y/RASTER_SPACING;
        if( foreground ) {
          if( enableCrossfade ) {
            CRGB fadeFactor = ColorFromPalette(fadeFactorPalette, pixelHue >> 7, 255, LINEARBLEND);
            uint8_t this_fade = 255 - fadeFactor.red;
            blend(leds[XY(x, ledsY)], palette[0], this_fade);
          } else {
            CRGB color2 = ColorFromPalette(palette, pixelHue >> 7, 255, LINEARBLEND);
            qaddColors(&leds[XY(x, ledsY)], &color2);
          }
        } else {
          leds[XY(x, ledsY)] = ColorFromPalette(palette, pixelHue >> 7, 255, blendType);
        }      
      }
      pixelHue += xHueDelta;      
    }
  }
}

void spendida_crossfader() {
    // sup(95,110,3,Andy_GIlmore_2_gp, LINEARBLEND, ANIM_BACKGROUND, DISABLE_CROSSFADE);
    sup(95,110,3, Andy_GIlmore_2_gp, LINEARBLEND, false, false);
}

/* vu_meter ...
    https://edg.berkeley.edu/wp-content/uploads/2020/12/2020Fall_ME102B_delainejonathan.pdf
*/

#define s_MIC_HIGH (600)
#define s_MIC_LOW  (0)
#define MIC_OFFSET (1923)   //From MATLAB processing
#define BUFFER_DEV (400)

struct averagesContainer *samples;
struct averagesContainer *longsamples;
struct averagesContainer *buffersamples;

float hue = 0;
float hueInc = 0.7;
int hueOffset = 120;
float fadeSc = 1.3;

//Initialize class objects
#define NUMSAMPLES (20)
#define NUMLONGSAMPLES (250)
#define BUFFER_SIZE (3)

void vu_meter() { //Sound Reactive Vu Meter

  Serial.println("start");
  if (samples == NULL) {
    samples = new averagesContainer(NUMSAMPLES);
    longsamples = new averagesContainer(NUMLONGSAMPLES);
    buffersamples = new averagesContainer(BUFFER_SIZE);
    while (buffersamples->setSample(250) == true) {}
    while (longsamples->setSample(200) == true) {}
  }

  int condraw = abs (analogRead(MIC_PIN) - MIC_OFFSET);   //Centers reading around zero, removes negative range
  Serial.println(condraw);

  // Attempt to use static calibration instead of dynamic
  int bufferval = buffersamples->findAverage();
  if (!(abs(condraw - bufferval) > BUFFER_DEV)) {
    buffersamples->setSample(condraw);
  }

  // Scale conditioned signal to Log Scale with .4 scalar
  condraw = fscale(s_MIC_LOW, s_MIC_HIGH, s_MIC_LOW, s_MIC_HIGH, condraw, 0.4);

  if (samples->setSample(condraw))
    return; //continue adding until full

  uint16_t longsamplesAvg = longsamples->findAverage();
  uint16_t inputVal = samples->findAverage();

  longsamples->setSample(inputVal);

  // Change hue of colors based on long term averages -- TODO - see how this looks!?
  int diff = (inputVal - longsamplesAvg);
  if (diff > 5) {
    if (hue < 235) {
      hue += hueInc;
    }
  } else if (diff < -5) {
    if (hue > 2) {
      hue -= hueInc;
    }
  } 

  // TODO - is my math here right to fill up to HEIGHT?
  int height = fscale(s_MIC_LOW, s_MIC_HIGH, 0.0, (float)HEIGHT, (float)inputVal, 0);
  Serial.print("height: ");
  Serial.println(height);

  int end_pixel = 0;
  for (int row=HEIGHT-1; row>HEIGHT-1-height; row--) {
    end_pixel += ROWS[row];
  }
  Serial.print("end pixel: ");
  Serial.println(end_pixel);
  for (int i=NUMPIXELS; i > end_pixel; i--) {
      leds[i] = CHSV(gHue + hueOffset + (i * 2), 255, FastLED.getBrightness());       // TODO last argument was max_bright
      Serial.println(i);
  }

  for (int i=end_pixel; i > 0; i--) {
      leds[i] = CRGB(leds[i].r / fadeSc, leds[i].g / fadeSc, leds[i].b / fadeSc);
      Serial.println(i);
  }

}

#define REDZONE (5)

void sound_equalizer() {
   static byte matrix[WIDTH] = { 0 };
   unsigned static int signalMax = 0;
   unsigned static int signalMin = 20;


   // map 1v p-p level to the max scale of the display
   EVERY_N_MILLIS(100) {
    getSample();
    agcAvg();
    // do auto-scale
    if (sampleAgc > signalMax) {
      signalMax = sampleAgc;  // save just the max levels
    } else if (sampleAgc < signalMin) {
      signalMin = sampleAgc;  // save just the min levels
    }

    int displayPeak = map(sampleAgc, 0, 1023, 0, signalMax);
    // Serial.println(displayPeak);

    // Update the display:
    for (int i = 0; i < WIDTH-1; i++)  // shift the display left
    {
      matrix[i] = matrix[i+1];
    }

    matrix[WIDTH-1] = displayPeak;   // add new sample

    // draw the new sample
    for (int i=0; i < WIDTH; i++) {
        for (int j=0; j < HEIGHT; j++) {
          if (j >= matrix[i]) {   // black
            leds[XY(i,HEIGHT-j-1)] = CRGB::Black;
          } else if (j < REDZONE) { 
            leds[XY(i,HEIGHT-j-1)] = CRGB::LightGreen;
          } else {
            leds[XY(i, HEIGHT-j-1)] = CRGB::Purple;
          }
        }
    }
   }
}

// to validate the circleMatrix and its data definition - also useful to test all LEDs
void test_circle() {

  for (int step=0; step<NUMELEMENTS(circleMatrix); step++) 
    for (int i=0; i<circleMatrix[step].numElements; i++)
      leds[circleMatrix[step].circles[i]] = distinct_colors[step];

}

// https://github.com/atuline/FastLED-SoundReactive/blob/master/sound_ripple/sound_ripple.ino
int step = -1;                                                // -1 is the initializing step.
uint8_t colour;                                               // Ripple colour is randomized.
uint8_t myfade = 255;                                         // Starting brightness.
#define maxsteps NUMELEMENTS(circleMatrix)                                           // Case statement wouldn't allow a variable.

void sound_ripple() {

  getSample();

  if (samplePeak == 1) {step = -1; samplePeak = 0; }          // If we have a peak, let's reset our ripple.

  fadeToBlackBy(leds, NUMPIXELS, 64);                          // 8 bit, 1 = slow, 255 = fast
  
  switch (step) {

    case -1:                                                  // Initialize ripple variables.
      colour = random8();                                     // More peaks/s = higher the hue colour.
      step = 0;
      break;

    case 0:
      for (int i=0; i<NUMELEMENTS(circle_1); i++) {
        leds[circle_1[i]] = CHSV(colour, 255, 255);                     // Display the first pixel of the ripple in the center
      }
      step ++;
      break;

    case maxsteps:                                            // At the end of the ripples.
      // step = -1;
      break;

    default:                                                  // Middle of the ripples.
      //leds[(center + step + NUM_LEDS) % NUM_LEDS] += CHSV(colour, 255, myfade/step*2);       // Simple wrap.
      //leds[(center - step + NUM_LEDS) % NUM_LEDS] += CHSV(colour, 255, myfade/step*2);
      for (int i=0; i<circleMatrix[step].numElements; i++)
        leds[circleMatrix[step].circles[i]] += CHSV(colour, 255, myfade/step*2);       
        EVERY_N_MILLIS_I(sound_ripple_timer, GHUE_SPEED) {
          sound_ripple_timer.setPeriod(GHUE_SPEED);  
          step ++;                                            // Next step - moving the ring out concentrically one more step
        }
      break;  
  } // switch step
  
} // sound_ripple()

// ---------- END PATTERNS ------------------------------

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

// What happens when flirted with
void nervous(bool is_sender=false) {

  getSample();                                                // Sample the microphone.
  agcAvg();                                                   // Calculate the adjusted value as sampleAvg.

  FastLED.clear();
  for (int i=0; i<ARRAY_SIZE(outer_ring); i++) {
    if (is_sender) {
      leds[outer_ring[i]] = CHSV(HUE_BLUE, 100, sampleAgc);    
    } else {
      leds[outer_ring[i]] = CHSV(HUE_RED, 100, sampleAgc);    
    }
  }

  if (!is_sender)  { // no forget the baby and make it sound activated
    for (int i=0; i<ARRAY_SIZE(little_heart); i++) {
      leds[little_heart[i]] = CHSV(HUE_RED, 200, sampleAgc);    
    }
  }

  // FastLED.show();

}

void nervous() {
  nervous(false);
}

void sound_show() {
  blending_overlay();
  getSample();                                                // Sample the microphone.
  agcAvg();                                                   // Calculate the adjusted value as sampleAvg.
  // if (samplePeak == 1) { leds[0] = CRGB::Gray; samplePeak = 0;}
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
  const bool is_sound;
};

SimplePatternList gPatterns[] = { 
                                {test_circle, "Test Circle Def", 0},      // TODO: tune the peak detection a bit but otherwise okay (it's okay to be empty too, don't overdo it)
                                {sound_ripple, "Sound Ripple", 1},      // TODO: tune the peak detection a bit but otherwise okay (it's okay to be empty too, don't overdo it)
                                {kriegsman_swirl, "Krigesman Swirl", 0},  // super cool 
                                {outside_sound, "Outside Sound", 1},      // works
                                {flyingcircular, "Flying Circular", 0},   // works
                                {sound_sample, "Sound Sample", 1},        // works
                                {sound_wave_color, "Sound Wave Based on Color", 1},     // works
                                // {spendida_crossfader, "Splendida Crossfader", 0}, // TODO - needs lots of help
                                // {vu_meter, "VU Meter", 1},    // TODO - somehow overwrites other memory and sets no_update to 29
                                {matrix, "Matrix", 0},              // works
                                {fire, "Fire", 0},                // works
                                {sindots, "Sin Dot", 0},
                                {sound_show, "Sound Show", 1},
                                {sound_wave, "Sound Wave", 1},
                                {bpm_rings, "BMP with Rings", 0},   // works
                                {outside, "Outside Only", 0},
                                {sinelon, "Sinelon", 0},
                                {runningOutside, "Running Dot Outside", 0},
                                {rainbowCircles, "Rainbow Circles", 0},
                                {confetti, "Confetti", 0},
                                {pride, "Pride", 0},
                                {rainbowLines, "Rainbow Lines", 0},
                                {juggle, "Juggle", 0},
                                {bpm, "BPM", 0},
                                {rainbow, "Rainbow", 0},
                                {sound_equalizer, "Equalizer", 1},      // still needs some work
                                {draw_little_heart, "Little Heart", 0},
                                {sinusoid, "SinusOID", 0},
                                // {nervous, "Nervous", 1},
                                };

SimplePatternList gOverlay[] = { 
                                {blending_overlay, "Blending for sound_show"}
                               };

bool is_sound_show(uint16_t show_index) {
  return gPatterns[show_index].is_sound;
}

void indicate_show(uint8_t show_index) {
  timer.cancel();
  timer.in(400, reset_no_update);          // milliseconds - show just briefly
  no_updates = true;

  // outside steps
  FastLED.clear();
  for (uint8_t i=0; i<=show_index; i++) {
    CRGB c = CRGB::Green;
    if (is_sound_show(i)) c = CRGB::Blue;
    leds[outer_ring[i]] = c; 
  }
  FastLED.show();
}                      

void nextPattern() {
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
  Serial.println(gPatterns[gCurrentPatternNumber].name);
  no_updates = false;
  show_initialized = false;     // we go to a new show and need to make sure the new show gets initialized again

  indicate_show(gCurrentPatternNumber);
}

void previousPattern() {
  // add one to the current pattern number, and wrap around at the end
  if (gCurrentPatternNumber > 0) gCurrentPatternNumber = (gCurrentPatternNumber - 1); 
  else gCurrentPatternNumber = ARRAY_SIZE(gPatterns) - 1;
  Serial.println(gPatterns[gCurrentPatternNumber].name);
  no_updates = false;
  show_initialized = false;     // we go to a new show and need to make sure the new show gets initialized again

  indicate_show(gCurrentPatternNumber);
}

void updatePattern() {
  if (!no_updates) {
    gPatterns[gCurrentPatternNumber].functPtr();   // call the helper script
    // FastLED.setBrightness(BRIGHTNESS);
    FastLED.show();

    EVERY_N_MILLIS_I( thistimer, 50 ) { // initial period = 50ms can be changed via IR remote
      thistimer.setPeriod(GHUE_SPEED);
      gHue = (gHue+1) % 255; 
    }
    if (gOverlayPattern != 255) {
      gOverlay[gOverlayPattern].functPtr();   // call the helper script
    }
  }
}
