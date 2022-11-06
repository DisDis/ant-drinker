#ifndef _MQTT_NOTIFICATIONS_h
#define _MQTT_NOTIFICATIONS_h

#include <WiFiClient.h>
#include <PubSubClient.h>

class MQTTNotification{
    private:
        WiFiClient client;
        PubSubClient mqtt;
        unsigned int port = 1883;
        String user;
        String pass;
        String clientId;
        String server;
        void _connect();
    public:
        MQTTNotification();
        void setPort(int port);
        void setServer(const char* server);
        void setUser(const char* user);
        void setPass(const char* pass);
        
        int getPort();
        String getServer();
        String getUser();
        String getPass();

        void test();
        void execute();
        void init();
};

#endif