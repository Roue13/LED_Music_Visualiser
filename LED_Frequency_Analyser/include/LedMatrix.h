#ifndef LEDMATRIX_H
#define LEDMATRIX_H

DEFINE_GRADIENT_PALETTE(pink_red_gp)
{
    0  , 255, 182, 193,   // Pink
    255, 255, 0  , 0      // Red
};


DEFINE_GRADIENT_PALETTE(cyan_blue_gp)
{
    0  , 0  , 255, 255,   // Cyan
    255, 0  , 0  , 200    // Dark Blue
};


DEFINE_GRADIENT_PALETTE(lightYellow_Green_gp)
{
    0  , 255, 250, 205,  // Light Yellow
    255, 0  , 210, 0     // Dark Green
};


CRGBPalette16 pinkRedPal = pink_red_gp;
CRGBPalette16 cyanBluePal = cyan_blue_gp;
CRGBPalette16 yellowGreenPal = lightYellow_Green_gp;

#endif /*   LEDMATRIX_H     */