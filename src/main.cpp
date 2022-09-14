/*********
  Rui Santos
  Complete instructions at https://RandomNerdTutorials.com/esp32-wi-fi-manager-asyncwebserver/

  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*********/

#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include "SPIFFS.h"
#include <AsyncElegantOTA.h>
#include <Wire.h>
#include "SSD1306Wire.h"
#include "Adafruit_Sensor.h"
#include "Adafruit_AM2320.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DHT.h"
#include <Preferences.h>
#include "wificfg.h"
#include "web_server.h"
#include "splash.h"
#include "config.h"
#include "TimerMs.h"

#define SEPARATE_LINE "-------------------------------"

// for 128x64 displays:
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// AM2320
// Adafruit_AM2320 am2320 = Adafruit_AM2320();
// DHT22/AM2302
DHT am2320 = DHT(DHTPin, DHTTYPE);

// NTP
const char *ntpServer = "time.google.com";

//
TimerMs tmrButtons(50, 1, 0);

// Set LED GPIO
const int ledPin = 13;

enum ScreenType
{
  graphT = 1,
  main,
  graphH,
  menu
};

class State
{
public:
  bool isDisplayOn = true;
  unsigned long lastActionMillis = millis();
  unsigned long lastTHUpdate = 0;
  unsigned long lastWaterLevelUpdate = 0;
  unsigned long lastSaveTHData = 0;
  unsigned long lastBuzzerOn = 0;
  float currentTemperature = 0.0;
  float currentHumidity = 0.0;
  ScreenType screenType = main;
  // status
  bool isWifi = false;
};
State state;

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

void initSensors()
{
  Serial.println("  sensors:");
  // Serial.print("    AM2320..");
  // if (!am2320.begin())
  // {
  //   Serial.println(F("am2320 failed"));
  // } else { Serial.println("OK");}

  // DHT
  Serial.print("    DHT...");
  pinMode(DHTPin, INPUT);
  am2320.begin();
  Serial.println("OK");
}

void initButtons()
{
  Serial.print("  buttons...");
  pinMode(ButtonClickPin, INPUT_PULLDOWN);
  pinMode(ButtonLeftPin, INPUT_PULLDOWN);
  pinMode(ButtonRightPin, INPUT_PULLDOWN);
  pinMode(ButtonUpPin, INPUT_PULLDOWN);
  pinMode(ButtonDownPin, INPUT_PULLDOWN);
  Serial.println("OK");
}

void initDisplay()
{
  Serial.print("  display...");
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println(F("SSD1306 allocation failed"));
    return;
  }
  Serial.println("OK");
  // display.flipScreenVertically();
  // display.setContrast(255);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.clearDisplay();
}

void initOTA()
{
  Serial.print("  OTA...");
  AsyncElegantOTA.begin(&server);
  Serial.println("OK");
}

void printTime()
{

  struct tm time;

  if (!getLocalTime(&time))
  {
    Serial.println("Could not obtain time info");
    return;
  }

  Serial.println("\n---------TIME----------");

  Serial.print("Number of years since 1900: ");
  Serial.println(time.tm_year);

  Serial.print("month, from 0 to 11: ");
  Serial.println(time.tm_mon);

  Serial.print("day, from 1 to 31: ");
  Serial.println(time.tm_mday);

  Serial.print("hour, from 0 to 23: ");
  Serial.println(time.tm_hour);

  Serial.print("minute, from 0 to 59: ");
  Serial.println(time.tm_min);

  Serial.print("second, from 0 to 59: ");
  Serial.println(time.tm_sec);
}

void initShowSplashScreen()
{
  Serial.print("  Splash...");
  // https://github.com/ThingPulse/esp8266-oled-ssd1306/
  display.drawBitmap((128 - Splash_Logo_width) / 2, 0, splash_Logo_bits, Splash_Logo_width, Splash_Logo_height, 1);
  display.setCursor((128 - Splash_Logo_width) / 2 + Splash_Logo_width, 0);
  display.println(APP_VERSION);
  display.display();
  Serial.println("OK");
}

void initLED()
{
  Serial.print("  LED...");
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  Serial.println("OK");
}
void initBuzzer()
{
  Serial.print("  Buzzer...");
  // pinMode(ledPin, OUTPUT);
  // digitalWrite(ledPin, LOW);
  Serial.println("OK");
}

void setup()
{
  // Serial port for debugging purposes
  Serial.begin(115200);
  Serial.println(SEPARATE_LINE);
  Serial.println("Init system:");
  initDisplay();
  initShowSplashScreen();
  initButtons();
  initSensors();
  initSPIFFS();
  initLED();
  initBuzzer();
  tmrButtons.setPeriodMode();

  if (initWiFi())
  {
    state.isWifi = true;
    initNormalWebServer();
  }
  else
  {
    state.isWifi = false;
    initAPWiFi();
    initAPWebServer();
  }
  initOTA();
  server.begin();
  configTime(0, 3600, ntpServer);
  printTime();

  Serial.println("System is initialized");
  Serial.println(SEPARATE_LINE);
  Serial.println();
  delay(1000);
  display.clearDisplay();
}

unsigned long currentMillis = millis();
unsigned long previousMillis = 0;

#define DISPLAYOFF 0xAE
#define DISPLAYON 0xAF
#define NORMALDISPLAY 0xA6
#define INVERTDISPLAY 0xA7
void displayOn(void)
{
  display.ssd1306_command(DISPLAYON);
}

void displayOff(void)
{
  display.ssd1306_command(DISPLAYOFF);
}

// void invertDisplay(void) {
//   display.sendCommand(INVERTDISPLAY);
// }

// void normalDisplay(void) {
//   display.sendCommand(NORMALDISPLAY);
// }

void controlDisplayTimeOff()
{
  if (state.isDisplayOn)
  {
    if (currentMillis - state.lastActionMillis > config.automaticScreenOffTimeMs)
    {
      state.isDisplayOn = false;
      displayOff();
      state.screenType = main;
    }
  }
  else
  {
    if (currentMillis - state.lastActionMillis < config.automaticScreenOffTimeMs)
    {
      state.isDisplayOn = true;
      displayOn();
    }
  }
}

void showMainPage()
{
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Main page");
  display.print("T: ");
  display.printf("%.1f", state.currentTemperature);
  display.print("H: ");
  display.printf("%.1f", state.currentHumidity);
  display.println();
  // display.drawStringf(0,10,"%.1f",(state.currentTemperature));

  display.drawBitmap(64, 0, splash_Logo_bits, Splash_Logo_width, Splash_Logo_height, 1);
  display.display();
}

void showCurrentState()
{
  if (!state.isDisplayOn)
  {
    return;
  }
  switch (state.screenType)
  {
  case (graphT):
    break;
  case (main):
    showMainPage();
    break;
  case (graphH):
    break;
  case (menu):
    break;
  default:
    state.screenType = menu;
  }
}

void saveTHDateToLog()
{
  // 1 min
  if (currentMillis - state.lastSaveTHData > 60 * 1000)
  {
    state.lastSaveTHData = currentMillis;
    // TODO: Save T&H data
  }
}

void pollSensors()
{
  if (currentMillis - state.lastTHUpdate > 2000)
  {
    state.lastTHUpdate = currentMillis;
    state.currentTemperature = am2320.readTemperature();
    state.currentHumidity = am2320.readHumidity();
    saveTHDateToLog();
    // Serial.print("T: ");
    // Serial.printf("%.1f", state.currentTemperature);
    // Serial.print("H: ");
    // Serial.printf("%.1f", state.currentHumidity);
    // Serial.println();
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
  if (tmrButtons.tick())
  {

    currentState = digitalRead(ButtonClickPin);
    if (lastStateC == HIGH && currentState == LOW)
    {
      Serial.println("Click");
      state.lastActionMillis = currentMillis;
    }
    lastStateC = currentState;

    currentState = digitalRead(ButtonLeftPin);
    if (lastStateL == HIGH && currentState == LOW)
    {
      Serial.println("Left");
      state.lastActionMillis = currentMillis;
    }
    lastStateL = currentState;

    currentState = digitalRead(ButtonRightPin);
    if (lastStateR == HIGH && currentState == LOW)
    {
      Serial.println("Right");
      state.lastActionMillis = currentMillis;
    }
    lastStateR = currentState;

    currentState = digitalRead(ButtonUpPin);
    if (lastStateU == HIGH && currentState == LOW)
    {
      Serial.println("Up");
      digitalWrite(ledPin, HIGH);
      state.lastActionMillis = currentMillis;
    }
    lastStateU = currentState;

    currentState = digitalRead(ButtonDownPin);
    if (lastStateD == HIGH && currentState == LOW)
    {
      Serial.println("Down");
      digitalWrite(ledPin, LOW);
      state.lastActionMillis = currentMillis;
    }
    lastStateD = currentState;
  }
}

void loop()
{
  currentMillis = millis();
  controlDisplayTimeOff();
  showCurrentState();
  pollSensors();
  pollButtons();
  // menuLoop();
}