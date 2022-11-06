#include <Arduino.h>
#include "config.h"
#include "common.h"
#include "notifications.h"

#define NOTIFICATIONS_PREF_NAME "notifications"
#define KEY_TELEGRAM_TOKEN "telg_token"
#define KEY_TELEGRAM_CHAT_ID "telg_chatId"

#define KEY_MQTT_USER "mqtt_user"
#define KEY_MQTT_PASS "mqtt_pass"
#define KEY_MQTT_PORT "mqtt_port"
#define KEY_MQTT_SERVER "mqtt_server"

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
    preferences.putString(KEY_TELEGRAM_CHAT_ID, telegramN.getChatId());

    preferences.putString(KEY_MQTT_USER, mqttN.getUser());
    preferences.putString(KEY_MQTT_PASS, mqttN.getPass());
    preferences.putInt(KEY_MQTT_PORT, mqttN.getPort());
    preferences.putString(KEY_MQTT_SERVER, mqttN.getServer());

    preferences.end();
}

void Notifications::load()
{
    Serial.println("load");
    if (preferences.begin(NOTIFICATIONS_PREF_NAME, RO_MODE))
    {
        telegramN.setToken(preferences.getString(KEY_TELEGRAM_TOKEN, String(BOT_TOKEN)).c_str());
        telegramN.setChatId(preferences.getString(KEY_TELEGRAM_CHAT_ID).c_str());

        mqttN.setUser(preferences.getString(KEY_MQTT_USER).c_str());
        mqttN.setPass(preferences.getString(KEY_MQTT_PASS).c_str());
        mqttN.setPort(preferences.getInt(KEY_MQTT_PORT, 1883));
        mqttN.setServer(preferences.getString(KEY_MQTT_SERVER).c_str());
    }
    preferences.end();
}

void Notifications::test()
{
    telegramN.test();
}

void Notifications::execute()
{
    telegramN.execute();
    mqttN.execute();
}