
#ifndef _Display_h_
#define _Display_h_

#include <Adafruit_GFX.h>
// #include <Adafruit_SSD1306.h>
#include <Adafruit_ST7735_DoubleBuffer.h>
#include "config.h"
#include <SPI.h>

#define TFT_CS 5
#define TFT_RST 16
#define TFT_DC 17

extern Adafruit_ST7735 display;
void drawImage(int16_t x, int16_t y, const uint16_t *bitmap, int16_t w,
               int16_t h/*, uint16_t bg*/);

class DisplayDevice
{
private:
    /// default 5 min
    unsigned long automaticScreenOffTimeMs = 5 * 60 * 1000;
    void showSplashScreen();

public:
    DisplayDevice();
    void init();

    void turnOn();
    void turnOff();
    void detectTimeOff();
    void swapBuffer();
};

extern DisplayDevice displayDevice;

#endif