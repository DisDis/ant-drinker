#ifndef _WATER_TANK_h
#define _WATER_TANK_h

class WaterTank
{
private:
    const char *_id;

public:
    unsigned int capacity = 0;
    float value = 0;

    WaterTank(const char *id);
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

    void load();
    void save();
};

#endif