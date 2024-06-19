#ifndef DISPLAY_H
#define DISPLAY_H



U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

void initDisplay()
{
    u8g2.begin();
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.setDrawColor(1);
    u8g2.setFontPosTop(); // Upper left is the character position
    u8g2.clearBuffer();
    u8g2.drawStr(0, 0, "Start FFT v0.4");
    u8g2.sendBuffer();
}

#endif /*   DISPLAY_H   */