#define FASTLED_INTERNAL
#include <FastLED.h>

#ifndef _ledstrip_h
#define _ledstrip_h

#define STRIPTYPE APA102
#define DATAPIN 14
#define CLOCKPIN 13
#define COLORSCHEME BGR
#define NUMLEDS 193

#define BRIGHTNESS  64
#define FPS 100

CRGB leds[NUMLEDS];

void initLed() {
  pinMode(CLOCKPIN, OUTPUT);
  pinMode(DATAPIN, OUTPUT);

  FastLED.addLeds<STRIPTYPE, DATAPIN, CLOCKPIN, COLORSCHEME>(leds, NUMLEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
}

#endif
