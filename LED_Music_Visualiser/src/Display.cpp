#include "Display.h"
#include "AudioData.h"

/* LED and MATRIX objects */
CRGB leds[NUM_BANDS * NUM_LEDS_PER_BAND];

FastLED_NeoMatrix *matrix = new FastLED_NeoMatrix(leds, NUM_BANDS, NUM_LEDS_PER_BAND,
                                                  NEO_MATRIX_TOP + NEO_MATRIX_LEFT + // Had to reverse TOP and BOTTOM ???
                                                      NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG);

/* Color palettes */
DEFINE_GRADIENT_PALETTE(pink_red_gp){
    0, 255, 182, 193, // Pink
    255, 255, 0, 0    // Red
};
CRGBPalette16 pinkRedPal = pink_red_gp;

DEFINE_GRADIENT_PALETTE(blue_gp){
    0, 0, 0, 255,  // Cyan
    255, 0, 0, 100 // Dark Blue
};
CRGBPalette16 BluePal = blue_gp;

DEFINE_GRADIENT_PALETTE(bgr_gp){
    0, 0, 0, 255,  // Blue
    80, 0, 255, 0, // Green
    220, 255, 0, 0 // Red
};
CRGBPalette16 BGRPal = bgr_gp;

DEFINE_GRADIENT_PALETTE(lightYellow_Green_gp){
    0, 255, 250, 205, // Light Yellow
    255, 0, 210, 0    // Dark Green
};
CRGBPalette16 yellowGreenPal = lightYellow_Green_gp;

void initDisplay()
{
    FastLED.addLeds<LED_STRIP_TYPE, LED_DATA_PIN, GRB>(leds, NUM_BANDS * NUM_LEDS_PER_BAND); // GRB ordering is typical
    FastLED.setMaxPowerInVoltsAndMilliamps(LED_STRIP_VOLTAGE, LED_STRIP_MAX_CURRENT);
    FastLED.setBrightness(BRIGHTNESS);
    FastLED.clear();
    fill_solid(leds, TOTAL_LEDS, CRGB::Black);
    FastLED.show();
}

void processBandsRaw()
{
    for (uint8_t band = 0; band < NUM_BANDS; band++)
    {
        // Process Bands Height
        int barHeight = bandValues[band] / AMPLITUDE_RAW;
        if (barHeight > NUM_LEDS_PER_BAND)
        {
            barHeight = NUM_LEDS_PER_BAND;
        }

        // Averaging / Smoothing Bands Heights
        barHeight = ((oldBandValues[band] * (100 - AVERAGE_FACTOR)) + (barHeight * AVERAGE_FACTOR)) / 100;

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

void drawBandsHeightsRaw()
{
    for (int8_t x = 0; x < NUM_BANDS; x++)
    {
        for (int y = 0; y <= bandValues[x]; y++)
        {
            matrix->drawPixel(x, y, ColorFromPalette(BGRPal, constrain(y * (255 / NUM_LEDS_PER_BAND), 0, 255))); // Change color here
        }
    }
}

void processBandsFull()
{
    int maxMeasureValue = 255 * NUM_LEDS_PER_BAND;

    for (uint8_t band = 0; band < NUM_BANDS; band++)
    {
        // Process Bands Height
        int barHeight = bandValues[band] / AMPLITUDE_TRAILING;
        if (barHeight > maxMeasureValue)
        {
            barHeight = maxMeasureValue;
        }

        barHeight = ((oldBandValues[band] * (100 - AVERAGE_FACTOR)) + (barHeight * AVERAGE_FACTOR)) / 100;

        oldBandValues[band] = barHeight;
        bandValues[band] = barHeight;
    }
}

void drawBandsHeightsT1()
{
    for (int8_t band = 0; band < NUM_BANDS; band++)
    {
        // 3 variables for position of the leds (between 0 and NUM_LEDS_PER_BAND)
        uint8_t lastLed = min(((bandValues[band] - (bandValues[band]) % 255) / 255), NUM_LEDS_PER_BAND);

        for (int led = 0; led < NUM_LEDS_PER_BAND; led++)
        {
            uint8_t color = min((led * 255) / NUM_LEDS_PER_BAND, 255);
            int lumLastLed = (bandValues[band] % 255) * (float)(35.0 / 100.0);

            // Actualise led values
            if (led < lastLed)
            {
                matrix->drawPixel(band, led, ColorFromPalette(BGRPal, color, 255)); // Full RED
            }
            else if (led == lastLed)
            {
                matrix->drawPixel(band, led, ColorFromPalette(BGRPal, color, lumLastLed)); // Change color here
            }
            else
            {
                matrix->drawPixel(band, led, CHSV(0, 0, 0));
            }
        }
    }
}

void drawBandsHeightsT3()
{
    for (int8_t band = 0; band < NUM_BANDS; band++)
    {
        // 3 variables for position of the leds (between 0 and NUM_LEDS_PER_BAND)
        uint8_t lastLed = min(((bandValues[band] - (bandValues[band]) % 255) / 255), NUM_LEDS_PER_BAND - 1);
        uint8_t lastLedBefore = max(lastLed - 1, 0);
        uint8_t lastLedAfter = lastLed + 1;

        for (int led = 0; led < NUM_LEDS_PER_BAND; led++)
        {
            uint8_t color = min((led * 255) / NUM_LEDS_PER_BAND, 255);

            // Variables for intensity of the leds (between 0 and 255)
            int lumToShare = 255 + (bandValues[band] % 255); // One full led (lastLedBefore) + the rest

            /*int lumLastLedBefore = lumToShare * (float)((float)T3_FACTOR_1ST / 100.0);
            int lumLastLed = lumToShare * (float)((float)T3_FACTOR_2ND / 100.0);
            int lumLastLedAfter = lumToShare * (float)((float)T3_FACTOR_3RD / 100.0);*/

            int lumLastLedBefore = lumToShare * (float)(60.0 / 100.0);
            int lumLastLed = lumToShare * (float)(25.0 / 100.0);
            int lumLastLedAfter = lumToShare * (float)(15.0 / 100.0);

            // Actualise led values
            if (led < lastLedBefore)
            {
                matrix->drawPixel(band, led, ColorFromPalette(BGRPal, color, 255)); // Full RED
            }
            else if (led == lastLedBefore)
            {
                matrix->drawPixel(band, led, ColorFromPalette(BGRPal, color, lumLastLedBefore)); // Change color here
            }
            else if (led == lastLed)
            {
                matrix->drawPixel(band, led, ColorFromPalette(BGRPal, color, lumLastLed)); // Change color here
            }
            else if (led == lastLedAfter)
            {
                matrix->drawPixel(band, led, ColorFromPalette(BGRPal, color, lumLastLedAfter)); // Change color here
            }
            else
            {
                matrix->drawPixel(band, led, CHSV(0, 0, 0));
            }
        }
    }
}

void drawPeaks()
{
    for (int8_t band = 0; band < NUM_BANDS; band++)
    {
        matrix->drawPixel(band, bandPeaks[band], CHSV(0, 0, 255)); // White Peaks
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