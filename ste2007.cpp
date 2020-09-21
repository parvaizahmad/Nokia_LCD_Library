#include "ste2007.h"

byte DISPLAY_RAM[X_PIXELS*DISPLAY_STR_SIZE];
volatile uint8_t RESET_PIN, CE_PIN, MOSI_PIN, CLK_PIN;

/*  Class constractor, used to initialize the display. 
    Arguments: 
            _RESET_PIN : RESET_PIN pin 
            _CE_PIN    : Chip enable or chip select pin of display
            _MOSI_PIN  : Data pin 
            _CLK_PIN   : Clock sourCE_PIN pin
*/

STE2007::STE2007(uint8_t _RESET_PIN, uint8_t _CE_PIN, uint8_t _MOSI_PIN, uint8_t _CLK_PIN)
{
RESET_PIN = _RESET_PIN;
CE_PIN = _CE_PIN;
MOSI_PIN = _MOSI_PIN;
CLK_PIN = _CLK_PIN;
}

/* This menthod is used to clear the display. */

void STE2007::Clear() {
    for (int index = 0; index < 864 ; index++){
        DISPLAY_RAM[index] = (0x00);
    }
}

/* This menthod sends the whole byte to display controller. */

void STE2007::SendByte(byte mode, byte c){
    digitalWrite(CE_PIN, 0);
    (mode)? digitalWrite(MOSI_PIN,1) : digitalWrite(MOSI_PIN,0);
    digitalWrite(CLK_PIN, 1);

    for(byte i=0;i<8;i++){
    digitalWrite(CLK_PIN,0);
    (c & 0x80)? digitalWrite(MOSI_PIN,1) : digitalWrite(MOSI_PIN,0);
    digitalWrite(CLK_PIN,1);
    c <<= 1;
    }

    digitalWrite(CLK_PIN, 0);
}

/* This menthod update the screen display.*/

void STE2007::Update(){
    for(byte p = 0; p < 9; p++){
        this->SendByte(SEND_CMD, Y_ADDR | p);
        this->SendByte(SEND_CMD, X_ADDR1);
        this->SendByte(SEND_CMD, X_ADDR2);

        for (byte col = 0; col < X_PIXELS; col++)
        {
            this->SendByte(SEND_DATA, DISPLAY_RAM[(X_PIXELS * p) + col]);
        }
    }
}

/* This menthod initialized the pins for the screen.*/

void STE2007::Initialize(){
    pinMode(RESET_PIN,   OUTPUT);
    pinMode(CE_PIN,    OUTPUT);
    pinMode(MOSI_PIN,  OUTPUT);
    pinMode(CLK_PIN, OUTPUT);

    digitalWrite(RESET_PIN, 1);
    digitalWrite(CLK_PIN, 0);
    digitalWrite(MOSI_PIN, 0);
    digitalWrite(CE_PIN, 0);
    delay(20);
    digitalWrite(CE_PIN, 1);

    this->SendByte(SEND_CMD, 0x2F); // Power control set(charge pump on/off)
    this->SendByte(SEND_CMD, 0xA4);
    this->SendByte(SEND_CMD, 0xAF); // screen on / off
    this->Clear();
    this->Update();
}

/* This menthod draws a pixel.*/

void STE2007::drawPixel (byte x, byte y, boolean color) {
    if ((x < 0) || (x >= X_PIXELS) || (y < 0) || (y >= Y_PIXELS)) return;

    if (color) DISPLAY_RAM[x+ (y/8)*X_PIXELS] |= (1 << y%8);
    else       DISPLAY_RAM[x+ (y/8)*X_PIXELS] &= ~(1 << y%8); 
}

/* This menthod fill up the screen.*/

void STE2007::fillScreen(boolean color) {
    this->fillRect(0, 0, X_PIXELS, Y_PIXELS, color);
}

/* This menthod draws a character word on the display.*/

void STE2007::drawChar(byte x, byte y, boolean color, unsigned char c) 
{
    if((x >= X_PIXELS) ||(y >= Y_PIXELS) || ((x + 4) < 0) || ((y + 7) < 0))
    return;

    if(c<128)            c = c-32;
    if(c>=144 && c<=175) c = c-48;
    if(c>=128 && c<=143) c = c+16;
    if(c>=176 && c<=191) c = c-48;
    if(c>191)  return;

    for (byte i=0; i<6; i++ ) 
    {
        byte line;
        (i == 5)? line = 0x0 : line = PROGRAM(font+(c*5)+i);
        for (byte j = 0; j<8; j++) 
        {
            (line & 0x1) ? this->drawPixel(x + i, y + j, color) : this->drawPixel(x + i, y + j, !color);
            line >>= 1;
        }
    }
}

/* This menthod prints the char/string.*/

void STE2007::print(byte x, byte y, boolean color, char *str){
    unsigned char type = *str;
    if(type>=128) x=x-3;
    while(*str){
        this->drawChar(x, y, color, *str++);
        unsigned char type = *str;
        (type >= 128) ? x = x + 3 : x = x + 6;
    }
}

/* This menthod prints the numbers.*/

void STE2007::print(byte x, byte y, boolean color, long num){
    char c[20];
    this->print(x, y, color, ltoa(num, c, 10));
}

/* This menthod prints the 16*07 string.*/

void STE2007::print_1607(byte x, byte y, boolean color, char *str)
{
byte nPos[16]={0,6,12,18,24,30,36,42,48,54,60,66,72,78,84,90};
byte nStr[7]={1,10,20,30,40,50,60};
this->print(nPos[x], nStr[y], color, str);
}

/* This menthod drwas the line.*/

void STE2007::drawLine(byte x0, byte y0, byte x1, byte y1, boolean color) {
    int steep = abs(y1 - y0) > abs(x1 - x0);
    if (steep) {
        SWAP(x0, y0);
        SWAP(x1, y1);
    }
    if (x0 > x1) {
        SWAP(x0, x1);
        SWAP(y0, y1);
    }
    int dx, dy;
    dx = x1 - x0;
    dy = abs(y1 - y0);

    int err = dx / 2;
    int ystep;

    (y0 < y1)?  ystep = 1 : ystep = -1;

    for (; x0<=x1; x0++) {
        (steep) ? this->drawPixel(y0, x0, color) : this->drawPixel(x0, y0, color);
        err -= dy;
        if (err < 0) {
            y0 += ystep;
            err += dx;
        }
    }
}

/* This menthod draws vertical line*/

void STE2007::drawFastVLine(byte x, byte y, byte h, boolean color) {
    this->drawLine(x, y, x, y + h - 1, color);
}

/* This menthod draws horizantal line.*/

void STE2007::drawFastHLine(byte x, byte y, byte w, boolean color) {
    this->drawLine(x, y, x + w - 1, y, color);
}

/* This menthod draws rectangle.*/

void STE2007::drawRect(byte x, byte y, byte w, byte h, boolean color) {
    this->drawFastHLine(x, y, w, color);
    this->drawFastHLine(x, y + h - 1, w, color);
    this->drawFastVLine(x, y, h, color);
    this->drawFastVLine(x + w - 1, y, h, color);
}

/* This menthod draes circle.*/

void STE2007::drawCircle(byte x0, byte y0, int16_t r, boolean color) {
    int f = 1 - r;
    int ddF_x = 1;
    int ddF_y = -2 * r;
    int x = 0;
    int y = r;

    this->drawPixel(x0, y0 + r, color);
    this->drawPixel(x0, y0 - r, color);
    this->drawPixel(x0 + r, y0, color);
    this->drawPixel(x0 - r, y0, color);

    while (x<y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        this->drawPixel(x0 + x, y0 + y, color);
        this->drawPixel(x0 - x, y0 + y, color);
        this->drawPixel(x0 + x, y0 - y, color);
        this->drawPixel(x0 - x, y0 - y, color);
        this->drawPixel(x0 + y, y0 + x, color);
        this->drawPixel(x0 - y, y0 + x, color);
        this->drawPixel(x0 + y, y0 - x, color);
        this->drawPixel(x0 - y, y0 - x, color);
    }
}

/* This menthod draws rounded edge rectangle.*/

void STE2007::drawRoundRect(byte x, byte y, byte w, byte h, byte r, boolean color) {
    // smarter version
    this->drawFastHLine(x + r, y, w - 2 * r, color); // Top
    this->drawFastHLine(x + r, y + h - 1, w - 2 * r, color); // Bottom
    this->drawFastVLine(x, y + r, h - 2 * r, color);         // Left
    this->drawFastVLine(x + w - 1, y + r, h - 2 * r, color); // Right
    // draw four corners
    this->drawCircleHelper(x + r, y + r, r, 1, color);
    this->drawCircleHelper(x + w - r - 1, y + r, r, 2, color);
    this->drawCircleHelper(x + w - r - 1, y + h - r - 1, r, 4, color);
    this->drawCircleHelper(x + r, y + h - r - 1, r, 8, color);
}

/* This menthod draws triangle.*/

void STE2007::drawTriangle(byte x0, byte y0, byte x1, byte y1, byte x2, byte y2, boolean color) {
    this->drawLine(x0, y0, x1, y1, color);
    this->drawLine(x1, y1, x2, y2, color);
    this->drawLine(x2, y2, x0, y0, color);
}

/* This menthod draws arc.*/

void STE2007::drawCircleHelper(byte x0, byte y0, byte r, byte cornername, boolean color) {
    int f = 1 - r;
    int ddF_x = 1;
    int ddF_y = -2 * r;
    int x = 0;
    int y = r;

    while (x<y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;
        if (cornername & 0x4) {
            this->drawPixel(x0 + x, y0 + y, color);
            this->drawPixel(x0 + y, y0 + x, color);
        }
        if (cornername & 0x2) {
            this->drawPixel(x0 + x, y0 - y, color);
            this->drawPixel(x0 + y, y0 - x, color);
        }
        if (cornername & 0x8) {
            this->drawPixel(x0 - y, y0 + x, color);
            this->drawPixel(x0 - x, y0 + y, color);
        }
        if (cornername & 0x1) {
            this->drawPixel(x0 - y, y0 - x, color);
            this->drawPixel(x0 - x, y0 - y, color);
        }
    }
}

/* This menthod draws filled circle.*/

void STE2007::fillCircle(byte x0, byte y0, byte r, boolean color) {
    this->drawFastVLine(x0, y0 - r, 2 * r + 1, color);
    this->fillCircleHelper(x0, y0, r, 3, 0, color);
}

/* This menthod draws filled arc.*/

void STE2007::fillCircleHelper(byte x0, byte y0, byte r, byte cornername, byte delta, boolean color) {

    int f = 1 - r;
    int ddF_x = 1;
    int ddF_y = -2 * r;
    int x = 0;
    int y = r;

    while (x<y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        if (cornername & 0x1) {
            this->drawFastVLine(x0 + x, y0 - y, 2 * y + 1 + delta, color);
            this->drawFastVLine(x0 + y, y0 - x, 2 * x + 1 + delta, color);
        }
        if (cornername & 0x2) {
            this->drawFastVLine(x0 - x, y0 - y, 2 * y + 1 + delta, color);
            this->drawFastVLine(x0 - y, y0 - x, 2 * x + 1 + delta, color);
        }
    }
}

/* This menthod draws filled rectangle.*/

void STE2007::fillRect(byte x, byte y, byte w, byte h, boolean color) {
    for (int16_t i=x; i<x+w; i++) {
        this->drawFastVLine(i, y, h, color);
    }
}

/* This menthod draws filled round rectangle.*/

void STE2007::fillRoundRect(byte x, byte y, byte w, byte h, byte r, boolean color) {
    // smarter version
    this->fillRect(x + r, y, w - 2 * r, h, color);

    // draw four corners
    this->fillCircleHelper(x + w - r - 1, y + r, r, 1, h - 2 * r - 1, color);
    this->fillCircleHelper(x + r, y + r, r, 2, h - 2 * r - 1, color);
}

/* This menthod draws filled triangle.*/

void STE2007::fillTriangle(byte x0, byte y0, byte x1, byte y1, byte x2, byte y2, boolean color) {

    int a, b, y, last;

    // Sort coordinates by Y order (y2 >= y1 >= y0)
    if (y0 > y1) {
    SWAP(y0, y1); SWAP(x0, x1);
    }
    if (y1 > y2) {
    SWAP(y2, y1); SWAP(x2, x1);
    }
    if (y0 > y1) {
    SWAP(y0, y1); SWAP(x0, x1);
    }

    if(y0 == y2) { // Handle awkward all-on-same-line case as its own thing
    a = b = x0;
    if(x1 < a) a = x1;
    else if(x1 > b) b = x1;
    if(x2 < a) a = x2;
    else if(x2 > b) b = x2;
    this->drawFastHLine(a, y0, b - a + 1, color);
    return;
    }

    int16_t
    dx01 = x1 - x0,
    dy01 = y1 - y0,
    dx02 = x2 - x0,
    dy02 = y2 - y0,
    dx12 = x2 - x1,
    dy12 = y2 - y1,
    sa = 0,
    sb = 0;

    if(y1 == y2) last = y1; // Include y1 scanline
    else last = y1-1; // Skip it

    for(y=y0; y<=last; y++) {
    a = x0 + sa / dy01;
    b = x0 + sb / dy02;
    sa += dx01;
    sb += dx02;
    /* longhand:
    a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
    b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
    */
    if (a > b)
    SWAP(a, b);
    this->drawFastHLine(a, y, b - a + 1, color);
    }

    sa = dx12 * (y - y1);
    sb = dx02 * (y - y0);
    for(; y<=y2; y++) {
    a = x1 + sa / dy12;
    b = x0 + sb / dy02;
    sa += dx12;
    sb += dx02;
    /* longhand:
    a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
    b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
    */
    if(a > b) SWAP(a,b);
    this->drawFastHLine(a, y, b - a + 1, color);
    }
}

/* This menthod is used to draw bitmap image.*/

void STE2007::drawBitmap(byte x, byte y, const char *bitmap, byte w, byte h, boolean color) {
    for (int16_t j=0; j<h; j++) {
        for (int16_t i=0; i<w; i++ ) {
            if (PROGRAM(bitmap + i + (j/8)*w) & (1 << j%8)) {
                this->drawPixel(x + i, y + j, color);
            }
        }
    }
}

/* This menthod draws 16x32 character.*/

void STE2007::simb16x32(byte x, byte y, boolean color, byte c){
    for (byte k=0;k<4;k++){
        for (byte i=0;i<16;i++){
            byte line = PROGRAM(&(mass16x32[c][i+k*16]));
            for (byte j = 0; j<8; j++) {
                (line & 0x01) ? this->drawPixel(x + i, y + j + k * 8, color) : this->drawPixel(x + i, y + j + k * 8, !color);
                line >>= 1;
            }
        }
    }
}

/* This menthod draws 10x16 char.*/

void STE2007::simb10x16(byte x, byte y, boolean color, byte c){
    for (byte k=0;k<2;k++){
        for (byte i=0;i<10;i++){
            byte line = PROGRAM(&(mass10x16[c][i+k*10]));
            for (byte j = 0; j<8; j++) {
                (line & 0x01) ? this->drawPixel(x + i, y + j + k * 8, color) : this->drawPixel(x + i, y + j + k * 8, !color);
                line >>= 1;
            }
        }
    }
}