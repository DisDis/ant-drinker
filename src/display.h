
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
    ///default 5 min
    unsigned long automaticScreenOffTimeMs = 5*60*1000; 
    void showSplashScreen();
public:
    DisplayDevice();
    void init();

    void turnOn();
    void turnOff();
    void detectTimeOff();
};

extern DisplayDevice displayDevice;

#endif