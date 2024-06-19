#ifndef FFT_H
#define FFT_H

#include <arduinoFFT.h>

float vReal[BUFFER_LEN];
float vImag[BUFFER_LEN];

ArduinoFFT<float> FFT = ArduinoFFT<float>(vReal, vImag, BUFFER_LEN, SAMPLING_FREQUENCY);

#endif /*   FFT_H   */