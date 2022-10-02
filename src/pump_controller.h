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
extern const char *PumpModeName[];
// = { "Not Calibrated", "Calibrating", "Ready", "Working", "TurnOn" };

typedef void (*ON_FINISH_WORK_CALLBACK)(unsigned long ml);

class PumpController
{
private:
    const char *_id;
    bool _enabled = false;
    Pump *pump;

    // Продолжительность
    SimpleTimer tmrWork = SimpleTimer();
    // Калибровка
    TimerMs tmrCalibration = TimerMs(100 * 1000, 0, 1);

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
    // Как часто? (1мин .. 1день ... 1 неделя .. 2х недель)
    DateTimeTimer tmrAction = DateTimeTimer();
    inline bool getEnabled(){ return _enabled;}
    void setEnabled(bool newValue);
    // Сколько нужно мл за 1 раз
    float mlAtTime = 10.0;
    // Left/Right
    bool isInverted = false;

    // Скорость мл жидкости в миллисекунду
    float speedMlPerMs = 1.0;
    // 0..255 [1-100%]
    unsigned char power = 255;

    PumpController(const char *id, Pump *pump, ON_FINISH_WORK_CALLBACK finishWorkCallback);
    PumpMode calculateMode();
    inline PumpMode getMode() { return mode; };
    void execute();
    void stopImmediate();
    void emergencyStop();
    void startImmediate();
    void startCalibration();
    void finishCalibration();
    void init();
    void load();
    void save();
    void saveLastAction();
};

#endif