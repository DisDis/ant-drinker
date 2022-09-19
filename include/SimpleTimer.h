#ifndef _SimpleTimer_h
#define _SimpleTimer_h

#include <Arduino.h>

class SimpleTimer
{
public:
    SimpleTimer(){

    };
    inline void setDuration(unsigned long durationMs)
    {
        duration = durationMs;
    }
    unsigned long getDuration()
    {
        return duration;
    }
    // сколько ремени прошло с момента запуска таймера.
    inline unsigned long getActuallyPassed()
    {
        return millis() - startMs;
    }

    bool isHappened()
    {
        if (!enabled)
        {
            return false;
        }
        return (getActuallyPassed() >= duration);
    }
    void start(){
        startMs = millis();
        enabled = true;
    }

    void stop(){
        enabled = false;
    }

private:
    bool enabled = false;
    unsigned long startMs = 0;
    unsigned long duration = 0;
};

#endif