#ifndef _LED_DEVICE_h_
#define _LED_DEVICE_h_

class LEDDevice{
 public:
    bool enabled = false;
    LEDDevice();
    void init();
    void turnOn();
    void turnOff();
};

#endif