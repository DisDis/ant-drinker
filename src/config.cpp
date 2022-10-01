#include "config.h"
#include "common.h"

Preferences preferences;
Config appConfig;

Config::Config(){

}

void Config::load(PumpController* pc){
    pc->isEnabled = true;
    pc->tmrAction.setDuration(1000);
    pc->calibration100SecMl = 1000;
    pc->isInverted = false;
    pc->mlAtTime = 10;
    pc->power = 255;
    pc->speedMlPerMs = 10;
}
