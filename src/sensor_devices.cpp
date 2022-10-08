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
    // Serial.print("LIBRARY: ");
    // Serial.print(AM232X_LIB_VERSION);
    if (!sensorTH.begin())
    {
        Serial.println(F(" AM2320 failed"));
    }
    else
    {
        // sensorTH.wakeUp();
        // Serial.println("GET SENSOR INFO (experimental)");
        // Serial.print("  Model:\t");
        // Serial.println(sensorTH.getModel());
        // Serial.print("  Version:\t");
        // Serial.println(sensorTH.getVersion());
        // Serial.print("  DevId:\t");
        // Serial.println(sensorTH.getDeviceID());

        // Serial.println();
        // Serial.println("GET REGISTERS (experimental)");
        // Serial.print(" Status:\t");
        // Serial.println(sensorTH.getStatus());
        // Serial.print("  UserA:\t");
        // Serial.println(sensorTH.getUserRegisterA());
        // Serial.print("  UserB:\t");
        // Serial.println(sensorTH.getUserRegisterB());
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
#ifdef SENSOR_T_H_AM2320
    // int status = sensorTH.read();
    // switch (status)
    // {
    // case AM232X_OK:
    //     currentTemperature = sensorTH.getTemperature();
    //     currentHumidity = sensorTH.getHumidity();
    //     // Serial.println("OK");
    //     break;
    // default:
    //     Serial.print("AM2320 error: ");
    //     Serial.println(status);
    //     break;
    // }
    currentTemperature = sensorTH.readTemperature();
    currentHumidity = sensorTH.readHumidity();
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