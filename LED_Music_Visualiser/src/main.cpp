
/***** Files and Libraries *****/
#include <Arduino.h>
#include <math.h>
#include "Settings.h"
#include "AudioData.h"
#include "Calculations.h"
#include "Display.h"

/***** SETUP *****/
void setup()
{
#if DEBUG
  Serial.begin(115200);
  Serial.println("***** Starting setup *****");
#endif
  setupI2sAudio();
  initDisplay();
  initVariables();

#if DEBUG
  Serial.println("***** Setup finished *****");
  delay(1000);
#endif
}

/***** MAIN LOOP *****/
void loop()
{
  FastLED.clear();
  int readSamples = readAudioSamples(); // Gets data from DMA buffers

  for (int i = 0; i < readSamples; i++) //
  {
    vReal[i] = readBuffer[i];
    vImag[i] = 0;
  }

  computeFFT();           // Applies all FFT calculations
  assignFreqBinsValues(); // Gets values for frequency bands
  processBandsRaw();      // Assign new band heights and peaks values
  drawBandsHeightsRaw();  // Draw new band heights values (0 = normal, 1 = fading)
  /*drawPeaks();            // Draw new band peaks   values
  EVERY_N_MILLISECONDS(PEAK_DECAY_SPEED)
  {
    peaksDecay();
  }*/

  FastLED.show();
}
