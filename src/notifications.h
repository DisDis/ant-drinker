#ifndef _NOTIFICATIONS_h
#define _NOTIFICATIONS_h

#include "telegram_notification.h"
#include "mqtt_notification.h"

class Notifications
{
private:

public:
    TelegramNotification telegramN;
    MQTTNotification mqttN;
    Notifications();
    void load();
    void save();
    void init();
    void test();
    void execute();
};

#endif