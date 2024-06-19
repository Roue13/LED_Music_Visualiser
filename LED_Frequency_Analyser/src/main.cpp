#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <U8g2lib.h>
#include "Defs.h"
#include "I2S_SETUP.h"
#include "FFT.h"

U8G2_SS1315_128X;

void setup()
{
  Serial.begin(115200);
  Serial.println(" ***** Starting setup *****");
  setup_i2s();

  Serial.println(" ***** Setup finished *****");
  delay(1000);
}

void loop()
{
  size_t readBytes = 0;
  esp_err_t result = i2s_read(I2S_PORT, &readBuffer, sizeof(readBuffer), &readBytes, portMAX_DELAY);

  if (readBytes != sizeof(readBuffer))
  {
    Serial.printf("Could only read %u bytes of %u in FillBufferI2S()\n", readBytes, sizeof(readBuffer));
    // return;
  }

  int readSamples = readBytes / sizeof(int16_t); // 16 bit per sample

  for (int i = 0; i < readSamples; i++)
  {
    vReal[i] = readBuffer[i];
    vImag[i] = 0;
  }

  FFT.dcRemoval();
  FFT.windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.compute(FFT_FORWARD);
  FFT.complexToMagnitude();
}
