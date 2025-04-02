#ifndef AUDIODATA_H
#define AUDIODATA_H

#include "Settings.h"

#define I2S_PORT I2S_NUM_0 // Use I2S0 peripheral

int readAudioSamples(void);
void setupI2sAudio(void);

#endif /* AUDIODATA_H */