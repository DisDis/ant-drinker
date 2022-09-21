#ifndef _Config_h
#define _Config_h

#include <Preferences.h>
#include <Adafruit_SSD1306.h>
#include "pump_controller.h"

extern Preferences preferences;
extern unsigned long currentMillis;

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define RO_MODE true
#define RW_MODE false

#define APP_VERSION "v0.3"

#define wifiSSIDKey "ssid" 
#define wifiPassKey "pass"

#define wifiConfigKey "credentials"

//#define SENSOR_TEM_HYM 
#define DHTTYPE DHT22
#define DHTPin 26

#define ButtonClickPin 34
#define ButtonLeftPin 35
#define ButtonRightPin 33
#define ButtonUpPin 32
#define ButtonDownPin 25

#define MOTORA_PWM_PIN 31
#define MOTORA_DIR0_PIN 29
#define MOTORA_DIR1_PIN 30

#define MOTORB_PWM_PIN 25
#define MOTORB_DIR0_PIN 27
#define MOTORB_DIR1_PIN 26

#define MOTORS_ON_PIN 28

#define BUZZER_PIN 24

#define DATATIME_FORMAT "%d-%m-%y, %H:%M:%S"

// Set LED GPIO
#define LED_PIN 13
#define LED_BUILTIN 16

struct WaterTank{
  int capacity = 100;
};

class Config{
    public:
    ///default 5 min
      unsigned long automaticScreenOffTimeMs = 5*60*1000; 
      bool isBuzzerOn = true;
      unsigned long buzzerNotificationRepeatTime = 1*60*60*1000;
      WaterTank wTank1;
      WaterTank wTank2;
      Config();
      void load(PumpController* pc);
    private:

};

extern Config appConfig;

#endif