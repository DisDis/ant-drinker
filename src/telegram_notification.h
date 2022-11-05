#ifndef _TELEGRAM_NOTIFICATIONS_h
#define _TELEGRAM_NOTIFICATIONS_h

#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

class TelegramNotification{
    private:
        WiFiClientSecure secured_client;
        UniversalTelegramBot* bot;
        String chatId = "";
    public:
        TelegramNotification();
        void setToken(const char* token);
        void setChatId(const char* token);
        String getToken();
        String getChatId();
        void test();
};

#endif