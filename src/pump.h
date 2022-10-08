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
    Pump(unsigned char pwmPin, unsigned char pwmChannel, unsigned char directionPin0, unsigned char directionPin1)
    {
        this->pwmPin = pwmPin;
        this->pwmChannel = pwmChannel;
        this->directionPin0 = directionPin0;
        this->directionPin1 = directionPin1;
    }

    void init()
    {
        pinMode(directionPin0, OUTPUT);
        pinMode(directionPin1, OUTPUT);
        pinMode(pwmPin, OUTPUT);

        if (ledcSetup(pwmChannel, frequency, resolution) == 0)
        {
            Serial.println("ledc channel was not configured.");
        }
        ledcAttachPin(pwmPin, pwmChannel);
        stop();
    }
    /// @brief
    /// @param power Power 0-255
    /// @param isInverted
    void start(unsigned char power, bool isInverted)
    {
        ledcWrite(pwmChannel, power);
        digitalWrite(directionPin0, isInverted ? HIGH : LOW);
        digitalWrite(directionPin1, isInverted ? LOW : HIGH);
    }

    void stop()
    {
        ledcWrite(pwmChannel, 0);
        digitalWrite(directionPin0, LOW);
        digitalWrite(directionPin1, LOW);
    }
};

#endif