#include <Arduino.h>
#include "global_time.h"
#include "config.h"
#include "common.h"

// NTP
const char *ntpServer1 = "time.google.com";
const char *ntpServer2 = "clock.isc.org";
const char *ntpServer3 = "ntp.ix.ru";

char _hrFormatBuffer[] = "000d00h00m00s ";
char* convertSecondsToHumanReadableFormat(unsigned long timeSec = 0, bool showSeconds = 0)
{
    if (timeSec == 0)
    {
        timeSec = millis() / 1000;
    }
    unsigned long tt = timeSec;
    int day = tt / (unsigned long)(60 * 60 * 24);
    unsigned long tmpTime = tt - day * (60 * 60 * 24);
    int h = tmpTime / (60 * 60);
    tmpTime = tmpTime - h * (60 * 60);
    int m = tmpTime / (60);
    int s = tmpTime - m * 60;
    if (showSeconds)
    {
        snprintf(_hrFormatBuffer, sizeof(_hrFormatBuffer), "%03dd%02dh%02dm%02ds", day, h, m, s);
    }
    else
    {
        snprintf(_hrFormatBuffer, sizeof(_hrFormatBuffer), "%03dd%02dh%02dm", day, h, m);
    }
    return _hrFormatBuffer;
}

GlobalTime::GlobalTime()
{
}
void GlobalTime::init()
{
    char output[80];
    struct tm timeinfo;
    LOG.println("  DateTime:");
    LOG.println("   restore ...");
    load();
    if (!getLocalTime(&timeinfo))
    {
        LOG.println(" Error: Could not obtain time info");
    }
    else
    {
        strftime(output, 80, DATETIME_FORMAT, &timeinfo);
        LOG.print(output);
        LOG.println(" ... OK");
    }

    LOG.println("   NTP ");
#ifndef SKIP_NTP_SYNC
    configTime(0, 3600 * timeZone /*+3 GMT*/, ntpServer1, ntpServer2, ntpServer3);
#endif
    if (!getLocalTime(&timeinfo))
    {
        LOG.println(" Error: Could not obtain time info");
        return;
    }
    strftime(output, 80, DATETIME_FORMAT, &timeinfo);
    LOG.print(output);
    LOG.println(" ... OK");
}

void GlobalTime::loop()
{
    if (onSave.tick() && onSave.ready())
    {
        save();
    }
}

void GlobalTime::load()
{
    if (preferences.begin(globalTimeConfigKey, RO_MODE))
    {
        time(&lastSyncDateTime);
        time_t datetimeValue = preferences.getULong(globalTimeKey, 0);
        if (lastSyncDateTime < datetimeValue)
        {
            lastSyncDateTime = datetimeValue;
            struct timeval now = {.tv_sec = lastSyncDateTime};
            settimeofday(&now, NULL);
        }
    }
    preferences.end();
}
void GlobalTime::save()
{
    time(&lastSyncDateTime);
    preferences.begin(globalTimeConfigKey, RW_MODE);
    preferences.putULong(globalTimeKey, lastSyncDateTime);
    preferences.end();
    LOG.println("Save current date time");
}
