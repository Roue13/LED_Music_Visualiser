#include "AudioData.h"
#include "Global.h"

int16_t readBuffer[NB_SAMPLES] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int oldBandValues[NUM_BANDS] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int bandValues[NUM_BANDS] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int bandPeaks[NUM_BANDS] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
BluetoothA2DPSink a2dp_sink;
int bluetoothDataLength = 0; // Length of Bluetooth data
unsigned long lastBTCheck = 0;

void readDataStream(const uint8_t *data, uint32_t length)
{
    bluetoothRecievingData = true;
    int16_t *values = (int16_t *)data;
    int samples = length / sizeof(int16_t); // Nb of recieved samples

    // Limit stored data
    bluetoothDataLength = min((const int)samples, NB_SAMPLES);

    // Copy recieved data in the global buffer
    memcpy(readBuffer, values, bluetoothDataLength * sizeof(int16_t));

#if DEBUG
    Serial.printf("Bluetooth Data Received: %u samples\n", bluetoothDataLength);
    for (size_t i = 0; i < bluetoothDataLength; i += 2)
    {
        Serial.printf("L: %d, R: %d\n", readBuffer[i], readBuffer[i + 1]);
    }
#endif
}

int readDataMicrophone()
{
    size_t readBytes = 0;
    esp_err_t result = i2s_read(I2S_PORT, &readBuffer, sizeof(readBuffer), &readBytes, portMAX_DELAY);
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

int readDataBluetooth()
{
    return bluetoothDataLength;
}

int readAudioSamples(void)
{
    int readSamples = 0;
    if (AUDIO_MODE == MODE_MICROPHONE)
    {
        readSamples = readDataMicrophone();
    }
    else if (AUDIO_MODE == MODE_BLUETOOTH)
    {
        readSamples = readDataBluetooth();
    }
    return readSamples;
}

void setupI2sMicrophone()
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

void bt_connexion_toggle(esp_a2d_connection_state_t state, void *ptr)
{
    switch (state)
    {
    case ESP_A2D_CONNECTION_STATE_CONNECTED:
        bluetoothConnexionState = true;
#if DEBUG_BT
        Serial.println("Bluetooth Connected!");
#endif
        break;

    case ESP_A2D_CONNECTION_STATE_CONNECTING:
        bluetoothRecievingData = false;
        bluetoothConnexionState = false;
#if DEBUG_BT
        Serial.println("Bluetooth Connecting!");
#endif
        break;

    case ESP_A2D_CONNECTION_STATE_DISCONNECTED:
        bluetoothRecievingData = false;
        bluetoothConnexionState = false;
#if DEBUG_BT
        Serial.println("Bluetooth Disconnected!");
#endif
        break;

    case ESP_A2D_CONNECTION_STATE_DISCONNECTING:
        bluetoothRecievingData = false;
        bluetoothConnexionState = false;
#if DEBUG_BT
        Serial.println("Bluetooth Disconnecting!");
#endif
        break;
    }

    drawConnectionState(state);
}

void forceBluetoothReconnect()
{
    Serial.println("Forcing Bluetooth reconnect...");

    Serial.println("Disconnecting Bluetooth...");
    a2dp_sink.disconnect();
    delay(2000); // Petit délai pour éviter un bug de reconnexion immédiate

    // Reconnexion
    Serial.println("Reconnecting Bluetooth...");
    setupI2sBluetooth();
}

void checkBTConnectionState()
{
    if (a2dp_sink.get_connection_state() == ESP_A2D_CONNECTION_STATE_CONNECTING)
    {
        // Verify every 10sec if stuck in "Connecting state"
        if (millis() - lastBTCheck > DELAY_BEFORE_FORCE_RECONNECT * 1000)
        {
            Serial.println("Disconnecting Bluetooth...");
            a2dp_sink.disconnect();
            restartESP();
        }
        lastBTCheck = millis();

        // Reset time counter if a connexion has been established
        if (bluetoothConnexionState)
        {
            lastBTCheck = 0;
        }
    }
}

void setupI2sBluetooth()
{
    // Free I2S ressources if already used
    i2s_driver_uninstall(I2S_PORT);

    // Configure I2S for Bluetooth
    const i2s_config_t i2s_config_bluetooth = {
        .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX),
        .sample_rate = SAMPLING_FREQUENCY,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
        .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_STAND_I2S),
        .intr_alloc_flags = 0,
        .dma_buf_count = NB_DMA_BUFFERS,
        .dma_buf_len = NB_SAMPLES,
        .use_apll = false};

    // Install I2S drivers
    i2s_driver_install(I2S_PORT, &i2s_config_bluetooth, 0, NULL);

    // Configure I2S pins
    const i2s_pin_config_t pin_config = {
        .bck_io_num = I2S_SCK,
        .ws_io_num = I2S_WS,
        .data_out_num = I2S_SD};
    i2s_set_pin(I2S_PORT, &pin_config);

    // Empty DMA buffers
    i2s_zero_dma_buffer(I2S_PORT);

    // Activate BT with auto-reconnect
    bluetoothConnexionState = false;
    a2dp_sink.set_stream_reader(readDataStream);                    // Callback used when data is recieved
    a2dp_sink.set_on_connection_state_changed(bt_connexion_toggle); // Callback used when connexion state changes
    a2dp_sink.start(BT_DEVICE_NAME, false);                         // Starts Bluetooth without "auto-reconnect" (false)
    drawConnectionState(ESP_A2D_CONNECTION_STATE_DISCONNECTED);
}

void setupI2sAudio()
{
    if (AUDIO_MODE == MODE_MICROPHONE)
    {
        setupI2sMicrophone();
    }
    else if (AUDIO_MODE == MODE_BLUETOOTH)
    {
        setupI2sBluetooth();
    }
}