#include "ledstrip.h"

#ifndef _systemmonitor_h
#define _systemmonitor_h

#define NUMCPUs 12
#define CPUWARN 50
#define CPUCRIT 70

int grid[NUMCPUs + 1];
int cpuData[NUMCPUs];
int ledsPerCPU = 0;

void initMonitor() {
  ledsPerCPU = NUMLEDS / NUMCPUs;
  if (NUMLEDS % NUMCPUs == 0) ledsPerCPU--;

  int ledsOFF = ( NUMLEDS - ( (ledsPerCPU * NUMCPUs) + 1 ) );

  int j = 0;

  for ( int i = (ledsOFF / 2); i < NUMLEDS; i+=ledsPerCPU) {
    grid[j] = i;
    j++;
    if (j > NUMCPUs) continue;
  }
}

void WriteDelimiters() {
  for ( int i = 0; i < NUMLEDS; i++) {
    leds[i] = CRGB::Black;
  }

  for ( int i = 0; i < NUMCPUs + 1; i++) {
    leds[grid[i]] = CRGB::Gray;
  }
}

void WriteCPUValues() {
  CRGB color = CRGB::Black;

  for ( int i = 0; i < NUMCPUs; i++) {
    int cpuLoad = cpuData[i];
    if (cpuLoad <= CPUWARN) color = CRGB::Green;
    if (cpuLoad > CPUWARN && cpuLoad <= CPUCRIT) color = CRGB::Yellow;
    if (cpuLoad > CPUCRIT) color = CRGB::Red;

    int cpuLed = int(map(cpuData[i],0,100,0,(ledsPerCPU - 1)));


    for ( int j = 0; j < cpuLed; j++) {
      leds[grid[NUMCPUs - i] - 1 - j] = color;
    }
    for ( int j = cpuLed; j < ledsPerCPU - 1; j++) {
      leds[grid[NUMCPUs - i] - 1 - j] = CRGB::Black;
    }
  }
}

#endif