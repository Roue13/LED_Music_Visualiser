#ifndef DISPLAY_H
#define DISPLAY_H

#include "BluetoothA2DPSink.h"

/// Initialise LED strip parameters and characteristics
void initDisplay(void);

/* %%%%%%%%%%%%%%%%%%%% RAW CALCULATION / DISPLAY %%%%%%%%%%%%%%%%%%%% */

/// Calculations between sampling and display.
/// Outpout LED values are 0 or 255 (OFF - ON)
void processBandsRaw(void);

/// Display raw calculated LED values.
/// Each LED can be ON or OFF without smoothing or transitions
void drawBandsHeightsRaw();

/* %%%%%%%%%%%%%%%%%%%% Full CALCULATION / DISPLAY %%%%%%%%%%%%%%%%%%%% */

/// Calculations between sampling and display.
/// Outpout LED values are between 0 and 255 (smoother display)
void processBandsFull(void);

/// Display full calculated LED values.
/// Trailing 1 --> The highest LED of each band smoothly fade out
void drawBandsHeightsT1();

/// Display full calculated LED values.
/// Trailing 3 --> The 3 highest LEDs of each band smoothly fade out
void drawBandsHeightsT3();

/* %%%%%%%%%%%%%%%%%%%% Full CALCULATION / DISPLAY %%%%%%%%%%%%%%%%%%%% */
void drawPeaks(void);
void peaksDecay(void);

/* %%%%%%%%%%%%%%%%%%%% State Animations / Messages %%%%%%%%%%%%%%%%%%%% */
void drawConnectionState(esp_a2d_connection_state_t);

#endif /* DISPLAY_H */