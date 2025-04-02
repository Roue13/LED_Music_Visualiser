#include "Calculations.h"
#include "Global.h"

/***** Variables and instances *****/
// For the FFT Library
double vReal[NB_SAMPLES] = {0};
double vImag[NB_SAMPLES] = {0}; // vImag will always stay at 0
ArduinoFFT<double> FFT = ArduinoFFT<double>(vReal, vImag, NB_SAMPLES, SAMPLING_FREQUENCY);

const double binWidth = (double)SAMPLING_FREQUENCY / (double)NB_SAMPLES;
const int LOW_FREQUENCY = (int)binWidth;
const double freqMultiplierPerBand = pow((double)(HIGH_FREQUENCY / LOW_FREQUENCY), 1 / (double)(NUM_BANDS - 1));
const double nbUsableBins = (NB_SAMPLES / 2) - 1;
int centerFreqBand[NUM_BANDS + 1] = {0};
int centerFreqBin[NUM_BANDS + 1] = {0};
int lowFreqBin[NUM_BANDS] = {0};
int highFreqBin[NUM_BANDS] = {0};
int oldBandValues[NUM_BANDS] = {0};
int bandValues[NUM_BANDS] = {0};
int bandPeaks[NUM_BANDS] = {0};

/***** Variables and instances *****/

void initVariables()
{
    // Center frequency and center bin (first)
    for (int band = 0; band < (NUM_BANDS + 1); band++)
    {
        centerFreqBand[band] = round(LOW_FREQUENCY * pow(freqMultiplierPerBand, band));
        centerFreqBin[band] = round(centerFreqBand[band] / binWidth);
    }

    // Low and high bin (then)
    for (int band = 0; band < NUM_BANDS; band++)
    {
        highFreqBin[band] = round(((centerFreqBin[band + 1] - centerFreqBin[band]) / 2) + centerFreqBin[band]);
        if (band == 0)
        {
            lowFreqBin[band] = 0;
        }
        else
        {
            round(lowFreqBin[band] = highFreqBin[band - 1]);
        }
    }

#if DEBUG
    for (int i = 0; i < 4; i++)
    {
        switch (i)
        {
        case 0:
            Serial.println("");
            Serial.print("Center Frequency Band : ");
            Serial.println("");
            for (int band = 0; band < NUM_BANDS; band++)
            {
                Serial.print("Band : ");
                Serial.print(band);
                Serial.print("  -->  ");
                Serial.print(centerFreqBand[band]);
                Serial.println(" Hz");
            }
            break;

        case 1:
            Serial.println("");
            Serial.print("Center Frequency Bin : ");
            Serial.println("");
            for (int band = 0; band < NUM_BANDS; band++)
            {
                Serial.print("Band : ");
                Serial.print(band);
                Serial.print("  -->  ");
                Serial.println(centerFreqBin[band]);
            }
            break;

        case 2:
            Serial.println("");
            Serial.print("Low Frequency Bin : ");
            Serial.println("");
            for (int band = 0; band < NUM_BANDS; band++)
            {
                Serial.print("Band : ");
                Serial.print(band);
                Serial.print("  -->  ");
                Serial.println(lowFreqBin[band]);
            }
            break;

        case 3:
            Serial.println("");
            Serial.print("High Frequency Bin : ");
            Serial.println("");
            for (int band = 0; band < NUM_BANDS; band++)
            {
                Serial.print("Band : ");
                Serial.print(band);
                Serial.print("  -->  ");
                Serial.println(highFreqBin[band]);
            }
            break;
        }
    }
#endif
}

void computeFFT()
{
    FFT.dcRemoval();
    FFT.windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    FFT.compute(FFT_FORWARD);
    FFT.complexToMagnitude();
    // Serial.printf("\nMax freq : %f Hz", FFT.majorPeak());
}

void assignFreqBinsValues()
{
    for (int band = 0; band < NUM_BANDS; band++)
    {
        bandValues[band] = 0;
        for (int i = 1; i < (NB_SAMPLES / 2); i++)
        {
            if (band == 0)
            {
                if (i <= highFreqBin[band])
                    bandValues[band] += (int)vReal[i];
            }
            else
            {
                if (i > lowFreqBin[band] && i <= highFreqBin[band])
                    bandValues[band] += (int)vReal[i];
            }
        }
    }
}
