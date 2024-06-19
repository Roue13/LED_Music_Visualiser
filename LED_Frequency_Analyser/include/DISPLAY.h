#ifndef DISPLAY_H
#define DISPLAY_H

#include <Adafruit_GFX.h>
#include <FastLED.h>
#include <Wire.h>
#include <SPI.h>
#include <FastLED_NeoMatrix.h>

#define NUM_BANDS 10
#define NUM_LEDS_PER_BAND 12
#define DATA_PIN 23

#define AVERAGE_FACTOR 85 // Between 0 (soft) and 100 (reactive) --> 70 juste milieu
#define AMPLITUDE 9000    // Bigger = More attenuation
#define BRIGHTNESS 200
#define MAX_CURRENT_MA 2000
#define PEAK_DECAY_SPEED 100

CRGB leds[NUM_BANDS * NUM_LEDS_PER_BAND];

// For 10 bands
int oldBandValues[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int bandValues[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int bandPeaks[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

FastLED_NeoMatrix *matrix = new FastLED_NeoMatrix(leds, NUM_BANDS, NUM_LEDS_PER_BAND,
                                                  NEO_MATRIX_TOP + NEO_MATRIX_LEFT + // Had to reverse TOP and BOTTOM ???
                                                      NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG);

void initDisplay()
{
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_BANDS * NUM_LEDS_PER_BAND); // GRB ordering is typical
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

    // Averaging
    barHeight = ((oldBandValues[band] * (100 - AVERAGE_FACTOR)) + (barHeight * AVERAGE_FACTOR)) / 100;

    oldBandValues[band] = barHeight;
    bandValues[band] = barHeight;

    // Process Bands Peak
    if (bandValues[band] > bandPeaks[band])
    {
      bandPeaks[band] = min((const int)bandValues[band], NUM_LEDS_PER_BAND);
    }
  }
  Serial.println("   ");
}

void drawBandsHeights()
{
  for (int16_t x = 0; x < NUM_BANDS; x++)
  {
    for (int16_t y = bandValues[x]; y > 0; y--)
    {
      matrix->drawPixel(x, y, CRGB(250)); // Change color here
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

#endif /*   DISPLAY_H   */