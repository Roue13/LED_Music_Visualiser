// Theme : Atom One Dark ("Atom One Dark Theme" Extension)

#include <Arduino.h>
#include <math.h>

//#define FASTLED_INTERNAL // Used to disable FastLED #pragma Warnings
#include "Settings.h"
#include "Setup.h"
#include "Calculations.h"
#include "Display.h"

void setup()
{
#if DEBUG
  Serial.begin(115200);
  Serial.println("***** Starting setup *****");
#endif

  setup_i2s_MEMS();
  initDisplay();
  initVariables();

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
  drawBandsHeights(0);    // Draw new band heights values (0 = normal, 1 = fading)
  drawPeaks();            // Draw new band peaks   values
  EVERY_N_MILLISECONDS(PEAK_DECAY_SPEED)
  {
    peaksDecay();
  }

  FastLED.show();
}
