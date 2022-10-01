/*********
  Rui Santos
  Complete instructions at https://RandomNerdTutorials.com/esp32-wi-fi-manager-asyncwebserver/

  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*********/
/// TODO: Issue - I2C - Pull-Up resistor 2.4kOm (21,21 pins)
#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include "SPIFFS.h"
#include "display.h"
#include <AsyncElegantOTA.h>
#include <Wire.h>


#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <Preferences.h>
#include "wificfg.h"
#include "web_server.h"
#include "splash.h"
#include "config.h"
#include "TimerMs.h"
#include "menu_local.h"
#include "state.h"
#include "pump.h"
#include "pump_controller.h"
#include "buzzer.h"
#include "global_time.h"
#include "led_device.h"
#include "common.h"

#define SEPARATE_LINE "-------------------------------"

//#define SKIP_INIT_ALL

extern void saveTHDateToLog();

AppState applicationState;



BuzzerDevice buzzerDevice;
GlobalTime globalTime;
LEDDevice ledDevice;
SensorDevices sensorDevices(saveTHDateToLog);

WaterTank waterTank1("B1");
WaterTank waterTank2("B2");
Pump pump1(MOTORA_PWM_PIN, MOTORA_DIR0_PIN, MOTORA_DIR1_PIN);
Pump pump2(MOTORB_PWM_PIN, MOTORB_DIR0_PIN, MOTORB_DIR1_PIN);
extern void providePump1(unsigned long ml);
extern void providePump2(unsigned long ml);
PumpController pumpController1("pump1", &pump1, providePump1);
PumpController pumpController2("pump2", &pump2, providePump2);

void providePump1(unsigned long ml)
{
  Serial.printf("Pump1: %lu\n", ml);
}
void providePump2(unsigned long ml)
{
  Serial.printf("Pump2: %lu\n", ml);
}

//
TimerMs tmrButtons(150, 1, 0);

// Initialize SPIFFS
void initSPIFFS()
{
  Serial.printf("  SPIFFS...");
  if (!SPIFFS.begin(true))
  {
    Serial.println("An error has occurred while mounting SPIFFS");
  }
  Serial.println("OK: SPIFFS mounted successfully");
}

void initButtons()
{
  Serial.print("  buttons...");
  pinMode(ButtonClickPin, INPUT_PULLDOWN);
  pinMode(ButtonLeftPin, INPUT_PULLDOWN);
  pinMode(ButtonRightPin, INPUT_PULLDOWN);
  pinMode(ButtonUpPin, INPUT_PULLDOWN);
  pinMode(ButtonDownPin, INPUT_PULLDOWN);
  tmrButtons.setPeriodMode();
  Serial.println("OK");
}

void initOTA()
{
  Serial.print("  OTA...");
  AsyncElegantOTA.begin(&server);
  Serial.println("OK");
}

void initPumps()
{
  Serial.print("  Pumps...");
  pump1.init();
  pump2.init();
  pumpController1.init();
  pumpController2.init();
  Serial.println("OK");
}

void setup()
{
  // Serial port for debugging purposes
  Serial.begin(115200);
  Serial.println(SEPARATE_LINE);
  Serial.println("Init system:");
  displayDevice.init();
#ifdef SKIP_INIT_ALL
#else
  initSPIFFS();

  if (initWiFi())
  {
    applicationState.isWifi = true;
    initNormalWebServer();
  }
  else
  {
    applicationState.isWifi = false;
    initAPWiFi();
    initAPWebServer();
  }
  initOTA();
  server.begin();
  globalTime.init();
  #endif
  initButtons();
  initPumps();
  sensorDevices.init();
  ledDevice.init();
  buzzerDevice.init();
  menuSetup();

  Serial.println("System is initialized");
  Serial.println(SEPARATE_LINE);
  Serial.println();
}

unsigned long previousMillis = 0;
unsigned long currentMillis = millis();

// TODO: Remove!
char output[80];
struct tm timeinfo;
time_t now;

void loopMainPage()
{
  if (applicationState.buttonClick)
  {
    applicationState.currentPage = menuPage;
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.printf("T:%.1f H:%.1f", sensorDevices.currentTemperature, sensorDevices.currentHumidity);
  display.println();
  display.print("RRSI: ");
  display.println(WiFi.RSSI());
  time(&now);
  localtime_r(&now, &timeinfo);
  strftime(output, 80, DATETIME_FORMAT, &timeinfo);
  display.println(output);
  display.drawBitmap(64, 0, splash_Logo_bits, Splash_Logo_width, Splash_Logo_height, 1);
  display.display();
}

void loopMenuPage()
{
  menuLoop();
}

void loopIdlePage()
{
  delay(10);
  if (applicationState.buttonClick)
  {
    applicationState.currentPage = mainPage;
  }
  if (applicationState.buttonDown || applicationState.buttonDown)
  {
    applicationState.currentPage = menuPage;
  }
}

void executeCurrentState()
{
  switch (applicationState.currentPage)
  {
  case (graphTPage):
    break;
  case (mainPage):
    loopMainPage();
    break;
  case (graphHPage):
    break;
  case (menuPage):
    loopMenuPage();
    break;
  case (idlePage):
    loopIdlePage();
    break;
  default:
    applicationState.currentPage = idlePage;
  }
}

void saveTHDateToLog()
{
  // 1 min
  if (currentMillis - applicationState.lastSaveTHData > 60 * 1000)
  {
    applicationState.lastSaveTHData = currentMillis;
    // TODO: Save T&H data
  }
}


// Variables will change:
int lastStateC = LOW; // the previous state from the input pin
int currentState;     // the current reading from the input pin
int lastStateL = LOW;
int lastStateR = LOW;
int lastStateU = LOW;
int lastStateD = LOW;

void pollButtons()
{
  applicationState.buttonClick = false;
  applicationState.buttonLeft = false;
  applicationState.buttonRight = false;
  applicationState.buttonUp = false;
  applicationState.buttonDown = false;
  if (tmrButtons.tick())
  {

    currentState = digitalRead(ButtonClickPin);
    applicationState.buttonClick = lastStateC == HIGH && currentState == LOW;
    if (applicationState.buttonClick)
    {
      // Serial.println("Click");
      applicationState.lastActionMillis = currentMillis;
    }
    lastStateC = currentState;

    currentState = digitalRead(ButtonLeftPin);
    applicationState.buttonLeft = lastStateL == HIGH && currentState == LOW;
    if (applicationState.buttonLeft)
    {
      // Serial.println("Left");
      applicationState.lastActionMillis = currentMillis;
    }
    lastStateL = currentState;

    currentState = digitalRead(ButtonRightPin);
    applicationState.buttonRight = lastStateR == HIGH && currentState == LOW;
    if (applicationState.buttonRight)
    {
      // Serial.println("Right");
      applicationState.lastActionMillis = currentMillis;
    }
    lastStateR = currentState;

    currentState = digitalRead(ButtonUpPin);
    applicationState.buttonUp = lastStateU == HIGH && currentState == LOW;
    if (applicationState.buttonUp)
    {
      // Serial.println("Up");
      applicationState.lastActionMillis = currentMillis;
    }
    lastStateU = currentState;

    currentState = digitalRead(ButtonDownPin);
    applicationState.buttonDown = lastStateD == HIGH && currentState == LOW;
    if (applicationState.buttonDown)
    {
      // Serial.println("Down");
      applicationState.lastActionMillis = currentMillis;
    }
    lastStateD = currentState;
  }
}

void loop()
{
  currentMillis = millis();
  pollButtons();
  displayDevice.detectTimeOff();
  buzzerDevice.loop();
  executeCurrentState();
  sensorDevices.poll();
  globalTime.loop();
}