#ifndef DISPLAY_H
#define DISPLAY_H

#include "Settings.h"
#include <Adafruit_GFX.h>
#define FASTLED_INTERNAL
#include <FastLED.h>
#include <FastLED_NeoMatrix.h>
#include <SPI.h>
#include <Wire.h>

/* Functions */
void initDisplay(void);

void processBands(void);

void drawBandsHeights(uint8_t);

void drawPeaks(void);

void peaksDecay(void);

#endif /* DISPLAY_H */