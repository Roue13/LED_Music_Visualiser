#ifndef SETTINGS_H
#define SETTINGS_H

#define DEBUG 0

/***** PINS connections *****/
#define I2S_WS 25       // WS pin
#define I2S_SD 33       // SD pin
#define I2S_SCK 32      // SCK pin
#define LED_DATA_PIN 23 // Led strips DATA pin

/***** Data Processing Parameters *****/
#define SAMPLING_FREQUENCY 48000
#define NB_SAMPLES 1024     // Between 8 and 1024
#define NB_DMA_BUFFERS 2    // Between 2 and ??? 128 ???
#define LOW_FREQUENCY 80    // Lowest frequency in Hz
#define HIGH_FREQUENCY 8000 // Highest frequency in Hz

/***** LED Setup Parameters *****/
#define NUM_BANDS 12
#define NUM_LEDS_PER_BAND 12
#define TOTAL_LEDS NUM_LEDS_PER_BAND *NUM_BANDS
#define BRIGHTNESS 240
#define LED_STRIP_TYPE WS2812B
#define LED_STRIP_VOLTAGE 5
#define LED_STRIP_MAX_CURRENT 3500

/***** LED Setup Parameters *****/
#define AVERAGE_FACTOR 60     // Between 0 (soft) and 100 (reactive)
#define AMPLITUDE_RAW 15000   // Bigger = More attenuation
#define AMPLITUDE_TRAILING 70 // Bigger = More attenuation
#define PEAK_DECAY_SPEED 60   // Time in ms between each fade out sweep

#define T3_FACTOR_1ST 60 // Trailing 3, lowest trailing led luminosity factor (in %)
#define T3_FACTOR_2ND 25 // Trailing 3, middle trailing led luminosity factor (in %)
#define T3_FACTOR_3RD 15 // Trailing 3, highest trailing led luminosity factor (in %)
#define T1_FACTOR 50     // Trailing 1, led luminosity factor (in %)

#endif /*   SETTINGS_H    */