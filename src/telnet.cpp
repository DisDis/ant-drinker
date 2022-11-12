#include <config.h>
#include <common.h>
#if defined(GENERAL_DEBUG) && GENERAL_DEBUG_TELNET

WiFiServer telnetServer(TELNET_PORT);                       // telnet сервер
WiFiClient telnet;                                          // обработчик событий telnet клиента
bool telnetGreetingShown = false;                           // признак "показано приветствие в telnet"


void handleTelnetClient()
{
  if (telnetServer.hasClient())
  {
    if (!telnet || !telnet.connected())
    {
      if (telnet)
      {
        telnet.stop();                                      // клиент отключился
        telnetGreetingShown = false;
      }
      telnet = telnetServer.available();                    // готов к подключению нового клиента
    }
    else
    {
      telnetServer.available().stop();                      // один клиент уже подключен, блокируем подключение нового
      telnetGreetingShown = false;
    }
  }

  if (telnet && telnet.connected() && telnet.available())
  {
    if (!telnetGreetingShown)
    {
      telnet.println("Connection to the device via telnet protocol established\n-------");
      telnetGreetingShown = true;
    }
  }
}

#endif
