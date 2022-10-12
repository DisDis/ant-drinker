#ifndef _wifi_signal_indicator_h_
#define _wifi_signal_indicator_h_

#define WIFI_W_BAR (1)
#define WIFI_H (7 * WIFI_W_BAR)
#define WIFI_W (7 * WIFI_W_BAR + 2)
#define WIFI_INDICATOR_X (10)
#define WIFI_INDICATOR_Y (10)

#define b_bottleneck_w 8
#define b_bottleneck_h 4
#define b_stopper_w 12
#define b_stopper_h 5
#define bottle_h_bar 9

#define bottle_h (bottle_h_bar * 4 + b_bottleneck_h + b_stopper_h - 5)
#define bottle_w 24

extern void drawWifiRSSI(int x, int y, int rssi);
extern void drawBottle(int x, int y, unsigned char fullness);

#endif