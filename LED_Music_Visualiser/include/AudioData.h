#ifndef AUDIODATA_H
#define AUDIODATA_H

/***** Files and Libraries *****/
#include <Arduino.h>
#include <driver/i2s.h>
#include <driver/adc.h>
#include "BluetoothA2DPSink.h"
#include "Settings.h"

/***** Variables *****/
#define I2S_PORT I2S_NUM_0 // Use I2S0 peripheral
extern int16_t readBuffer[NB_SAMPLES];
extern int oldBandValues[NUM_BANDS];
extern int bandValues[NUM_BANDS];
extern int bandPeaks[NUM_BANDS];

/***** Functions prototypes *****/

void setupI2sMicrophone(void);
void setupI2sBluetooth(void);
int readDataMicrophone(void);
void readDataStream(const uint8_t, uint32_t);
int readAudioSamples(void);

#endif /* AUDIODATA_H */