#include "config.h"
#include <Preferences.h>
#include <Adafruit_SSD1306.h>
#include "pump_controller.h"
#include "global_time.h"
#include "pump.h"
#include "pump_controller.h"
#include "buzzer.h"
#include "water_tank.h"
#include "led_device.h"
#include "sensor_devices.h"


extern Preferences preferences;
extern unsigned long currentMillis;
extern BuzzerDevice buzzerDevice;
extern GlobalTime globalTime;

extern WaterTank waterTank1;
extern WaterTank waterTank2;
extern Pump pump1;
extern Pump pump2;
extern PumpController pumpController1;
extern PumpController pumpController2;
extern LEDDevice ledDevice;
extern SensorDevices sensorDevices;

class Config{
    public:
      Config();
      void load(PumpController* pc);
    private:

};

extern Config appConfig;

