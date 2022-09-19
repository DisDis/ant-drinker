#include "TimerMs.h"
#include "SimpleTimer.h"
#include "pump_controller.h"


void PumpController::_runPump()
{
    /// TODO: PIN - PWM
}
void PumpController::_stopPump()
{
    /// TODO: PIN - LOW
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

PumpController::PumpController(const char *id, ON_FINISH_WORK_CALLBACK finishWorkCallback)
{
    this->_id = id;
    this->finishWorkCallback = finishWorkCallback;
}
void PumpController::_executeStartWork()
{
    if (!tmrAction.elapsed())
    {
        return;
    }
    if (mode != ReadyMode)
    {
        return;
    }
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
}

void PumpController::execute()
{
    // Pump disabled
    if (!isEnabled)
    {
        return;
    }
    _executeStartWork();
    _executeStopWork();
    _executeCalibrationProcess();
}

void PumpController::stopImmediate()
{
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
    tmrAction.restart();
    mode = TurnOnMode;
    tmrWork.stop();
    _runPump();
}

void PumpController::emergencyStop()
{
    mode = NotCalibratedMode;
    _stopPump();
    tmrCalibration.stop();
    tmrWork.stop();
}

void PumpController::startCalibration()
{
    if (!(mode == ReadyMode || mode == NotCalibratedMode))
    {
        return;
    }
    mode = CalibratingMode;
    tmrCalibration.restart();
    _runPump();
}
void PumpController::_executeCalibrationProcess()
{
    if (!tmrCalibration.elapsed())
    {
        return;
    }
    tmrCalibration.stop();
    _stopPump();
}
void PumpController::finishCalibration()
{
    speedMlPerMs = (calibration100SecMl * 1000.0) / 100.0;
    if (speedMlPerMs <= 0)
    {
        speedMlPerMs = 0.0;
        mode = NotCalibratedMode;
        return;
    }
    mode = ReadyMode;
}
