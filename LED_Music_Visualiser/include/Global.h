#ifndef GLOBAL_H
#define GLOBAL_H

/***** Files and Libraries *****/
#include <Arduino.h>
#include <math.h>
#include <Preferences.h>
#include <algorithm>
#include <driver/i2s.h>
#include <driver/adc.h>
#include <string>
#include <Adafruit_GFX.h>
#include <FastLED.h>
#include <FastLED_NeoMatrix.h>
#include <SPI.h>
#include <Wire.h>
#include <arduinoFFT.h>
#include "BluetoothA2DPSink.h"

#include "Settings.h"
#include "AudioData.h"
#include "Calculations.h"
#include "Display.h"

/***** Variables *****/
// Preferences pref; // Object to handle NVS memory
extern bool bluetoothConnexionState;
extern bool bluetoothRecievingData;
extern BluetoothA2DPSink a2dp_sink;

/***** Functions prototypes *****/

/**
 * @brief Runs the main "display" loop
 */
void runDisplay(void);

void restartESP(void);

#endif /*   GLOBAL_H   */