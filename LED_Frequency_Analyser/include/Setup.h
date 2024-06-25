#ifndef SETUP_H
#define SETUP_H

#include "Settings.h"

#include <Arduino.h>
#include <driver/i2s.h>
#include <driver/adc.h>

#define I2S_PORT I2S_NUM_0 // Use I2S0 peripheral

extern int16_t readBuffer[NB_SAMPLES];
extern int oldBandValues[NUM_BANDS];
extern int bandValues[NUM_BANDS];
extern int bandPeaks[NUM_BANDS];

void setup_i2s_MEMS(void);

int readSampledData(void);

#endif /* SETUP_H */