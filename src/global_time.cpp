#include <Arduino.h>
#include "global_time.h"
#include "config.h"

// NTP
const char *ntpServer1 = "time.google.com";
const char *ntpServer2 = "clock.isc.org";
const char *ntpServer3 = "ntp.ix.ru";

GlobalTime::GlobalTime()
{
}
void GlobalTime::init()
{
    char output[80];
    struct tm timeinfo;
    Serial.println("  DateTime:");
    Serial.println("   restore ...");
    restoreLastDateTime();
    if (!getLocalTime(&timeinfo))
    {
        Serial.println(" Error: Could not obtain time info");
    }
    else
    {
        strftime(output, 80, DATATIME_FORMAT, &timeinfo);
        Serial.print(output);
        Serial.println(" ... OK");
    }

    Serial.println("   NTP ");
    ///FIXME: Enable!!
    //configTime(0, 3600 * timeZone /*+3 GMT*/, ntpServer1, ntpServer2, ntpServer3);

    if (!getLocalTime(&timeinfo))
    {
        Serial.println(" Error: Could not obtain time info");
        return;
    }
    strftime(output, 80, DATATIME_FORMAT, &timeinfo);
    Serial.print(output);
    Serial.println(" ... OK");
}

void GlobalTime::loop()
{
    if (onSave.ready())
    {
        storeDateTime();
    }
}

void GlobalTime::restoreLastDateTime()
{
    preferences.begin(globalTimeConfigKey, RO_MODE);

    time(&lastSyncDateTime);
    time_t datetimeValue = preferences.getULong(globalTimeKey, 0);
    if (lastSyncDateTime < datetimeValue)
    {
        lastSyncDateTime = datetimeValue;
        struct timeval now = {.tv_sec = lastSyncDateTime};
        settimeofday(&now, NULL);
    }
    preferences.end();
}
void GlobalTime::storeDateTime()
{
    time(&lastSyncDateTime);
    preferences.begin(globalTimeConfigKey, RW_MODE);
    preferences.putULong(globalTimeKey, lastSyncDateTime);
    preferences.end();
    Serial.println("Save current date time");
}