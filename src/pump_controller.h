#ifndef _PUMP_CONTROLLER_H
#define _PUMP_CONTROLLER_H

#include "TimerMs.h"
#include "SimpleTimer.h"
#include "DateTimeTimer.h"
#include "pump.h"

enum PumpMode
{
    NotCalibratedMode = 1,
    CalibratingMode,
    ReadyMode,
    WorkingMode,
    TurnOnMode
};

typedef void (*ON_FINISH_WORK_CALLBACK)(unsigned long ml);

class PumpController
{
private:
    const char *_id;
    bool isEnabled = false;
    Pump* pump;
    // Как часто? (1мин .. 1день ... 1 неделя .. 2х недель)
    DateTimeTimer tmrAction = DateTimeTimer();
    // Продолжительность
    SimpleTimer tmrWork = SimpleTimer();
    // Калибровка
    TimerMs tmrCalibration = TimerMs(100 * 1000, 0, 1);
    // Скорость мл жидкости в миллисекунду
    float speedMlPerMs = 1.0;
    // 0..255 [1-100%]
    unsigned char power = 255;
    // Left/Right
    bool isInverted = false;
    // Сколько нужно мл за 1 раз
    float mlAtTime = 10.0;
    // Сколько мл получилось при калибровке(100 секунд)
    float calibration100SecMl = 0.0;
    PumpMode mode = NotCalibratedMode;
    void _executeStartWork();
    void _executeStopWork();
    void _executeCalibrationProcess();
    void _runPump();
    void _stopPump();
    ON_FINISH_WORK_CALLBACK finishWorkCallback = nullptr;

public:
    PumpController(const char *id, Pump* pump, ON_FINISH_WORK_CALLBACK finishWorkCallback);
    PumpMode calculateMode();
    void execute();
    void stopImmediate();
    void emergencyStop();
    void startImmediate();
    void startCalibration();
    void finishCalibration();
    void init(){};
    friend class Config;
};

#endif