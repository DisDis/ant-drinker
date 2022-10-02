#include <Arduino.h>
#include "config.h"
#include "common.h"
#include "notifications.h"

#define NOTIFICATIONS_PREF_NAME "notifications"

Notifications notifications;

Notifications::Notifications()
{
}

void Notifications::init()
{
    Serial.print("  Notifications...");
    Serial.print("    ");
    load();
    Serial.println("    OK");
}

void Notifications::save()
{
    Serial.println("save");
    preferences.begin(NOTIFICATIONS_PREF_NAME, RW_MODE);
    // preferences.putBool(KEY_isEnabled, _enabled);
    // preferences.putFloat(KEY_mlAtTime, mlAtTime);
    // preferences.putBool(KEY_isInverted, isInverted);
    // preferences.putFloat(KEY_speedMlPerMs, speedMlPerMs);
    // preferences.putUChar(KEY_power, power);
    // preferences.putULong(KEY_INTERVAL_SEC, tmrAction.getDuration());
    // preferences.putULong(KEY_START_DATETIME_SEC, tmrAction.getStartTime());
    preferences.end();
}

void Notifications::load()
{
    Serial.println("load");
    if (preferences.begin(NOTIFICATIONS_PREF_NAME, RO_MODE))
    {
        //     _enabled = preferences.getBool(KEY_isEnabled, _enabled);
        //     mlAtTime = preferences.getFloat(KEY_mlAtTime, mlAtTime);
        //     isInverted = preferences.getBool(KEY_isInverted, isInverted);
        //     speedMlPerMs = preferences.getFloat(KEY_speedMlPerMs, speedMlPerMs);
        //     power = preferences.getUChar(KEY_power, power);
        //     tmrAction.setDuration(preferences.getULong(KEY_INTERVAL_SEC, tmrAction.getDuration()));
        //     tmrAction.setStartTime(preferences.getULong(KEY_START_DATETIME_SEC, tmrAction.getStartTime()));
    }
    preferences.end();
}

