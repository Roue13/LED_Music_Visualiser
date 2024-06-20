#ifndef SETTINGS_H
#define SETTINGS_H

/***** PINS connections *****/
#define I2S_WS       25   // WS pin
#define I2S_SD       33   // SD pin
#define I2S_SCK      32   // SCK pin
#define LED_DATA_PIN 23   // Led strips DATA pin


/***** Data Processing Parameters *****/
#define SAMPLING_FREQUENCY 48000
#define NB_SAMPLES         1024


/***** LED Display Parameters *****/
#define NUM_BANDS         12
#define NUM_LEDS_PER_BAND 12
#define BRIGHTNESS        200
#define MAX_CURRENT_MA    4000


#define AVERAGE_FACTOR   85      // Between 0 (soft) and 100 (reactive) --> 70 just in the middle
#define AMPLITUDE        10000   // Bigger = More attenuation
#define PEAK_DECAY_SPEED 70


#endif /*   SETTINGS_H    */