#ifndef _WATER_TANK_h
#define _WATER_TANK_h

class WaterBottle
{
private:
    const char *_id;
    friend class WaterBottleController;
public:
    unsigned int capacity = 1000;
    float value = 1000;

    WaterBottle(const char *id);
    bool enabled = false;
    inline unsigned int getCapacity()
    {
        return capacity;
    }
    inline float getValue()
    {
        return value;
    }
    void reset();
    void init();
    void load();
    void save();
};

class WaterBottleController
{
public:
    WaterBottle *wtank;
    WaterBottleController(WaterBottle *wtank);
    void provide(unsigned long ml);
    void init();
    void execute();
};

#endif