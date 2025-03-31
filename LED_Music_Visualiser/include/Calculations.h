#ifndef CALCULATIONS_H
#define CALCULATIONS_H

#include "Settings.h"

/***** Variables *****/
extern double vReal[NB_SAMPLES];
extern double vImag[NB_SAMPLES];

/***** Function prorotypes *****/
void initVariables(void);
void computeFFT(void);
void assignFreqBinsValues(void);

#endif /* CALCULATIONS_H */