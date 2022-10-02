#ifndef _SENSOR_DEVICES_H
#define _SENSOR_DEVICES_H
#include "TimerMs.h"
#include "config.h"
#ifdef SENSOR_T_H_DHT
#include "DHT.h"
#include "Adafruit_Sensor.h"
#endif
#ifdef SENSOR_T_H_AM2320
#include "Adafruit_AM2320.h"
#endif


typedef void (*ON_WATER_LEVEL_CHANGED_CALLBACK)(void);

class SensorDevices{
    public:
        float currentTemperature = 0.0;
        float currentHumidity = 0.0;
        bool isWater1Low = false;
        SensorDevices(ON_WATER_LEVEL_CHANGED_CALLBACK waterLevelChangedCallback);
        void init();
        void poll();
        void save();
        void load();
    private:
        TimerMs tmrTempHumi = TimerMs(2000, 1, 0);
    ON_WATER_LEVEL_CHANGED_CALLBACK waterLevelChangedCallback = nullptr;
        #ifdef SENSOR_T_H_DHT
        DHT sensorTH = DHT(SENSOR_T_H_PIN, DHTTYPE);
        #endif
        #ifdef SENSOR_T_H_AM2320
        // AM2320
        Adafruit_AM2320 sensorTH = Adafruit_AM2320();
        #endif
        
};

#endif