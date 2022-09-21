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
    unsigned char directionPin0 = 1;
    unsigned char directionPin1 = 2;
    unsigned char pwmPin = 3;
    //частота ШИМ
    const int frequency = 30000;

    //разрешение ШИМа (в битах)
    const int resolution = 8;

public:
    Pump(unsigned char pwmPin, unsigned char directionPin0, unsigned char directionPin1)
    {
        this->pwmPin = pwmPin;
        this->directionPin0 = directionPin0;
        this->directionPin1 = directionPin1;
    }

    void init()
    {
        pinMode(directionPin0, OUTPUT);
        pinMode(directionPin1, OUTPUT);
        ledcSetup(pwmPin, frequency, resolution);
    }
    /// @brief
    /// @param power Power 0-255
    /// @param isInverted
    void start(unsigned char power, bool isInverted)
    {
        ledcWrite(pwmPin, power);
        digitalWrite(directionPin0, isInverted ? HIGH : LOW);
        digitalWrite(directionPin1, isInverted ? LOW : HIGH);
    }

    void stop()
    {
        ledcWrite(pwmPin, 0);
        digitalWrite(directionPin0, LOW);
        digitalWrite(directionPin1, LOW);
    }
};

#endif