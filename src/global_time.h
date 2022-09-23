#ifndef _GLOBAL_TIME_H_
#define _GLOBAL_TIME_H_
#include <time.h>
#include "TimerMs.h"

class GlobalTime
{
public:
    GlobalTime();
    void init();

    void storeDateTime();
    inline char getTimeZone()
    {
        return timeZone;
    }
    friend class Config;
    void loop();

private:
    TimerMs onSave = TimerMs(60 * 60 * 1000, 1, 0);
    void restoreLastDateTime();
    char timeZone = +3;
    time_t lastSyncDateTime = 0;
};

#endif