#include "display.h"
#include <Arduino.h>
#include "icons.h"
#include "splash.h"
#include "state.h"

#define DISPLAYOFF 0xAE
#define DISPLAYON 0xAF
#define NORMALDISPLAY 0xA6
#define INVERTDISPLAY 0xA7

Adafruit_SSD1306 display = Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

DisplayDevice::DisplayDevice()
{
}

void DisplayDevice::init()
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
    showSplashScreen();
}

void DisplayDevice::showSplashScreen()
{
    Serial.print("  Splash...");
    // https://github.com/ThingPulse/esp8266-oled-ssd1306/
    display.drawBitmap((SCREEN_WIDTH - Splash_Logo_width) / 2, 0, splash_Logo_bits, Splash_Logo_width, Splash_Logo_height, 1);
    display.setCursor((SCREEN_WIDTH - Splash_Logo_width) / 2 + Splash_Logo_width, 0);
    display.println(APP_VERSION);
    display.display();
    Serial.println("OK");
}

void DisplayDevice::detectTimeOff()
{
    if (applicationState.isDisplayOn)
    {
        if (currentMillis - applicationState.lastActionMillis > automaticScreenOffTimeMs)
        {
            applicationState.isDisplayOn = false;
            display.clearDisplay();
            turnOff();
            applicationState.currentPage = idlePage;
        }
    }
    else
    {
        if (currentMillis - applicationState.lastActionMillis < automaticScreenOffTimeMs)
        {
            applicationState.isDisplayOn = true;
            turnOn();
            applicationState.currentPage = mainPage;
        }
    }
}

void DisplayDevice::turnOn(void)
{
    display.ssd1306_command(DISPLAYON);
}

void DisplayDevice::turnOff(void)
{
    display.ssd1306_command(DISPLAYOFF);
}

// void DisplayDevice::invertDisplay(void) {
//   display.sendCommand(INVERTDISPLAY);
// }

// void DisplayDevice::normalDisplay(void) {
//   display.sendCommand(NORMALDISPLAY);
// }