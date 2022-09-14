#ifndef Config_h
#define Config_h

#include <Preferences.h>
#include <Adafruit_SSD1306.h>

extern Preferences preferences;

extern Adafruit_SSD1306 display;

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define RO_MODE true
#define RW_MODE false

#define APP_VERSION "v0.1"

#define wifiSSIDKey "ssid" 
#define wifiPassKey "pass"

#define wifiConfigKey "credentials"

#define DHTTYPE DHT22
#define DHTPin 26

#define ButtonClickPin 34
#define ButtonLeftPin 35
#define ButtonRightPin 33
#define ButtonUpPin 32
#define ButtonDownPin 25

// Set LED GPIO
#define LED_PIN 13
#define LED_BUILTIN 16


class Config{
    public:
    ///default 5 min
      unsigned long automaticScreenOffTimeMs = 5*60*1000; 
      bool isBuzzerOn = true;
      unsigned long buzzerNotificationRepeatTime = 1*60*60*1000;
      Config();

    private:

};

extern Config config;

#endif