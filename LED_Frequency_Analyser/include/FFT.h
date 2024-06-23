#ifndef FFT_H
#define FFT_H

#include <arduinoFFT.h>

double vReal[NB_SAMPLES];
double vImag[NB_SAMPLES];

ArduinoFFT<double> FFT = ArduinoFFT<double>(vReal, vImag, NB_SAMPLES, SAMPLING_FREQUENCY);

void computeFFT()
{
  FFT.dcRemoval();
  FFT.windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.compute(FFT_FORWARD);
  FFT.complexToMagnitude();
}

void assignFreqBinsValues()
{
  for (int band = 0; band < NUM_BANDS; band++)
  {
    bandValues[band] = 0;
  }

  // For Fs = 48000 Hz from 100 Hz to 12000 kHz
  for (int i = 1; i < (NB_SAMPLES / 2); i++)
  {
    if (i <= 3)
      bandValues[0] += (int)vReal[i];
    if (i > 3 && i <= 4)
      bandValues[1] += (int)vReal[i];
    if (i > 4 && i <= 6)
      bandValues[2] += (int)vReal[i];
    if (i > 6 && i <= 10)
      bandValues[3] += (int)vReal[i];
    if (i > 10 && i <= 15)
      bandValues[4] += (int)vReal[i];
    if (i > 15 && i <= 24)
      bandValues[5] += (int)vReal[i];
    if (i > 24 && i <= 37)
      bandValues[6] += (int)vReal[i];
    if (i > 37 && i <= 57)
      bandValues[7] += (int)vReal[i];
    if (i > 57 && i <= 88)
      bandValues[8] += (int)vReal[i];
    if (i > 88 && i <= 136)
      bandValues[9] += (int)vReal[i];
    if (i > 136 && i <= 211)
      bandValues[10] += (int)vReal[i];
    if (i > 211 && i <= 326)
      bandValues[11] += (int)vReal[i];
  }
}

#endif /*   FFT_H   */
