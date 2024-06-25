#include "Setup.h"

int16_t readBuffer[NB_SAMPLES] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int oldBandValues[NUM_BANDS] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int bandValues[NUM_BANDS] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int bandPeaks[NUM_BANDS] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

void setup_i2s_MEMS()
{
    esp_err_t err;

    // Set up I2S Processor configuration
    const i2s_config_t i2s_config_mems = {
        .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX),
        .sample_rate = SAMPLING_FREQUENCY,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
        .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_STAND_I2S),
        .intr_alloc_flags = 0,
        .dma_buf_count = NB_DMA_BUFFERS,
        .dma_buf_len = NB_SAMPLES,
        .use_apll = false};

    err = i2s_driver_install(I2S_PORT, &i2s_config_mems, 0, NULL);
#if DEBUG
    if (err != ESP_OK)
    {
        Serial.print("Error while installing I2S drivers : ");
        Serial.println(err);
    }
    else
    {
        Serial.println("I2S drivers correctly installed");
    }
#endif

    // Set I2S pin configuration
    const i2s_pin_config_t pin_config = {.bck_io_num = I2S_SCK,
                                         .ws_io_num = I2S_WS,
                                         .data_out_num = I2S_PIN_NO_CHANGE,
                                         .data_in_num = I2S_SD};

    err = i2s_set_pin(I2S_PORT, &pin_config);
#if DEBUG
    if (err != ESP_OK)
    {
        Serial.print("Error setting I2S pins : ");
        Serial.println(err);
    }
    else
    {
        Serial.println("I2S pins correctly configured");
    }
#endif
}

int readSampledData()
{
    size_t readBytes = 0;
    esp_err_t result = i2s_read(I2S_PORT, &readBuffer, sizeof(readBuffer),
                                &readBytes, portMAX_DELAY);
#if DEBUG
    if (readBytes != sizeof(readBuffer))
    {
        Serial.printf("Could only read %u bytes of %u in FillBufferI2S()\n",
                      readBytes, sizeof(readBuffer));
        // return;
    }
#endif

    int readSamples = readBytes / sizeof(int16_t); // 16 bit per sample

    return readSamples;
}
