#ifndef FFT_H
#define FFT_H

#include <arduinoFFT.h>

double vReal[NB_SAMPLES];
double vImag[NB_SAMPLES];

ArduinoFFT<double> FFT = ArduinoFFT<double>(vReal, vImag, NB_SAMPLES, SAMPLING_FREQUENCY);

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

----- Frequency bins for 125 Hz --> 16 kHz at Fs = 70 kHz -----
Band	Frequency	    Center bin	    Low bin	    High bin
0	    125	            2	            0	        3
1	    250	            4	            3	        5
2	    500	            7	            5	        11
3	    1000	        15	            11	        22
4	    2000	        29	            22	        44
5	    4000	        59	            44	        88
6	    8000	        117	            88	        176
7	    16000	        234	            176	        351

Band	Frequency	    Center bin	    Low bin	    High bin
0	    100	            1	            0	        2
1	    200	            3	            2	        4
2	    400	            6	            4	        9
3	    800	            12	            9	        18
4	    1600	        23	            18	        35
5	    3200	        47	            35	        70
6	    6400	        94	            70	        140
7	    12800	        187	            140	        281

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
    for (int band = 0; band < NUM_BANDS; band++)
    {
        bandValues[band] = 0;
    }

    /* For Fs = 70 000 Hz from 100 Hz to 14000 kHz
    for (int i = 1; i < (NB_SAMPLES / 2); i++)
    {
        if (i <= 2)
            bandValues[0] += (int)vReal[i];
        if (i > 2 && i <= 3)
            bandValues[1] += (int)vReal[i];
        if (i > 3 && i <= 6)
            bandValues[2] += (int)vReal[i];
        if (i > 6 && i <= 10)
            bandValues[3] += (int)vReal[i];
        if (i > 10 && i <= 18)
            bandValues[4] += (int)vReal[i];
        if (i > 18 && i <= 31)
            bandValues[5] += (int)vReal[i];
        if (i > 31 && i <= 54)
            bandValues[6] += (int)vReal[i];
        if (i > 54 && i <= 93)
            bandValues[7] += (int)vReal[i];
        if (i > 93 && i <= 162)
            bandValues[8] += (int)vReal[i];
        if (i > 162 && i <= 280)
            bandValues[9] += (int)vReal[i];
    }*/

    // For Fs = 44 100 Hz from 100 Hz to 12000 kHz
    for (int i = 1; i < (NB_SAMPLES / 2); i++)
    {
        if (i <= 3)
            bandValues[0] += (int)vReal[i];
        if (i > 3 && i <= 5)
            bandValues[1] += (int)vReal[i];
        if (i > 5 && i <= 9)
            bandValues[2] += (int)vReal[i];
        if (i > 9 && i <= 15)
            bandValues[3] += (int)vReal[i];
        if (i > 15 && i <= 26)
            bandValues[4] += (int)vReal[i];
        if (i > 26 && i <= 45)
            bandValues[5] += (int)vReal[i];
        if (i > 45 && i <= 76)
            bandValues[6] += (int)vReal[i];
        if (i > 76 && i <= 130)
            bandValues[7] += (int)vReal[i];
        if (i > 130 && i <= 221)
            bandValues[8] += (int)vReal[i];
        if (i > 221 && i <= 376)
            bandValues[9] += (int)vReal[i];
    }
}

#endif /*   FFT_H   */
