#include "config.h"
#include "common.h"
#include "display.h"
#include "wifi_signal_indicator.h"

void drawWifiRSSI(int x, int y, int rssi)
{
    display.drawLine(x, y + WIFI_H + 1, x + WIFI_W, y + WIFI_H + 1, ST7735_WHITE);
    display.drawLine(x, y + WIFI_H, x + WIFI_W - 1, y - 1, ST7735_WHITE);
    display.drawLine(x + WIFI_W, y - 1, x + WIFI_W, y + WIFI_H, ST7735_WHITE);
    unsigned int color = ST7735_GREEN;
    byte count = 7;
    if (rssi <= -90)
    {
        color = 0xE0E4;
        count = 1;
    }
    else if (rssi <= -80)
    {
        color = 0xEB23;
        count = 2;
    }
    else if (rssi <= -70)
    {
        color = 0xEBE3;
        count = 3;
    }
    else if (rssi <= -67)
    {
        color = 0xF523;
        count = 4;
    }
    else if (rssi <= -60)
    {
        color = 0xDE82;
        count = 5;
    }
    else if (rssi <= -50)
    {
        color = 0x8627;
        count = 6;
    }
    else if (rssi <= -30)
    {
        color = 0x1549; // BEST
        count = 7;
    }
    else
    {
        color = 0x1549; // BEST
        count = 7;
    }
    char index = 0;
    while (count > 0)
    {
        char h = WIFI_H - (7 - index) * WIFI_W_BAR;
        display.fillRect(x + 2, y + WIFI_H - h + 1, WIFI_W_BAR, h, color);
        x += WIFI_W_BAR;
        count--;
        index++;
    }
}