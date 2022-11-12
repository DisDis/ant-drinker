#include "config.h"
#include "common.h"
#include "pump.h"

Pump::Pump(unsigned char pwmPin, unsigned char pwmChannel, unsigned char directionPin0, unsigned char directionPin1)
{
    this->pwmPin = pwmPin;
    this->pwmChannel = pwmChannel;
    this->directionPin0 = directionPin0;
    this->directionPin1 = directionPin1;
}

void Pump::init()
{
    pinMode(directionPin0, OUTPUT);
    pinMode(directionPin1, OUTPUT);
    pinMode(pwmPin, OUTPUT);

    if (ledcSetup(pwmChannel, frequency, resolution) == 0)
    {
        LOG.println("ledc channel was not configured.");
    }
    ledcAttachPin(pwmPin, pwmChannel);
    stop();
}
/// @brief
/// @param power Power 0-255
/// @param isInverted
void Pump::start(unsigned char power, bool isInverted)
{
    ledcWrite(pwmChannel, power);
    digitalWrite(directionPin0, isInverted ? HIGH : LOW);
    digitalWrite(directionPin1, isInverted ? LOW : HIGH);
}

void Pump::stop()
{
    ledcWrite(pwmChannel, 0);
    digitalWrite(directionPin0, LOW);
    digitalWrite(directionPin1, LOW);
}