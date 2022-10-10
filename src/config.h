#ifndef _Config_h
#define _Config_h

#define HOSTNAME_WIFI "ant-drinker"

#define SKIP_NTP_SYNC

// #define SENSOR_T_H_DHT
#define SENSOR_T_H_AM2320
#define DHTTYPE DHT22

#define SENSOR_T_H_PIN 26
#define SENSOR_LOW_WATER 15

#define SCREEN_WIDTH 160 // OLED display width, in pixels
#define SCREEN_HEIGHT 80 // OLED display height, in pixels

#define RO_MODE true
#define RW_MODE false

#define SCAN_I2C
#define I2C_FREQ 100000

#define wifiSSIDKey "ssid"
#define wifiPassKey "pass"

#define wifiConfigKey "credentials"
#define globalTimeConfigKey "datetime"

#define globalTimeKey "dt"

#define PUMP1_ID "pump1"
#define PUMP2_ID "pump2"

#define ButtonClickPin 25
#define ButtonLeftPin 32
#define ButtonRightPin 33
#define ButtonUpPin 34
#define ButtonDownPin 35

//#define MOTORB_ENABLED

#define MOTORA_PWM_PIN 12
#define MOTORA_DIR0_PIN 0
#define MOTORA_DIR1_PIN 14

#ifdef MOTORB_ENABLED
 #define MOTORB_PWM_PIN 1
 #define MOTORB_DIR0_PIN 19
 #define MOTORB_DIR1_PIN 3
#endif

#define MOTORS_ON_PIN 4

#define BUZZER_PIN 2

#define DATETIME_FORMAT "%d-%m-%y, %H:%M:%S"
#define MENU_DATE_FORMAT "Date: %d-%m-%y"
#define MENU_TIME_FORMAT "Time: %H:%M:%S"

// Set LED GPIO
#define LED_PIN 13
#define LED_BUILTIN 16

#endif