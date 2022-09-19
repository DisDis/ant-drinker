#ifndef _PUMP_CONTROLLER_H
#define _PUMP_CONTROLLER_H

#include "TimerMs.h"
#include "SimpleTimer.h"

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
    unsigned long actionPeriodMs = 1 * 60 * 60 * 1000;

    // Как часто? (1мин .. 1день ... 1 неделя .. 2х недель)
    TimerMs tmrAction = TimerMs(1 * 60 * 60 * 1000, 0, 1);
    // Продолжительность
    SimpleTimer tmrWork = SimpleTimer();
    // Калибровка
    TimerMs tmrCalibration = TimerMs(100 * 1000, 0, 1);
    // Скорость мл жидкости в миллисекунду
    float speedMlPerMs = 1.0;
    // 1-100%
    unsigned char powerPwm = 100;
    // Left/Right
    bool isInverted = false;
    // Сколько нужно мл за 1 раз
    float mlAtTime = 10.0;
    // Сколько мл получилось при калибровке(100 секунд)
    float calibration100SecMl = 0.0;
    PumpMode mode = NotCalibratedMode;
    unsigned char directionPin0 = 1;
    unsigned char directionPin1 = 2;
    unsigned char pwmPin = 3;
    void _executeStartWork();
    void _executeStopWork();
    void _executeCalibrationProcess();
    void _runPump();
    void _stopPump();
    ON_FINISH_WORK_CALLBACK finishWorkCallback = nullptr;

public:
    PumpController(const char *id, ON_FINISH_WORK_CALLBACK finishWorkCallback);
    PumpMode calculateMode();
    void execute();
    void stopImmediate();
    void emergencyStop();
    void startImmediate();
    void startCalibration();
    void finishCalibration();
};

#endif