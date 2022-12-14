#ifndef _Buzzer_h_
#define _Buzzer_h_

#include "TimerMs.h"

class BuzzerDevice
{
private:
    // unsigned char doNotdisturb = 21;
    bool sound = false;
    char count = 0;
    TimerMs interval = TimerMs(1000, 0, 0);
    TimerMs durationSound = TimerMs(250, 0, 1);
    void _turnOnSound();

public:
    bool isEnabled = false;
    BuzzerDevice();
    void init();
    void loop();
    void turnOn(char count = 1);
    void turnOff();
    void load();
    void save();
};

#endif