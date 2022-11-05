#ifndef _GLOBAL_TIME_H_
#define _GLOBAL_TIME_H_
#include <time.h>
#include "TimerMs.h"

extern char* convertSecondsToHumanReadableFormat(unsigned long timeSec, bool showSeconds);

class GlobalTime
{
public:
    GlobalTime();
    void init();

    inline char getTimeZone()
    {
        return timeZone;
    }
    friend class Config;
    void loop();
    void load();
    void save();

private:
    TimerMs onSave = TimerMs(60 * 60 * 1000, 1, 0);
    char timeZone = +3;
    time_t lastSyncDateTime = 0;
};



#endif