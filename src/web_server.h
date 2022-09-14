#ifndef WebServer_h
#define WebServer_h

#include <Arduino.h>
#include "SPIFFS.h"
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

extern AsyncWebServer server;

String readFile(fs::FS &fs, const char * path);
void writeFile(fs::FS &fs, const char * path, const char * message);

void initAPWebServer();
void initNormalWebServer();


#endif