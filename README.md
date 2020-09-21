# Summery
This library can be used for any display which have STE2007 driver. 
Most commonly used LCD display with STE2007 driver is Nokia 1202 LCD.
This library was originally written for Arduino/AVR boards. I Updated this library to work with ESP8266.
I hope that this library will also work for ESP32, I have not done any testing at this point.

# Methods
Below is th list of all the availabe methods which one can use with the object.

1. #### STE2007(uint8_t _RES, uint8_t _CS, uint8_t _Data, uint8_t _Clock)
    `Object Creation with all the pins connected with the display.`
1. #### void Initialize()                                                                               
    `Initialization of the display.`
1. #### void Clear()                                                                                  
    `Clear the display.`
1. #### void Update()                                                           					   
    `Refreshing the display (outputting from the buffer).`
1. #### void drawPixel(byte x, byte y, boolean color)                                                  
    `Draw pixel by pixel.`
1. #### void fillScreen(boolean color)                                                              
    ` Fill up the screen.`
1. #### void drawChar(byte x, byte y, boolean color, unsigned char c)                                
    `Draw a character.`
1. #### void print(byte x, byte y, boolean color, char *str)                                    	     
    `Print a string.`
1. #### void print(byte x, byte y, boolean color, long num)											 
    ` Displaying a numerical value by coordinates.`
1. #### void print_1607(byte x, byte y, boolean color, char *str)            						     
    `Print a string of 1607 size`
1. #### void drawLine(byte x0, byte y0, byte x1, byte y1, boolean color)                               
    `Draw the line by coordinates.`
1. #### void drawFastVLine(byte x, byte y, byte h, boolean color)                                      
    `Draw vertical line by coordinates.`
1. #### void drawFastHLine(byte x, byte y, byte w, boolean color)                                      
    `Draw horizantol line by coordinates.`
1. #### void drawRect(byte x, byte y, byte w, byte h, boolean color)                                   
    `Draw a rectangle at coordinates x, y, height, width, color.`
1. #### void drawCircle(byte x0, byte y0, int16_t r, boolean color)                                     
    `Draw a circle centered on coordinates x, y, radius, color.`
1. #### void drawRoundRect(byte x, byte y, byte w, byte h, byte r, boolean color)                       
    `Draw a rounded rectangle by coordinates x, y, height, width, rounding radius, color.`
1. #### void drawTriangle(byte x0, byte y0, byte x1, byte y1, byte x2, byte y2, boolean color)         
    `Draw a triangle at the coordinates of the vertices x, y, x1, y1, x2, y2, color.`
1. #### void drawCircleHelper(byte x0, byte y0, byte r, byte cornername, boolean color)                
    `Draw a triangle at the coordinates of the vertices x, y, x1, y1, x2, y2, color.`
1. #### void fillCircle(byte x0, byte y0, byte r, boolean color)                                      
    `Draw a filled circle centered on coordinates x, y, radius, color.`
1. #### void fillCircleHelper(byte x0, byte y0, byte r, byte cornername, byte delta, boolean color)    
    `Draw a filled arc with coordinates x, y, radius, rotation angle, arc length, color.`
1. #### void fillRect(byte x, byte y, byte w, byte h, boolean color)                                   
    `Draw a filled rectangle at coordinates x, y, height, width, color.`
1. #### void fillRoundRect(byte x, byte y, byte w, byte h, byte r, boolean color)                      
    `Draw a filled, rounded rectangle along the x, y coordinates, height, width, radius, color.`
1. #### void fillTriangle(byte x0, byte y0, byte x1, byte y1, byte x2, byte y2, boolean color)         
    `Draw a filled triangle at the coordinates of the vertices x, y, x1, y1, x2, y2, color.`
1. #### void drawBitmap(byte x, byte y, const char bitmap, byte w, byte h, boolean color)     
    `Draw the image in coordinates x, y, with a size of 96 by 64 pixels, with color 1 (1 - pixel is on, dark).`
1. #### void simb16x32(byte x, byte y, boolean color, byte c)                                           
    `Draw a number to coordinates x, y, color 1, number, size 16x32 pixels`
1. #### void simb10x16(byte x, byte y, boolean color, byte c)                                         
    `Draw a number to coordinates x, y, color 1, number, size 10x16 pixels`
