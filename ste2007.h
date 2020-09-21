#ifndef __STE2007__
#define __STE2007__
#include <Arduino.h>  // Include the Arduino header file
#include "font.h"
#if defined(__AVR__)
#include <avr/pgmspace.h>
#endif

#define X_PIXELS            96
#define Y_PIXELS            68
#define DISPLAY_STR_SIZE    9
#define W                   94
#define H                   66
#define Y_ADDR              0xB0
#define X_ADDR1             0x00
#define X_ADDR2             0x10
#define PROGRAM             pgm_read_byte
#define SEND_DATA           1
#define SEND_CMD            0

#define SWAP(x, y) \
    {              \
        int t = x; \
        x = y;     \
        y = t;     \
    }
/*  Creating class for nokia lcd.
    This library supports the display which are STE2007 compatible
*/

class STE2007 {
    public:
        STE2007(uint8_t _RESET_PIN, uint8_t _CE_PIN, uint8_t _MOSI_PIN, uint8_t _CLK_PIN);
        void Initialize();
        void Clear();
        void Update();
        void drawPixel(byte x, byte y, boolean color);
        void fillScreen(boolean color);
        void drawChar(byte x, byte y, boolean color, unsigned char c);
        void print(byte x, byte y, boolean color, char *str);
        void print(byte x, byte y, boolean color, long num);
        void print_1607(byte x, byte y, boolean color, char *str);
        void drawLine(byte x0, byte y0, byte x1, byte y1, boolean color);
        void drawFastVLine(byte x, byte y, byte h, boolean color);
        void drawFastHLine(byte x, byte y, byte w, boolean color);
        void drawRect(byte x, byte y, byte w, byte h, boolean color);
        void drawCircle(byte x0, byte y0, int16_t r, boolean color);
        void drawRoundRect(byte x, byte y, byte w, byte h, byte r, boolean color);
        void drawTriangle(byte x0, byte y0, byte x1, byte y1, byte x2, byte y2, boolean color);
        void drawCircleHelper(byte x0, byte y0, byte r, byte cornername, boolean color);
        void fillCircle(byte x0, byte y0, byte r, boolean color);
        void fillCircleHelper(byte x0, byte y0, byte r, byte cornername, byte delta, boolean color);
        void fillRect(byte x, byte y, byte w, byte h, boolean color);
        void fillRoundRect(byte x, byte y, byte w, byte h, byte r, boolean color);
        void fillTriangle(byte x0, byte y0, byte x1, byte y1, byte x2, byte y2, boolean color);
        void drawBitmap(byte x, byte y, const char *bitmap, byte w, byte h, boolean color);
        void simb16x32(byte x, byte y, boolean color, byte c);
        void simb10x16(byte x, byte y, boolean color, byte c);
    private:
        void SendByte(byte mode, byte c);
};
#endif
