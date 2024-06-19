#ifndef FFT_H
#define FFT_H

#include <arduinoFFT.h>

float vReal[NB_SAMPLES];
float vImag[NB_SAMPLES];

ArduinoFFT<float> FFT = ArduinoFFT<float>(vReal, vImag, NB_SAMPLES, SAMPLING_FREQUENCY);

/*
----- Frequency bins for 125 Hz --> 16 kHz at Fs = 44,1 kHz -----
Band	Frequency	    Center bin	    Low bin	    High bin
0	    125	            3	            0	        4
1	    250	            6	            4	        9
2	    500	            12	            9	        17
3	    1000	        23	            17	        35
4	    2000	        46	            35	        70
5	    4000	        93	            70	        139
6	    8000	        186	            139     	279
7	    16000	        372	            279	        557


----- Frequency bins for 125 Hz --> 16 kHz at Fs = 48 kHz -----
Band	Frequency	    Center bin	    Low bin	    High bin
0	    125	            3	            0	        4
1	    250	            5	            4	        8
2	    500	            11	            8	        16
3	    1000	        21	            16	        32
4	    2000	        43	            32	        64
5	    4000	        85	            64	        128
6	    8000	        171	            128	        256
7	    16000	        341	            256	        512

*/

void computeFFT()
{
    FFT.dcRemoval();
    FFT.windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    FFT.compute(FFT_FORWARD);
    FFT.complexToMagnitude();
}

void assignFreqBinsValues()
{
    for (int i = 2; i < (NB_SAMPLES / 2); i++)
    {
        if (i <= 4)
            bandValues[0] += (int)vReal[i];
        if (i > 4 && i <= 9)
            bandValues[1] += (int)vReal[i];
        if (i > 9 && i <= 17)
            bandValues[2] += (int)vReal[i];
        if (i > 17 && i <= 35)
            bandValues[3] += (int)vReal[i];
        if (i > 35 && i <= 70)
            bandValues[4] += (int)vReal[i];
        if (i > 70 && i <= 139)
            bandValues[5] += (int)vReal[i];
        if (i > 139 && i <= 279)
            bandValues[6] += (int)vReal[i];
        if (i > 279)
            bandValues[7] += (int)vReal[i];
    }
}

#endif /*   FFT_H   */
