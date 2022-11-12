#include "buzzer.h"
#include "config.h"
#include <Arduino.h>
#include "common.h"

BuzzerDevice::BuzzerDevice()
{
}

void BuzzerDevice::init()
{
    LOG.print("  Buzzer...");
    pinMode(BUZZER_PIN, OUTPUT);
    digitalWrite(BUZZER_PIN, LOW);
    LOG.println("OK");
    LOG.print("    state...");
    load();
}

void BuzzerDevice::loop()
{
    if (!sound)
    {
        return;
    }
    if (durationSound.tick() && durationSound.ready())
    {
        digitalWrite(BUZZER_PIN, LOW);
    }
    if (interval.tick())
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
    if (isEnabled)
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

#define BUZZER_PREF_NAME "sensors"
#define KEY_isEnabled "ENABLED"

void BuzzerDevice::save()
{
    LOG.println("save");
    preferences.begin(BUZZER_PREF_NAME, RW_MODE);
    preferences.putBool(KEY_isEnabled, isEnabled);
    // preferences.putFloat(KEY_mlAtTime, mlAtTime);
    // preferences.putBool(KEY_isInverted, isInverted);
    // preferences.putFloat(KEY_speedMlPerMs, speedMlPerMs);
    // preferences.putUChar(KEY_power, power);
    preferences.end();
}
void BuzzerDevice::load()
{
    LOG.println("load");
    if (preferences.begin(BUZZER_PREF_NAME, RO_MODE))
    {
        isEnabled = preferences.getBool(KEY_isEnabled, isEnabled);
        // mlAtTime = preferences.getFloat(KEY_mlAtTime, mlAtTime);
        // isInverted = preferences.getBool(KEY_isInverted, isInverted);
        // speedMlPerMs = preferences.getFloat(KEY_speedMlPerMs, speedMlPerMs);
        // power = preferences.getUChar(KEY_power, power);
    }
    preferences.end();
}
