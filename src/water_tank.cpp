#include "water_tank.h"
#include "config.h"
#include "common.h"

WaterBottle::WaterBottle(const char *id)
{
    this->_id = id;
}

void WaterBottle::reset()
{
    value = capacity;
}

void WaterBottle::init()
{
    Serial.print("  WaterBottle...");
    Serial.print(_id);
    Serial.print("    ");
    load();
    Serial.println("    OK");
}

#define KEY_CAPACITY "capacity"
#define KEY_VALUE "value"
#define KEY_ENABLED "enabled"

void WaterBottle::save()
{
    Serial.println("save");
    preferences.begin(_id, RW_MODE);
    preferences.putUInt(KEY_CAPACITY, capacity);
    preferences.putFloat(KEY_VALUE, value);
    preferences.putBool(KEY_ENABLED, enabled);
    preferences.end();
}

void WaterBottle::load()
{
    Serial.println("load");
    if (preferences.begin(_id, RO_MODE))
    {
        capacity = preferences.getUInt(KEY_CAPACITY, capacity);
        value = preferences.getFloat(KEY_VALUE, value);
        enabled = preferences.getBool(KEY_ENABLED, enabled);
    }
    preferences.end();
}

WaterBottleController::WaterBottleController(WaterBottle *wtank)
{
    this->wtank = wtank;
}
void WaterBottleController::provide(unsigned long ml)
{
    if (!wtank->enabled) {
        return;
    }
    Serial.printf("Water - %s:  %lu\n",wtank->_id, ml);
    wtank->value -= ml;
    if (wtank->value < 0)
    {
        wtank->value = 0;
    }
    wtank->save();
};
void WaterBottleController::init()
{
}

void WaterBottleController::execute(){

}