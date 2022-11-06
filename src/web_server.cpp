#include <Arduino.h>
#include "SPIFFS.h"
#include "web_server.h"
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include "config.h"
#include "common.h"

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Set LED GPIO
const int ledPin = 13;

// Stores LED state
String ledState;

// Search for parameter in HTTP POST request
const char *PARAM_INPUT_1 = "ssid";
const char *PARAM_INPUT_2 = "pass";

// Read File from SPIFFS
String readFile(fs::FS &fs, const char *path)
{
  Serial.printf("Reading file: %s\r\n", path);

  File file = fs.open(path);
  if (!file || file.isDirectory())
  {
    Serial.println("- failed to open file for reading");
    return String();
  }

  String fileContent;
  while (file.available())
  {
    fileContent = file.readStringUntil('\n');
    break;
  }
  return fileContent;
}

// Write file to SPIFFS
void writeFile(fs::FS &fs, const char *path, const char *message)
{
  Serial.printf("Writing file: %s\r\n", path);

  File file = fs.open(path, FILE_WRITE);
  if (!file)
  {
    Serial.println("- failed to open file for writing");
    return;
  }
  if (file.print(message))
  {
    Serial.println("- file written");
  }
  else
  {
    Serial.println("- frite failed");
  }
}

// Replaces placeholder with LED state value
String processor(const String &var)
{
  if (var == "STATE")
  {
    if (digitalRead(ledPin))
    {
      ledState = "ON";
    }
    else
    {
      ledState = "OFF";
    }
    return ledState;
  }
  else if (var == "TBOT_TOKEN")
  {
    return notifications.telegramN.getToken();
  }
  else if (var == "TBOT_CHAT_ID")
  {
    return notifications.telegramN.getChatId();
  }
  else if (var == "MQTT_USER")
  {
    return notifications.mqttN.getUser();
  }
  else if (var == "MQTT_PASS")
  {
    return notifications.mqttN.getPass();
  }
  if (var == "MQTT_SERVER")
  {
    return notifications.mqttN.getServer();
  }
  if (var == "MQTT_PORT")
  {
    return String(notifications.mqttN.getPort());
  }

  return String();
}

void initNormalWebServer()
{
  Serial.print("  WebServer...");
  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/index.html", "text/html", false, processor); });
  server.serveStatic("/", SPIFFS, "/");

  // Route to set GPIO state to HIGH
  server.on("/on", HTTP_GET, [](AsyncWebServerRequest *request)
            {
      digitalWrite(ledPin, HIGH);
      request->send(SPIFFS, "/index.html", "text/html", false, processor); });

  // Route to set GPIO state to LOW
  server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request)
            {
      digitalWrite(ledPin, LOW);
      request->send(SPIFFS, "/index.html", "text/html", false, processor); });

  server.on("/telegram_update", HTTP_POST, [](AsyncWebServerRequest *request)
            {
      int params = request->params();
     
      for(int i=0;i<params;i++){
        AsyncWebParameter* p = request->getParam(i);
        if(p->isPost()){
          if (p->name() == "tbot_token") {
            notifications.telegramN.setToken(p->value().c_str());
          }
          if (p->name() == "tbot_chat_id") {
            notifications.telegramN.setChatId(p->value().c_str());
          }
        }
      }
      notifications.save();
      request->send(200, "text/plain", "Done."); });
  server.on("/mqtt_update", HTTP_POST, [](AsyncWebServerRequest *request)
            {
      int params = request->params();
     
      for(int i=0;i<params;i++){
        AsyncWebParameter* p = request->getParam(i);
        if(p->isPost()){
          if (p->name() == "mqtt_user") {
            notifications.mqttN.setUser(p->value().c_str());
          }
          if (p->name() == "mqtt_pass") {
            notifications.mqttN.setPass(p->value().c_str());
          }
          if (p->name() == "mqtt_port") {
            notifications.mqttN.setPort( atoi(p->value().c_str()) );
          }
          if (p->name() == "mqtt_server") {
            notifications.mqttN.setServer( p->value().c_str());
          }
        }
      }
      notifications.save();
      request->send(200, "text/plain", "Done."); });

  Serial.println("OK");
}
void initAPWebServer()
{
  Serial.println("  WebServer AP...");
  // Web Server Root URL
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/wifimanager.html", "text/html"); });

  server.serveStatic("/", SPIFFS, "/");

  server.on("/wifi_update", HTTP_POST, [](AsyncWebServerRequest *request)
            {
      int params = request->params();
      preferences.begin(wifiConfigKey, RW_MODE);
      for(int i=0;i<params;i++){
        AsyncWebParameter* p = request->getParam(i);
        if(p->isPost()){
          // HTTP POST ssid value
          if (p->name() == PARAM_INPUT_1) {
            String ssid = p->value().c_str();
            Serial.print("SSID set to: ");
            Serial.println(ssid);
            preferences.putString(wifiSSIDKey, ssid.c_str());
          }
          // HTTP POST pass value
          if (p->name() == PARAM_INPUT_2) {
            String pass = p->value().c_str();
            Serial.print("Password set to: ");
            Serial.println(pass);
            preferences.putString(wifiPassKey, pass.c_str());
          }
        }
      }
      preferences.end();
      request->send(200, "text/plain", "Done. ESP will restart, connect to your router.");
      delay(3000);
      ESP.restart(); });
  Serial.println("OK");
}
