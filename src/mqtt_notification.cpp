#include "config.h"
#include "common.h"
#include "mqtt_notification.h"

void callback(char *topic, byte *payload, unsigned int length)
{
    LOG.print("Message arrived [");
    LOG.print(topic);
    LOG.print("] ");
    for (int i = 0; i < length; i++)
    {
        LOG.print((char)payload[i]);
    }
    LOG.println();

    // Switch on the LED if an 1 was received as first character
    if ((char)payload[0] == '1')
    {
        ledDevice.turnOn();
    }
    else
    {
        ledDevice.turnOff();
    }
}

MQTTNotification::MQTTNotification()
{
    randomSeed(micros());
    clientId = "AntDrinker-" + String(random(0xffff), HEX);
    mqtt = PubSubClient(client);
}

void MQTTNotification::init()
{
    mqtt.setServer(server.c_str(), port);
    mqtt.setCallback(callback);
}

void MQTTNotification::setPort(int port)
{
    this->port = port;
}
void MQTTNotification::setServer(const char *server)
{
    this->server = server;
}
void MQTTNotification::setUser(const char *user)
{
    this->user = user;
}
void MQTTNotification::setPass(const char *pass)
{
    this->pass = pass;
}

int MQTTNotification::getPort()
{
    return this->port;
}
String MQTTNotification::getServer()
{
    return server;
}
String MQTTNotification::getUser()
{
    return user;
}
String MQTTNotification::getPass()
{
    return pass;
}

void MQTTNotification::test()
{
    if (!mqtt.connected())
    {
        _connect();
    }
    mqtt.publish("outTopic", "hello world");
}

void MQTTNotification::_connect()
{
    mqtt.connect(clientId.c_str(), user.c_str(), pass.c_str());
}

void MQTTNotification::execute()
{
    mqtt.loop();
}
