#ifndef LEDDISPLAY_H
#define LEDDISPLAY_H

#include <Adafruit_GFX.h>
#include <FastLED.h>
#include <FastLED_NeoMatrix.h>
#include <SPI.h>
#include <Wire.h>

CRGB leds[NUM_BANDS * NUM_LEDS_PER_BAND];

int oldBandValues[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int bandValues[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int bandPeaks[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

FastLED_NeoMatrix *matrix = new FastLED_NeoMatrix(leds, NUM_BANDS, NUM_LEDS_PER_BAND,
                                                  NEO_MATRIX_TOP + NEO_MATRIX_LEFT + // Had to reverse TOP and BOTTOM ???
                                                      NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG);

void initDisplay()
{
  FastLED.addLeds<WS2812B, LED_DATA_PIN, GRB>(leds, NUM_BANDS * NUM_LEDS_PER_BAND); // GRB ordering is typical
  FastLED.setMaxPowerInVoltsAndMilliamps(5, MAX_CURRENT_MA);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();

  FastLED.show();
}

void processBands()
{
  for (uint8_t band = 0; band < NUM_BANDS; band++)
  {
    // Process Bands Height
    int barHeight = bandValues[band] / AMPLITUDE;
    if (barHeight > NUM_LEDS_PER_BAND)
    {
      barHeight = NUM_LEDS_PER_BAND;
    }

    // Averaging / Smoothing Bands Heights
    barHeight = ((oldBandValues[band] * (100 - AVERAGE_FACTOR)) +
                 (barHeight * AVERAGE_FACTOR)) /
                100;

    // Assign New / Old values
    oldBandValues[band] = barHeight;
    bandValues[band] = barHeight;

    // Process Bands Peak
    if (bandValues[band] > bandPeaks[band])
    {
      bandPeaks[band] = min((const int)bandValues[band], NUM_LEDS_PER_BAND);
    }
  }
}

void drawBandsHeights()
{
  for (int8_t x = 0; x < NUM_BANDS; x++)
  {
    for (int y = 0; y <= bandValues[x]; y++)
    {
      matrix->drawPixel(x, y, ColorFromPalette(pinkRedPal, constrain(bandValues[y] * (255 / NUM_LEDS_PER_BAND), 0, 255))); // Change color here
    }
  }
}

void drawPeaks()
{
  for (int8_t band = 0; band < NUM_BANDS; band++)
  {
    matrix->drawPixel(band, bandPeaks[band], CHSV(0, 255, 255)); // White Peaks
  }
}

void peaksDecay()
{
  for (uint8_t band; band < NUM_BANDS; band++)
  {
    if (bandPeaks[band] > 0)
      bandPeaks[band] -= 1;
  }
}

#endif /*   LEDDISPLAY_H   */