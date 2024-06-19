#ifndef DISPLAY_H
#define DISPLAY_H

#include <Adafruit_GFX.h>
#include <FastLED.h>
#include <Wire.h>
#include <SPI.h>
#include <FastLED_NeoMatrix.h>

#define NUM_BANDS 8
#define NUM_LEDS_PER_BAND 12
#define DATA_PIN 23

#define AMPLITUDE 1000
#define BRIGHTNESS 200
#define MAX_CURRENT 2000 // mA
#define PEAK_DECAY_SPEED 50

CRGB leds[NUM_BANDS * NUM_LEDS_PER_BAND];

// For 8 bands
uint8_t oldBandValues[] = {0, 0, 0, 0, 0, 0, 0, 0};
uint8_t bandValues[] = {0, 0, 0, 0, 0, 0, 0, 0};
uint8_t bandPeaks[] = {0, 0, 0, 0, 0, 0, 0, 0};

int bandFreqLabels[] = {125, 250, 500, 1000, 2000, 4000, 8000, 16000};

FastLED_NeoMatrix *matrix = new FastLED_NeoMatrix(leds, NUM_BANDS, NUM_LEDS_PER_BAND,
                                                  NEO_MATRIX_BOTTOM + NEO_MATRIX_LEFT +
                                                      NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG);

void initDisplay()
{
  FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_BANDS * NUM_LEDS_PER_BAND); // GRB ordering is typical
  FastLED.setMaxPowerInVoltsAndMilliamps(5, MAX_CURRENT);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();

  FastLED.show();
}

void processBands()
{
  for (uint8_t band = 0; band < NUM_BANDS; band++)
  {
    // Process Bands Height
    uint8_t barHeight = bandValues[band] / AMPLITUDE;
    if (barHeight > NUM_LEDS_PER_BAND)
    {
      bandValues[band] = NUM_LEDS_PER_BAND;
    }
    // barHeight = ((oldBandValues[band] * 1) + bandValues) / 2;     // Small amount of averaging between frames
    oldBandValues[band] = bandValues[band];

    // Process Bands Peak
    if (bandValues[band] > bandPeaks[band])
    {
      bandPeaks[band] = min((const int)bandValues[band], NUM_LEDS_PER_BAND);
    }
  }
}

void drawBandsHeights()
{
  for (int16_t x = 0; x < NUM_BANDS; x++)
  {
    for (int16_t y = 0; y < NUM_LEDS_PER_BAND - bandValues[x]; y++)
    {
      matrix->drawPixel(x, y, CRGB(250)); // Change color here
    }
  }
}

void drawPeaks()
{
  for (int16_t x = 0; x < NUM_BANDS; x++)
  {
    matrix->drawPixel(x, NUM_LEDS_PER_BAND, CHSV(0, 0, 255)); // White Peaks
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

#endif /*   DISPLAY_H   */