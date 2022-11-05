#include <Arduino.h>
#include "config.h"
#include "common.h"
#include "notifications.h"

#define NOTIFICATIONS_PREF_NAME "notifications"
#define KEY_TELEGRAM_TOKEN "telg_token"
#define KEY_TELEGRAM_CHAT_ID "telg_chatId"

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
    preferences.putString(KEY_TELEGRAM_TOKEN, telegramN.getToken());
    preferences.putString(KEY_TELEGRAM_CHAT_ID,telegramN.getChatId());
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
        telegramN.setToken(preferences.getString(KEY_TELEGRAM_TOKEN, String(BOT_TOKEN)).c_str());
        telegramN.setChatId(preferences.getString(KEY_TELEGRAM_CHAT_ID).c_str());
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

void Notifications::test()
{
    telegramN.test();
}
