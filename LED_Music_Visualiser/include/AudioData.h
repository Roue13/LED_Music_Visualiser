#ifndef AUDIODATA_H
#define AUDIODATA_H

#include "Settings.h"
#include <stdint.h>

/***** Variables *****/
#define I2S_PORT I2S_NUM_0 // Use I2S0 peripheral
extern int16_t readBuffer[NB_SAMPLES];
extern int oldBandValues[NUM_BANDS];
extern int bandValues[NUM_BANDS];
extern int bandPeaks[NUM_BANDS];

/***** MEMS Microphone linked functions *****/
int readDataMicrophone(void);
void setupI2sMicrophone(void);

/***** Bluetooth linked functions *****/
void readDataStream(const uint8_t, uint32_t);
int readDataBluetooth(void);
int readAudioSamples(void);
void checkBTConnectionState(void);
void forceBluetoothReconnect(void);
void setupI2sBluetooth(void);

void setupI2sAudio(void);

#endif /* AUDIODATA_H */