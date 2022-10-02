#include <Arduino.h>
#include "display.h"
#include "icons.h"
#include "splash.h"
#include "state.h"
#include "common.h"

#define DISPLAYOFF 0xAE
#define DISPLAYON 0xAF
#define NORMALDISPLAY 0xA6
#define INVERTDISPLAY 0xA7
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)


Adafruit_SSD1306 display = Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

DisplayDevice displayDevice;

DisplayDevice::DisplayDevice()
{
}

void DisplayDevice::init()
{
    Serial.print("  display...");
    // pinMode(SDA, PULLUP);
    // pinMode(SCL, PULLUP);
    // Wire.begin(SDA, SCL, 400000);
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    {
        Serial.println(F("SSD1306 allocation failed"));
        return;
    }
    Serial.println("OK");
    // display.flipScreenVertically();
    // display.setContrast(255);
    turnOn();
    display.setTextSize(1);
    display.setTextWrap(false);
    display.setFont();
    display.setTextColor(WHITE);
    display.clearDisplay();
    showSplashScreen();
}

void DisplayDevice::showSplashScreen()
{
    Serial.print("  Splash...");
    // https://github.com/ThingPulse/esp8266-oled-ssd1306/
    display.drawBitmap((SCREEN_WIDTH - Splash_Logo_width) / 2, 15, splash_Logo_bits, Splash_Logo_width, Splash_Logo_height, 1);
    display.setCursor((SCREEN_WIDTH - Splash_Logo_width) / 2, 0);
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
            Serial.println("Display turns off");
            turnOff();
            applicationState.currentPage = idlePage;
        }
    }
    else
    {
        if (currentMillis - applicationState.lastActionMillis < automaticScreenOffTimeMs)
        {
            applicationState.isDisplayOn = true;
            Serial.println("Display turns on");
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