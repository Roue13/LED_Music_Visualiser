#ifndef HARDWARE_H
#define HARDWARE_H

#include "Defs.h"
#include <driver/i2s.h>

#define I2S_WS 25
#define I2S_SD 33
#define I2S_SCK 32

#define I2S_PORT I2S_NUM_0

#define SAMPLING_FREQUENCY 44100
#define BUFFER_LEN 1024
int16_t readBuffer[BUFFER_LEN];

void setup_i2s()
{
  Serial.println("Configuring I2S...");
  esp_err_t err;

  // Set up I2S Processor configuration
  const i2s_config_t i2s_config = {
      .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX),
      .sample_rate = SAMPLING_FREQUENCY,
      .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
      .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
      .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_STAND_I2S),
      .intr_alloc_flags = 0,
      .dma_buf_count = 4,
      .dma_buf_len = BUFFER_LEN,
      .use_apll = false};

  err = i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);
  if (err != ESP_OK)
  {
    Serial.print("Error while installing I2S drivers : ");
    Serial.println(err);
  }
  else
  {
    Serial.println("I2S drivers correctly installed");
  }

  // Set I2S pin configuration
  const i2s_pin_config_t pin_config = {
      .bck_io_num = I2S_SCK,
      .ws_io_num = I2S_WS,
      .data_out_num = I2S_PIN_NO_CHANGE,
      .data_in_num = I2S_SD};

  err = i2s_set_pin(I2S_PORT, &pin_config);
  if (err != ESP_OK)
  {
    Serial.print("Error setting I2S pins : ");
    Serial.println(err);
  }
  else
  {
    Serial.println("I2S pins correctly configured");
  }
}

#endif /*   HARDWARE_H    */