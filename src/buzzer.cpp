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
}
