#include "led_device.h"
#include "config.h"
#include "common.h"
#include <Arduino.h>

LEDDevice::LEDDevice()
{
}

void LEDDevice::init()
{
    LOG.print("  LED...");
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);
    LOG.println("OK");
}

void LEDDevice::turnOff()
{
    LOG.print("LED::turnOff");
    digitalWrite(LED_PIN, LOW);
}

void LEDDevice::turnOn()
{
    if (enabled)
    {
        LOG.print("LED::turnOn"); 
        digitalWrite(LED_PIN, HIGH);
    }
}