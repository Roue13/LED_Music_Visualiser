#include <Arduino.h>
#include "I2S_SETUP.h"
#include "DISPLAY.h"
#include "FFT.h"

void setup()
{
  Serial.begin(115200);
  Serial.println(" ***** Starting setup *****");
  setup_i2s();
  initDisplay();

  Serial.println(" ***** Setup finished *****");
  delay(1000);
}

void loop()
{
  int readSamples = readSampledData(); // Gets data from DMA buffers

  for (int i = 0; i < readSamples; i++)
  {
    vReal[i] = readBuffer[i];
    vImag[i] = 0;
  }

  computeFFT();           // Applies all FFT calculations
  assignFreqBinsValues(); // Gets values for frequency bands
  processBands();         // Assign new band heights and peaks values
  drawBandsHeights();     // Draw new band heights values
  drawPeaks();            // Draw new band peaks   values
  EVERY_N_MILLISECONDS(PEAK_DECAY_SPEED)
  {
    peaksDecay();
  }

  FastLED.show();
}
