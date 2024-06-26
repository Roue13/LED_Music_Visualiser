#include "Display.h"
#include "Setup.h"

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

void processBandsV2()
{
    for (uint8_t band = 0; band < NUM_BANDS; band++)
    {
        // Process Bands Height
        int maxMeasureValue = 255 * NUM_LEDS_PER_BAND;
        int barHeight = bandValues[band] / (AMPLITUDE / maxMeasureValue);
        if (barHeight > maxMeasureValue)
        {
            barHeight = maxMeasureValue;
        }

        bandValues[band] = barHeight;

        /*Serial.print("band : ");
        Serial.print(band + 1);
        Serial.print("  ->  ");
        Serial.println(barHeight);*/
    }
}

void drawBandsHeights(uint8_t mode)
{
    if (mode == 0) // Normal
    {
        for (int8_t x = 0; x < NUM_BANDS; x++)
        {
            for (int y = 0; y <= bandValues[x]; y++)
            {
                matrix->drawPixel(x, y, ColorFromPalette(BGRPal, constrain(y * (255 / NUM_LEDS_PER_BAND), 0, 255))); // Change color here
            }
        }
    }
    else // Fading mode
    {
        for (int8_t band = 0; band < NUM_BANDS; band++)
        {
            for (int y = 0; y < NUM_LEDS_PER_BAND; y++)
            {
                uint8_t color = (float)((float)band / (float)NUM_BANDS) * (float)255;
                uint8_t value = (int)((float)bandValues[band] * ((float)255 / (float)NUM_LEDS_PER_BAND));
                matrix->drawPixel(band, y, CHSV(color, 255, value)); // Change color here
            }
        }
    }
}

void drawBandsHeightsV2(uint8_t mode)
{
    for (int8_t band = 0; band < NUM_BANDS; band++)
    {
        uint8_t highLED = ((bandValues[band] - (bandValues[band]) % 255) / 255);
        for (int led = 0; led < highLED; led++)
        {
            matrix->drawPixel(band, led, CHSV(255, 255, 255)); // Full RED
        }
        matrix->drawPixel(band, highLED, CHSV(255, 255, bandValues[band] % 255)); // Change color here
        for (int led = highLED + 1; led < NUM_LEDS_PER_BAND; led++)
        {
            matrix->drawPixel(band, led, CHSV(150, 255, 255)); // OFF
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