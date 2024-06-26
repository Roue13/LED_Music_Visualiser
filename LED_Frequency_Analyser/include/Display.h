#ifndef DISPLAY_H
#define DISPLAY_H

#include "Settings.h"
#include <Adafruit_GFX.h>
#include <FastLED.h>
#include <FastLED_NeoMatrix.h>
#include <SPI.h>
#include <Wire.h>

/* Functions */
void initDisplay(void);

void processBands(void);

void processBandsV2(void);

void drawBandsHeights(uint8_t);

void drawBandsHeightsV2(uint8_t);

void drawPeaks(void);

void peaksDecay(void);

#endif /* DISPLAY_H */