#include <Arduino.h>
#include "display.h"
#include "icons.h"
#include "splash.h"
#include "state.h"
#include "common.h"
#include "Version.h"

#define DISPLAYOFF 0xAE
#define DISPLAYON 0xAF
#define NORMALDISPLAY 0xA6
#define INVERTDISPLAY 0xA7
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)

// Adafruit_SSD1306 display = Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_ST7735 display = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

DisplayDevice displayDevice;

DisplayDevice::DisplayDevice()
{
}

void drawImage(int16_t x, int16_t y, const uint16_t *bitmap, int16_t w,
               int16_t h /*, uint16_t bg*/)
{
    uint16_t row, col, buffidx = 0;
    for (row = 0; row < h; row++)
    { // For each scanline...
        for (col = 0; col < w; col++)
        { // For each pixel...
            // To read from Flash Memory, pgm_read_XXX is required.
            // Since image is stored as uint16_t, pgm_read_word is used as it uses 16bit address
            display.drawPixel(x + col, y + row, pgm_read_word(bitmap + buffidx));
            buffidx++;
        } // end pixel
    }
}

void DisplayDevice::init()
{
    Serial.print("  display...");

    // Use this initializer if you're using a 1.8" TFT
    // tft.initR(INITR_BLACKTAB);   // initialize a ST7735S chip, black tab

    // Use this initializer (uncomment) if you're using a 1.44" TFT
    // tft.initR(INITR_144GREENTAB);   // initialize a ST7735S chip, black tab

    // Use this initializer (uncomment) if you're using a 0.96" 180x60 TFT
    // FIX: https://arduino.ru/forum/apparatnye-voprosy/st7735s-096-80x160-spi-krivo-rabotaet#comment-565630
    /**
    _height = ST7735_TFTWIDTH_80;
    _width = ST7735_TFTHEIGHT_160;
    displayInit(Rcmd2green);
    _colstart = 26;
    _rowstart = 1;
     *
    */
    display.initR(INITR_GREENTAB /*INITR_MINI160x80*/); // initialize a ST7735S chip, mini display

    // Use this initializer (uncomment) if you're using a 1.54" 240x240 TFT
    // tft.init(240, 240);   // initialize a ST7789 chip, 240x240 pixels

    // OR use this initializer (uncomment) if using a 2.0" 320x240 TFT:
    // tft.init(240, 320);           // Init ST7789 320x240

    // OR use this initializer (uncomment) if using a 1.14" 240x135 TFT:
    // tft.init(135, 240);           // Init ST7789 240x135

    // OR use this initializer (uncomment) if using a 1.47" 174x320 TFT:
    // tft.init(174, 320);           // Init ST7789 174x320
    display.invertDisplay(1);
    Serial.println("OK");
    // display.flipScreenVertically();
    // display.setContrast(255);
    turnOn();
    display.setTextSize(1);
    display.setTextWrap(false);
    display.setFont();
    display.setTextColor(ST77XX_WHITE);
    display.setRotation(3);
    display.fillScreen(ST77XX_BLACK);
    showSplashScreen();
}

void DisplayDevice::showSplashScreen()
{
    Serial.print("  Splash...");
    // drawImage((SCREEN_WIDTH - Splash_Logo_width) / 2, 15, splash_Logo_bits, Splash_Logo_width, Splash_Logo_height);
    display.drawBitmap(64, SCREEN_HEIGHT - Splash_Logo_height, splash_Logo_bits, Splash_Logo_width, Splash_Logo_height, ST7735_WHITE);
    display.setCursor(0, 0);
    display.printf("v%s, %s", VERSION, BUILD_TIMESTAMP);

    // display.display();
    Serial.println("OK");
}

void DisplayDevice::detectTimeOff()
{
    if (applicationState.isDisplayOn)
    {
        if (currentMillis - applicationState.lastActionMillis > automaticScreenOffTimeMs)
        {
            applicationState.isDisplayOn = false;
            display.fillScreen(ST77XX_BLACK);
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
    display.enableDisplay(true);
}

void DisplayDevice::turnOff(void)
{
    display.enableDisplay(false);
}
