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

  // For Fs = 44 100 Hz from 100 Hz to 12000 kHz
  for (int i = 1; i < (NB_SAMPLES / 2); i++)
  {
    if (i <= 2)
      bandValues[0] += (int)vReal[i];
    if (i > 2 && i <= 4)
      bandValues[1] += (int)vReal[i];
    if (i > 4 && i <= 7)
      bandValues[2] += (int)vReal[i];
    if (i > 7 && i <= 12)
      bandValues[3] += (int)vReal[i];
    if (i > 12 && i <= 22)
      bandValues[4] += (int)vReal[i];
    if (i > 22 && i <= 38)
      bandValues[5] += (int)vReal[i];
    if (i > 38 && i <= 66)
      bandValues[6] += (int)vReal[i];
    if (i > 66 && i <= 115)
      bandValues[7] += (int)vReal[i];
    if (i > 115 && i <= 201)
      bandValues[8] += (int)vReal[i];
    if (i > 201 && i <= 351)
      bandValues[9] += (int)vReal[i];
  }
}

#endif /*   FFT_H   */
