#ifndef _COMMON_h
#define _COMMON_h

#include "config.h"
#include <Preferences.h>
#include "pump_controller.h"
#include "global_time.h"
#include "pump.h"
#include "pump_controller.h"
#include "buzzer.h"
#include "water_tank.h"
#include "led_device.h"
#include "sensor_devices.h"
#include "notifications.h"

extern char *getUptimeStr();
extern Preferences preferences;
extern unsigned long currentMillis;
extern BuzzerDevice buzzerDevice;
extern GlobalTime globalTime;
extern Notifications notifications;
extern WaterBottle waterBottle1;
extern WaterBottle waterBottle2;
extern WaterBottleController waterBottleController1;
extern WaterBottleController waterBottleController2;
extern Pump pump1;
extern PumpController pumpController1;
#ifdef MOTORB_ENABLED
extern Pump pump2;
extern PumpController pumpController2;
#endif
extern LEDDevice ledDevice;
extern SensorDevices sensorDevices;

#endif