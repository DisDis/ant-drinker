#include "TimerMs.h"
#include "SimpleTimer.h"
#include "pump_controller.h"
#include "config.h"
#include "common.h"

const char *PumpModeName[] = {"Not Calibrated", "Calibrating", "Ready", "Working", "TurnOn"};

void PumpController::_runPump()
{
    Serial.println("_runPump");
    pump->start(power, isInverted);
}
void PumpController::_stopPump()
{
    Serial.println("_stopPump");
    pump->stop();
}

PumpMode PumpController::calculateMode()
{
    PumpMode result = NotCalibratedMode;
    if (calibration100SecMl == 0.0 || speedMlPerMs == 0.0)
    {
        result = NotCalibratedMode;
    }
    else
    {
        result = ReadyMode;
    }
    return result;
}

PumpController::PumpController(const char *id, Pump *pump, ON_FINISH_WORK_CALLBACK finishWorkCallback)
{
    this->_id = id;
    this->pump = pump;
    this->finishWorkCallback = finishWorkCallback;
    this->mode = calculateMode();
}
void PumpController::_executeStartWork()
{
    if (!tmrAction.isHappened())
    {
        return;
    }
    if (mode != ReadyMode)
    {
        return;
    }
    Serial.println("_executeStartWork");
    tmrAction.restart();
    mode = WorkingMode;
    tmrWork.setDuration(truncf(mlAtTime / speedMlPerMs));
    tmrWork.start();
    _runPump();
}
void PumpController::_executeStopWork()
{
    if (!tmrWork.isHappened())
    {
        return;
    }
    Serial.println("_executeStopWork");
    if (mode != WorkingMode)
    {
        tmrWork.stop();
        return;
    }
    mode = ReadyMode;
    _stopPump();
    unsigned long realMl = speedMlPerMs * tmrWork.getActuallyPassed();
    tmrWork.stop();
    if (finishWorkCallback)
    {
        finishWorkCallback(realMl);
    }
    saveLastAction();
    globalTime.save();
}

void PumpController::execute()
{
    // Pump disabled
    if (!_enabled)
    {
        return;
    }
    _executeStartWork();
    _executeStopWork();
    _executeCalibrationProcess();
}

void PumpController::stopImmediate()
{
    Serial.println("stopImmediate");
    _stopPump();
    tmrAction.restart();
    tmrWork.stop();
    tmrCalibration.stop();
    if (mode == TurnOnMode)
    {
        mode = calculateMode();
    }
}
void PumpController::startImmediate()
{
    Serial.println("startImmediate");
    tmrAction.restart();
    mode = TurnOnMode;
    tmrWork.stop();
    _runPump();
}

void PumpController::emergencyStop()
{
    Serial.println("emergencyStop");
    mode = NotCalibratedMode;
    _stopPump();
    tmrCalibration.stop();
    tmrWork.stop();
}

void PumpController::startCalibration()
{
    Serial.println("startCalibration");
    if (!(mode == ReadyMode || mode == NotCalibratedMode))
    {
        return;
    }
    mode = CalibratingMode;
    tmrCalibration.start();
    _runPump();
}
void PumpController::_executeCalibrationProcess()
{
    if (!(tmrCalibration.tick() && tmrCalibration.ready()))
    {
        return;
    }
    Serial.println("_executeCalibrationProcess");
    tmrCalibration.stop();
    mode = NotCalibratedMode;
    _stopPump();
}
void PumpController::finishCalibration()
{
    Serial.println("finishCalibration");
    speedMlPerMs = calibration100SecMl / (100.0 * 1000.0);
    if (speedMlPerMs <= 0)
    {
        speedMlPerMs = 0.0;
        mode = NotCalibratedMode;
        return;
    }
    mode = ReadyMode;
}

void PumpController::setEnabled(bool newValue)
{
    if (newValue == _enabled)
    {
        return;
    }
    _enabled = newValue;
    if (_enabled)
    {
        tmrAction.start();
    }
    else
    {
        tmrAction.stop();
    }
}

#define KEY_isEnabled "ENABLED"
#define KEY_mlAtTime "ML_AT_TIME"
#define KEY_INTERVAL_SEC "INTERVAL_SEC"
#define KEY_START_DATETIME_SEC "START_DT_SEC"
#define KEY_isInverted "INVERTED"
#define KEY_speedMlPerMs "ML_PER_MS"
#define KEY_calibration100SecMl "CALIB_100_ML"
#define KEY_power "POWER"

void PumpController::init()
{
    Serial.print("  PumpController...");
    Serial.print("    ");
    load();
    Serial.println("    OK");
}

void PumpController::save()
{
    Serial.println("save");
    preferences.begin(_id, RW_MODE);
    preferences.putBool(KEY_isEnabled, _enabled);
    preferences.putFloat(KEY_mlAtTime, mlAtTime);
    preferences.putBool(KEY_isInverted, isInverted);
    preferences.putFloat(KEY_speedMlPerMs, speedMlPerMs);
    preferences.putUChar(KEY_power, power);
    preferences.putULong(KEY_INTERVAL_SEC, tmrAction.getDuration());
    preferences.putFloat(KEY_calibration100SecMl, calibration100SecMl);
    preferences.putULong(KEY_START_DATETIME_SEC, tmrAction.getStartTime());
    preferences.end();
}

void PumpController::saveLastAction()
{
    Serial.println("saveLastAction");
    preferences.begin(_id, RW_MODE);
    preferences.putULong(KEY_INTERVAL_SEC, tmrAction.getDuration());
    preferences.putULong(KEY_START_DATETIME_SEC, tmrAction.getStartTime());
    preferences.end();
}

void PumpController::load()
{
    Serial.println("load");
    if (preferences.begin(_id, RO_MODE))
    {
        _enabled = preferences.getBool(KEY_isEnabled, _enabled);
        mlAtTime = preferences.getFloat(KEY_mlAtTime, mlAtTime);
        isInverted = preferences.getBool(KEY_isInverted, isInverted);
        speedMlPerMs = preferences.getFloat(KEY_speedMlPerMs, speedMlPerMs);
        calibration100SecMl = preferences.getFloat(KEY_calibration100SecMl, calibration100SecMl);
        power = preferences.getUChar(KEY_power, power);
        tmrAction.setDuration(preferences.getULong(KEY_INTERVAL_SEC, tmrAction.getDuration()));
        tmrAction.setStartTime(preferences.getULong(KEY_START_DATETIME_SEC, tmrAction.getStartTime()));
    }
    preferences.end();
    if (_enabled)
    {
        tmrAction.resume();
    }
    else
    {
        tmrAction.stop();
    }
}