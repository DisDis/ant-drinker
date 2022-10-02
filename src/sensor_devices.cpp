#include <Arduino.h>
#include "sensor_devices.h"
#include "common.h"

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
    Serial.print("    state...");
    load();
}

void SensorDevices::poll()
{
    if (!tmrTempHumi.tick())
    {
        return;
    }
    currentTemperature = sensorTH.readTemperature();
    currentHumidity = sensorTH.readHumidity();
    int value = digitalRead(SENSOR_LOW_WATER) == HIGH;
    if (value != isWater1Low)
    {
        isWater1Low = value;
        if (waterLevelChangedCallback)
        {
            waterLevelChangedCallback();
        }
    }
}

#define SENSOR_DEVICES_PREF_NAME "sensors"

void SensorDevices::save()
{
    Serial.println("save");
    preferences.begin(SENSOR_DEVICES_PREF_NAME, RW_MODE);
    // preferences.putBool(KEY_isEnabled, isEnabled);
    // preferences.putFloat(KEY_mlAtTime, mlAtTime);
    // preferences.putBool(KEY_isInverted, isInverted);
    // preferences.putFloat(KEY_speedMlPerMs, speedMlPerMs);
    // preferences.putUChar(KEY_power, power);
    preferences.end();
}
void SensorDevices::load()
{
    Serial.println("load");
    if (preferences.begin(SENSOR_DEVICES_PREF_NAME, RO_MODE))
    {
        // isEnabled = preferences.getBool(KEY_isEnabled,isEnabled);
        // mlAtTime = preferences.getFloat(KEY_mlAtTime, mlAtTime);
        // isInverted = preferences.getBool(KEY_isInverted, isInverted);
        // speedMlPerMs = preferences.getFloat(KEY_speedMlPerMs, speedMlPerMs);
        // power = preferences.getUChar(KEY_power, power);
    }
    preferences.end();
}