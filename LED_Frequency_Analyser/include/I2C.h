#ifndef I2C_H
#define I2C_H

#include "Defs.h"

#define I2S_PORT I2S_NUM_0

void i2s_setup() {
  // Set up I2S Processor configuration
  const i2s_config_t i2s_config = {
    .mode = i2s_mode_t(I2S_MODE_ADC_BUILT_IN),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = i2s_bits_per_sample_t(I2S_BITS_PER_SAMPLE_16BIT),
    .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
    .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_STAND_I2S),
    .intr_alloc_flags = 0,
    .dma_buf_count = 4,
    .dma_buf_len = 1024,
    .use_apll = false,
    .tx_desc_auto_clear = false,
    .fixed_mclk = 0
  };
 
  esp_err_t err = i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);
  if (err != ESP_OK) {
    Serial.printf("Failed to install I2S driver: %d\n", err);
    return;
  }
}

void adc_setup() {
    i2s_set_adc_mode(ADC_UNIT_1, ADC_INPUT);
    i2s_adc_enable(I2S_NUM_0);
    adc1_config_channel_atten(ADC_INPUT, ADC_ATTEN_DB_11);
}


#endif // I2C_H