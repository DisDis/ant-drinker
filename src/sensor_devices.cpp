#include <Arduino.h>
#include "sensor_devices.h"
#include "common.h"

SensorDevices::SensorDevices(ON_WATER_LEVEL_CHANGED_CALLBACK waterLevelChangedCallback)
{
    this->waterLevelChangedCallback = waterLevelChangedCallback;
}

void SensorDevices::init()
{
    LOG.println("  sensors:");
    pinMode(SENSOR_T_H_PIN, INPUT);
#ifdef SENSOR_T_H_DHT
    LOG.print("    DHT...");

    sensorTH.begin();
    LOG.println("OK");
#endif
#ifdef SENSOR_T_H_AM2320
    LOG.print("    AM2320..");
    // LOG.print("LIBRARY: ");
    // LOG.print(AM232X_LIB_VERSION);
    if (!sensorTH.begin())
    {
        LOG.println(F(" AM2320 failed"));
    }
    else
    {
        // sensorTH.wakeUp();
        // LOG.println("GET SENSOR INFO (experimental)");
        // LOG.print("  Model:\t");
        // LOG.println(sensorTH.getModel());
        // LOG.print("  Version:\t");
        // LOG.println(sensorTH.getVersion());
        // LOG.print("  DevId:\t");
        // LOG.println(sensorTH.getDeviceID());

        // LOG.println();
        // LOG.println("GET REGISTERS (experimental)");
        // LOG.print(" Status:\t");
        // LOG.println(sensorTH.getStatus());
        // LOG.print("  UserA:\t");
        // LOG.println(sensorTH.getUserRegisterA());
        // LOG.print("  UserB:\t");
        // LOG.println(sensorTH.getUserRegisterB());
        LOG.println("OK");
    }
#endif
    LOG.print("    Low Water...");
    pinMode(SENSOR_LOW_WATER, INPUT);
    LOG.println("OK");
    LOG.print("    state...");
    load();
}

void SensorDevices::poll()
{
    if (!tmrTempHumi.tick())
    {
        return;
    }
#ifdef SENSOR_T_H_AM2320
    // int status = sensorTH.read();
    // switch (status)
    // {
    // case AM232X_OK:
    //     currentTemperature = sensorTH.getTemperature();
    //     currentHumidity = sensorTH.getHumidity();
    //     // LOG.println("OK");
    //     break;
    // default:
    //     LOG.print("AM2320 error: ");
    //     LOG.println(status);
    //     break;
    // }
    if (hasReadT)
    {
        float value = sensorTH.readTemperature();
        if (!isnan(value))
        {
            currentTemperature = value;
        }
    }
    else
    {
        float value = sensorTH.readHumidity();
        if (!isnan(value))
        {
            currentHumidity = value;
        }
    }
    hasReadT = !hasReadT;
#endif
#ifdef SENSOR_T_H_DHT
    currentTemperature = sensorTH.readTemperature();
    currentHumidity = sensorTH.readHumidity();
#endif
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
    LOG.println("save");
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
    LOG.println("load");
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