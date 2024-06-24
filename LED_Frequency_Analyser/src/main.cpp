#include <Arduino.h>
#include <math.h>

#define DEBUG 0

#include "Settings.h"
#include "LedDisplay.h"
#include "LedMatrix.h"
#include "FFT.h"
#include "I2S_Settings.h"

void setup()
{
#if DEBUG
  Serial.begin(115200);
  Serial.println("***** Starting setup *****");
#endif

  setup_i2s_MEMS();
  initDisplay();
  initFreqBands();

#if DEBUG
  Serial.println("***** Setup finished *****");
  delay(1000);
#endif
}

void loop()
{
  FastLED.clear();
  int readSamples = readSampledData(); // Gets data from DMA buffers

  for (int i = 0; i < readSamples; i++) //
  {
    vReal[i] = readBuffer[i];
    vImag[i] = 0;
  }

  computeFFT();           // Applies all FFT calculations
  assignFreqBinsValues(); // Gets values for frequency bands
  processBands();         // Assign new band heights and peaks values
  drawBandsHeights();     // Draw new band heights values
  drawPeaks();            // Draw new band peaks   values
  EVERY_N_MILLISECONDS(PEAK_DECAY_SPEED) { peaksDecay(); }

  FastLED.show();
}
