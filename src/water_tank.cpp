#include "water_tank.h"

WaterTank::WaterTank(const char *id)
{
    this->_id = id;
}

void WaterTank::reset()
{
    value = capacity;
}

void WaterTank::load()
{
}
void WaterTank::save()
{
}