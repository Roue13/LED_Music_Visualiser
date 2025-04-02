#include "AudioData.h"
#include "Global.h"

int readAudioSamples(void)
{
    int readSamples = 0;
    if (AUDIO_MODE == MODE_MICROPHONE)
    {
        readSamples = readDataMems();
    }
    else if (AUDIO_MODE == MODE_BLUETOOTH)
    {
        readSamples = readDataBluetooth();
    }
    return readSamples;
}

void setupI2sAudio()
{
    if (AUDIO_MODE == MODE_MICROPHONE)
    {
        setupI2sMems();
    }
    else if (AUDIO_MODE == MODE_BLUETOOTH)
    {
        setupI2sBluetooth();
    }
}