#ifndef _wifi_signal_indicator_h_
#define _wifi_signal_indicator_h_

#define WIFI_W_BAR (1)
#define WIFI_H (7 * WIFI_W_BAR)
#define WIFI_W (7 * WIFI_W_BAR + 2)
#define WIFI_INDICATOR_X (10)
#define WIFI_INDICATOR_Y (10)

extern void drawWifiRSSI(int x, int y, int rssi);

#endif