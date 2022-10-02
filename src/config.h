#ifndef _Config_h
#define _Config_h

#define SKIP_NTP_SYNC

#define SENSOR_T_H_DHT
//#define SENSOR_T_H_AM2320
#define DHTTYPE DHT22

#define SENSOR_T_H_PIN 26
#define SENSOR_LOW_WATER 15

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define RO_MODE true
#define RW_MODE false

#define APP_VERSION "v0.6"

#define wifiSSIDKey "ssid" 
#define wifiPassKey "pass"

#define wifiConfigKey "credentials"
#define globalTimeConfigKey "datetime"

#define globalTimeKey "dt"

#define PUMP1_ID "pump1"
#define PUMP2_ID "pump2"

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

#define DATETIME_FORMAT "%d-%m-%y, %H:%M:%S"
#define MENU_DATE_FORMAT "Date: %d-%m-%y"
#define MENU_TIME_FORMAT "Time: %H:%M:%S"

// Set LED GPIO
#define LED_PIN 13
#define LED_BUILTIN 16





#endif