#ifndef _PUMP_H
#define _PUMP_H

#if defined(ESP32) || defined(LIBRETUYA)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#else
#error Platform not supported
#endif

class Pump
{
private:
    unsigned char directionPin0;
    unsigned char directionPin1;
    unsigned char pwmPin;
    unsigned char pwmChannel;
    //частота ШИМ
    const int frequency = 50000;

    //разрешение ШИМа (в битах)
    const int resolution = 8;

public:
    Pump(unsigned char pwmPin, unsigned char pwmChannel, unsigned char directionPin0, unsigned char directionPin1);
    void init();
    /// @brief
    /// @param power Power 0-255
    /// @param isInverted
    void start(unsigned char power, bool isInverted);
    void stop();
};

#endif