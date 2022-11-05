#ifndef _NOTIFICATIONS_h
#define _NOTIFICATIONS_h

#include "telegram_notification.h"

class Notifications
{
private:

public:
    TelegramNotification telegramN;
    Notifications();
    void load();
    void save();
    void init();
    void test();
};

#endif