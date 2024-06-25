#ifndef CALCULATIONS_H
#define CALCULATIONS_H

#include "Settings.h"
#include <arduinoFFT.h>

extern double vReal[NB_SAMPLES];
extern double vImag[NB_SAMPLES];

void initVariables(void);

void computeFFT(void);

void assignFreqBinsValues(void);

#endif /* CALCULATIONS_H */