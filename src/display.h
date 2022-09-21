
#ifndef _Display_h_
#define _Display_h_

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "config.h"
#include <Wire.h>

// for 128x64 displays:
extern Adafruit_SSD1306 display;


class DisplayDevice
{
private:
    void showSplashScreen();

public:
    DisplayDevice();
    void init();

    void turnOn();
    void turnOff();
    void detectTimeOff();
};

#endif