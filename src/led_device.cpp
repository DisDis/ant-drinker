#include "led_device.h"
#include "config.h"
#include <Arduino.h>

LEDDevice::LEDDevice()
{
}

void LEDDevice::init()
{
    Serial.print("  LED...");
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);
    Serial.println("OK");
}

void LEDDevice::turnOff()
{
    digitalWrite(LED_PIN, LOW);
}

void LEDDevice::turnOn()
{
    if (enabled)
    {
        digitalWrite(LED_PIN, HIGH);
    }
}