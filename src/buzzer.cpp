#include "buzzer.h"
#include "config.h"
#include <Arduino.h>

BuzzerDevice::BuzzerDevice()
{
}

void BuzzerDevice::init()
{
    Serial.print("  Buzzer...");
    pinMode(BUZZER_PIN, OUTPUT);
    digitalWrite(BUZZER_PIN, LOW);
    Serial.println("OK");
}

void BuzzerDevice::loop()
{
    if (!sound)
    {
        return;
    }
    if (durationSound.ready())
    {
        digitalWrite(BUZZER_PIN, LOW);
    }
    if (interval.ready())
    {
        count--;
        if (count <= 0)
        {
            turnOff();
            return;
        }
        durationSound.restart();
        _turnOnSound();
    }
}

void BuzzerDevice::_turnOnSound()
{
    durationSound.start();
    digitalWrite(BUZZER_PIN, HIGH);
}

void BuzzerDevice::turnOn(char count)
{
    if (enabled)
    {
        sound = true;
        if (count <= 0)
        {
            count = 1;
        }
        this->count = count;
        interval.start();
        _turnOnSound();
    }
}
void BuzzerDevice::turnOff()
{
    sound = false;
    count = 0;
    digitalWrite(BUZZER_PIN, LOW);
    durationSound.stop();
    interval.stop();
}
