#include "config.h"
#include "common.h"
#include "display.h"
#include "wifi_signal_indicator.h"

// 5e282d
#define b_border_color 0x5925
// eae8e8
#define b_empty_color 0xE73C
// ffba7a
#define b_stopper_color 0xFDAE
// 94d1ea
#define b_water_color 0x8E7C

void drawBottle(int x, int y, unsigned char percent)
{
    display.fillRect(x + (bottle_w - b_stopper_w) / 2 + 1, y + 1, b_stopper_w - 1, b_stopper_h - 1, b_stopper_color);
    display.drawRect(x + (bottle_w - b_stopper_w) / 2, y, b_stopper_w, b_stopper_h, b_border_color);
    y += b_stopper_h - 1;
    display.fillRect(x + (bottle_w - b_bottleneck_w) / 2 + 1, y + 1, b_bottleneck_w - 1, b_bottleneck_h - 1, b_empty_color);
    display.drawRect(x + (bottle_w - b_bottleneck_w) / 2, y, b_bottleneck_w, b_bottleneck_h, b_border_color);
    y += b_bottleneck_h - 1;
    // 100-75
    display.fillRoundRect(x + 1, y + 1, bottle_w - 1, bottle_h_bar - 1, 2, percent >= 75 ? b_water_color : b_empty_color);
    display.drawRoundRect(x, y, bottle_w, bottle_h_bar, 2, b_border_color);
    // 75 - 50
    y += bottle_h_bar - 1;
    display.fillRoundRect(x + 1, y + 1, bottle_w - 1, bottle_h_bar - 1, 2, percent >= 50 ? b_water_color : b_empty_color);
    display.drawRoundRect(x, y, bottle_w, bottle_h_bar, 2, b_border_color);
    // Line
    display.drawLine(x + 2, y + 5,
                     x + 2 + 6, y + 5, b_border_color);
    display.writePixel(x + 2 + 6 + 2, y + 5, b_border_color);
    // 50 - 25
    y += bottle_h_bar - 1;
    display.fillRoundRect(x + 1, y + 1, bottle_w - 1, bottle_h_bar - 1, 2, percent >= 25 ? b_water_color : b_empty_color);
    display.drawRoundRect(x, y, bottle_w, bottle_h_bar, 2, b_border_color);
    // 25 - 5
    y += bottle_h_bar - 1;
    display.fillRoundRect(x + 1, y + 1, bottle_w - 1, bottle_h_bar - 1, 2, percent >= 5 ? b_water_color : b_empty_color);
    display.drawRoundRect(x, y, bottle_w, bottle_h_bar, 2, b_border_color);
}

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