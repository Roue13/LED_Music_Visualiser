#ifndef DISPLAY_H
#define DISPLAY_H

/***** Files and Libraries *****/
#include "Settings.h"
#include <Adafruit_GFX.h>
#include <FastLED.h>
#include <FastLED_NeoMatrix.h>
#include <SPI.h>
#include <Wire.h>

/*  Initialise led strip parameters and characteristics  */
void initDisplay(void);

/* %%%%%%%%%%%%%%%%%%%% RAW CALCULATION / DISPLAY %%%%%%%%%%%%%%%%%%%% */

/*  Calculations between sampling and display.
    Outpout led values are 0 or 255 (OFF - ON)  */
void processBandsRaw(void);

/*  Display raw calculated led values.
    Each led can be ON or OFF without smoothing or transitions  */
void drawBandsHeightsRaw();

/* %%%%%%%%%%%%%%%%%%%% Full CALCULATION / DISPLAY %%%%%%%%%%%%%%%%%%%% */

/*  Calculations between sampling and display.
    Outpout led values are between 0 and 255 (smoother display) */
void processBandsFull(void);

/*  Display full calculated led values.
    Trailing 1 --> The higher led of each band smoothly fade out */
void drawBandsHeightsT1();

/*  Display full calculated led values.
    Trailing 3 --> The 3 higher leds of each band smoothly fade out */
void drawBandsHeightsT3();

/* %%%%%%%%%%%%%%%%%%%% Full CALCULATION / DISPLAY %%%%%%%%%%%%%%%%%%%% */
void drawPeaks(void);
void peaksDecay(void);

#endif /* DISPLAY_H */