#include "config.h"
#include "telegram_notification.h"



TelegramNotification::TelegramNotification()
{
    bot = new UniversalTelegramBot(BOT_TOKEN, secured_client);
    secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
}

void TelegramNotification::setToken(const char *token)
{
    bot->updateToken(token);
}
String TelegramNotification::getToken()
{
    return bot->getToken();
}

void TelegramNotification::setChatId(const char *chatId)
{
    this->chatId = chatId;
}

String TelegramNotification::getChatId()
{
    return chatId;
}

void TelegramNotification::test()
{
    bot->sendMessage(chatId, "Bot test message", "");
}

void TelegramNotification::execute(){
    
}
