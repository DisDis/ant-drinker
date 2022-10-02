#ifndef _DateTimeTimer_h
#define _DateTimeTimer_h

#include <Arduino.h>

class DateTimeTimer
{
public:
    DateTimeTimer(){

    };
    inline void setDuration(unsigned long durationSec)
    {
        duration = durationSec;
    }
    unsigned long getDuration()
    {
        return duration;
    }
    // сколько ремени прошло с момента запуска таймера.
    inline unsigned long getActuallyPassed()
    {
        time(&now);
        return now - startDT;
    }

    // перезапустить
    inline void restart()
    {
        start();
    }

    time_t getStartTime()
    {
        return startDT;
    }

    void setStartTime(time_t newValue)
    {
        startDT = newValue;
    }

    bool isHappened()
    {
        if (!enabled)
        {
            return false;
        }
        return (getActuallyPassed() >= duration);
    }
    void start()
    {
        time(&startDT);
        enabled = true;
    }

    void resume()
    {
        enabled = true;
    }

    void stop()
    {
        enabled = false;
    }

    // TODO: PUBLIC FOR MENU :(
    // Время в секундах
    unsigned long duration = 5 * 60;

private:
    bool enabled = false;
    // Время в секундах.
    time_t startDT = 0;

    time_t now;
};

#endif