#include <Arduino.h>
#include "sensor_devices.h"

SensorDevices::SensorDevices(ON_WATER_LEVEL_CHANGED_CALLBACK waterLevelChangedCallback)
{
    this->waterLevelChangedCallback = waterLevelChangedCallback;
}

void SensorDevices::init()
{
    Serial.println("  sensors:");
    pinMode(SENSOR_T_H_PIN, INPUT);
#ifdef SENSOR_T_H_DHT
    Serial.print("    DHT...");

    sensorTH.begin();
    Serial.println("OK");
#endif
#ifdef SENSOR_T_H_AM2320
    Serial.print("    AM2320..");
    if (!sensorTH.begin())
    {
        Serial.println(F("am2320 failed"));
    }
    else
    {
        Serial.println("OK");
    }
#endif
    Serial.print("    Low Water...");
    pinMode(SENSOR_LOW_WATER, INPUT);
    Serial.println("OK");
}

void SensorDevices::poll()
{
    if (!tmrTempHumi.tick())
    {
        return;
    }
    currentTemperature = sensorTH.readTemperature();
    currentHumidity = sensorTH.readHumidity();
}