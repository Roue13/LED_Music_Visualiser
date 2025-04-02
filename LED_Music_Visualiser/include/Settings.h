#ifndef SETTINGS_H
#define SETTINGS_H

/***** DEBUG settings. Set to 1 to activate, else 0 *****/
#define DEBUG_BT 0
#define DEBUG_DATA 0
#define DEBUG_CONNEXION 0
#define DEBUG_THREADS 1
#define DEBUG 0

/***** PINS Connections *****/
#define I2S_WS 25       // MEMS WS pin
#define I2S_SD 33       // MEMS SD pin
#define I2S_SCK 32      // MEMS SCK pin
#define LED_DATA_PIN 23 // Led strip DATA pin
#define BTN_MODE_PIN 27 // Button used to switch between audio input modes

/***** Bluetooth Parameters *****/
#define MODE_MICROPHONE 0                     // Microphone audio input mode
#define MODE_BLUETOOTH 1                      // Bluetooth audio input mode
#define AUDIO_MODE MODE_BLUETOOTH             // Audio mode to use when starting
#define BT_DEVICE_NAME "LED Music Visualiser" // Name shown when looking for the ESP32 Bluetooth device
#define DELAY_BEFORE_FORCE_RECONNECT 10       // Delay before forcing a deconnexion + ESP_restart in seconds
#define SPEED_STATE_MESSAGE_SWEEP 85          // Speed of the displayed state message in ms

/***** Data Processing Parameters *****/
#define SAMPLING_FREQUENCY 72000
#define NB_SAMPLES 1024   // Between 8 and 1024
#define NB_DMA_BUFFERS 16 // Between 2 and ??? 128 ???
// #define LOW_FREQUENCY 50     // Lowest frequency in Hz
#define HIGH_FREQUENCY 16000 // Highest frequency in Hz

/***** LED Setup Parameters *****/
#define NUM_BANDS 12         // Number of columns
#define NUM_LEDS_PER_BAND 12 // Number of rows
#define TOTAL_LEDS NUM_LEDS_PER_BAND *NUM_BANDS
#define BRIGHTNESS 180             // LED Strip general brightness (between 0 and 255)
#define LED_STRIP_TYPE WS2812B     // LED Strip type
#define LED_STRIP_VOLTAGE 5        // LED Strip required voltage (in V)
#define LED_STRIP_MAX_CURRENT 3500 // Max power source current (in mA)

/***** LED Visuals Parameters *****/
#define AVERAGE_FACTOR 55     // Between 0 (soft) and 100 (reactive)
#define AMPLITUDE_RAW 15000   // Bigger = More attenuation
#define AMPLITUDE_TRAILING 70 // Bigger = More attenuation
#define PEAK_DECAY_SPEED 60   // Time in ms between each fade out sweep
#define T3_FACTOR_1ST 60      // Trailing 3, lowest trailing led luminosity factor (in %)
#define T3_FACTOR_2ND 25      // Trailing 3, middle trailing led luminosity factor (in %)
#define T3_FACTOR_3RD 15      // Trailing 3, highest trailing led luminosity factor (in %)
#define T1_FACTOR 50          // Trailing 1, led luminosity factor (in %)

#endif /*   SETTINGS_H    */