#include <Arduino.h>

// #include <Wire.h>
#include "config.h"
#include "display.h"
#include <menu.h>
#include <menuIO/adafruitGfxOut.h>
#include <menuIO/serialOut.h>
#include <menuIO/serialIO.h>
// #include <menuIO/u8g2Out.h>
//#include <menuIO/keyIn.h>
// #include <menuIO/altKeyIn.h>
#include <plugin/barField.h>
#include "state.h"
#include "i18n/en.h"
#include "common.h"
#include "Version.h"
//#include <menuIO/chainStream.h>
using namespace Menu;

#define ST7735_GRAY RGB565(128,128,128)

// define menu colors --------------------------------------------------------
//  {{disabled normal,disabled selected},{enabled normal,enabled selected, enabled editing}}
//monochromatic color table
// https://github.com/neu-rah/ArduinoMenu/blob/master/examples/targetSel/targetSel/targetSel.ino
// define menu colors --------------------------------------------------------
// each color is in the format:
//  {{disabled normal,disabled selected},{enabled normal,enabled selected, enabled editing}}
const colorDef<uint16_t> colors[6] MEMMODE = {
  {{(uint16_t)ST7735_BLACK,(uint16_t)ST7735_BLACK}, {(uint16_t)ST7735_BLACK, (uint16_t)ST7735_BLUE,  (uint16_t)ST7735_BLUE}},//bgColor
  {{(uint16_t)ST7735_GRAY, (uint16_t)ST7735_GRAY},  {(uint16_t)ST7735_WHITE, (uint16_t)ST7735_WHITE, (uint16_t)ST7735_WHITE}},//fgColor
  {{(uint16_t)ST7735_WHITE,(uint16_t)ST7735_BLACK}, {(uint16_t)ST7735_YELLOW,(uint16_t)ST7735_YELLOW,(uint16_t)ST7735_RED}},//valColor
  {{(uint16_t)ST7735_WHITE,(uint16_t)ST7735_BLACK}, {(uint16_t)ST7735_WHITE, (uint16_t)ST7735_YELLOW,(uint16_t)ST7735_YELLOW}},//unitColor
  {{(uint16_t)ST7735_WHITE,(uint16_t)ST7735_GRAY},  {(uint16_t)ST7735_BLACK, (uint16_t)ST7735_BLUE,  (uint16_t)ST7735_WHITE}},//cursorColor
  {{(uint16_t)ST7735_WHITE,(uint16_t)ST7735_YELLOW},{(uint16_t)ST7735_BLUE,  (uint16_t)ST7735_RED,   (uint16_t)ST7735_RED}},//titleColor
};

#define fontX 7
#define fontY 10

// #define fontW 7
// #define fontH 8

#define MAX_DEPTH 4
#define MENU_ASYNC

int ledCtrl = HIGH; // Default LED State of LED at D11 is LOW

result doAlert(eventMask e, prompt &item);

result showEvent(eventMask e, navNode &nav, prompt &item)
{

    return proceed;
}

result action1(eventMask e)
{

    return proceed;
}

result action2(eventMask e, navNode &nav, prompt &item)
{
    // Serial.print(e);
    // Serial.println(" action2 executed, quiting menu");
    return quit;
}

result ledOn()
{
    ledCtrl = HIGH;
    digitalWrite(LED_PIN, HIGH);
    return proceed;
}
result ledOff()
{
    ledCtrl = LOW;
    digitalWrite(LED_PIN, LOW);
    return proceed;
}

result internalLedOn()
{
    digitalWrite(LED_BUILTIN, HIGH);
    return proceed;
}
result internalLedOff()
{
    digitalWrite(LED_BUILTIN, LOW);
    return proceed;
}

result resetWaterBottle1(eventMask e)
{
    waterBottle1.reset();
    return proceed;
}

result saveWaterBottle1(eventMask e)
{
    waterBottle1.save();
    return proceed;
}

// ----------------- Water bottles
TOGGLE(waterBottle1.enabled, WaterBottle1OnOff, "Enabled: ", doNothing, noEvent, wrapStyle, VALUE("Off", false, doNothing, noEvent), VALUE("On", true, doNothing, noEvent));

MENU(tank1Menu, "Bottle1", doNothing, noEvent, wrapStyle,
     SUBMENU(WaterBottle1OnOff),
     FIELD(waterBottle1.capacity, "Capacity", "ml", 10, 99999, 10, 1, doNothing, noEvent, wrapStyle),
     FIELD(waterBottle1.value, "Left", "ml", 0, 99999, 10, 1, doNothing, noEvent, wrapStyle),
     OP("Save", saveWaterBottle1, enterEvent),
     OP("Reset", resetWaterBottle1, enterEvent),
     EXIT("<Back"));
// MENU(tank2Menu, "B2", doNothing, anyEvent, wrapStyle,  FIELD(WaterBottle2.capacity, "Capacity", "ml", 10, 9999, 10, 1, doNothing, enterEvent, wrapStyle),     EXIT("<Back"));

// SUBMENU(tank2Menu),

MENU(WaterBottlesMenu, "Water bottles", doNothing, noEvent, wrapStyle,
     SUBMENU(tank1Menu),
     EXIT("<Back"));
// ---------

// ------- Date/Time

char outpuDateTime[80];
struct tm timeinfoMenu;
time_t nowMenu;
class DatePrompt : public prompt
{
public:
    DatePrompt(constMEM promptShadow &p) : prompt(p) {}
    Used printTo(navRoot &root, bool sel, menuOut &out, idx_t idx, idx_t len, idx_t) override
    {
        time(&nowMenu);
        localtime_r(&nowMenu, &timeinfoMenu);
        strftime(outpuDateTime, 80, MENU_DATE_FORMAT, &timeinfoMenu);
        return out.printRaw(F(outpuDateTime), len);
    }
};

class TimePrompt : public prompt
{
public:
    unsigned int t = 0;
    unsigned int last = 0;
    TimePrompt(constMEM promptShadow &p) : prompt(p) {}
    Used printTo(navRoot &root, bool sel, menuOut &out, idx_t idx, idx_t len, idx_t) override
    {
        last = t;
        time(&nowMenu);
        localtime_r(&nowMenu, &timeinfoMenu);
        strftime(outpuDateTime, 80, MENU_TIME_FORMAT, &timeinfoMenu);
        return out.printRaw(F(outpuDateTime), len);
    }
    virtual bool changed(const navNode &nav, const menuOut &out, bool sub = true)
    {
        t = millis() / 1000;
        return last != t;
    }
};

MENU(dateTimeMenu, "Date/Time", doNothing, noEvent, wrapStyle,
     altOP(DatePrompt, "", doNothing, noEvent),
     altOP(TimePrompt, "", doNothing, noEvent),
     OP("Time zone: +3", doNothing, noEvent),
     EXIT("<Back"));
// --------

result pump1Stop(eventMask e)
{
    pumpController1.stopImmediate();
    return proceed;
}
result pump1Start(eventMask e)
{
    pumpController1.startImmediate();
    return proceed;
}
result pump1EmergencyStop(eventMask e)
{
    pumpController1.emergencyStop();
    return proceed;
}

result pump1calibration(eventMask e)
{
    pumpController1.startCalibration();
    return proceed;
}
result pump1Save(eventMask e)
{
    pumpController1.save();
    return proceed;
}

result pump1Enabled(eventMask e)
{
    Serial.println("Menu - pump1Enabled");
    pumpController1.setEnabled(true);
    return proceed;
}
result pump1Disabled(eventMask e)
{
    Serial.println("Menu - pump1Disabled");
    pumpController1.setEnabled(false);
    return proceed;
}

result pump1FinishCalib(eventMask e)
{
    Serial.println("Menu - pump1FinishCalib");
    pumpController1.finishCalibration();
    return proceed;
}

bool pumpController1isEnabled = pumpController1.getEnabled();
result pump1SyncEnabled(eventMask e)
{
    Serial.println("Menu - pump1SyncEnabled");
    pumpController1isEnabled = pumpController1.getEnabled();
    return proceed;
}

// ------------ Dispensers

class PumpModePrompt : public prompt
{
public:
    char output[80];
    PumpModePrompt(constMEM promptShadow &p) : prompt(p) {}
    Used printTo(navRoot &root, bool sel, menuOut &out, idx_t idx, idx_t len, idx_t) override
    {
        snprintf(output, 80, "Mode: %s", PumpModeName[pumpController1.getMode() - 1]);
        return out.printRaw(F(output), len);
    }
};

class PumpSpeedPrompt : public prompt
{
public:
    char output[50];
    PumpSpeedPrompt(constMEM promptShadow &p) : prompt(p) {}
    Used printTo(navRoot &root, bool sel, menuOut &out, idx_t idx, idx_t len, idx_t) override
    {
        float seedInSec = pumpController1.speedMlPerMs * 1000;
        snprintf(output, sizeof(output), "Speed: %3f ml/sec", seedInSec);
        return out.printRaw(F(output), len);
    }
};

TOGGLE(pumpController1isEnabled, pumpController1OnOff, "Enabled: ", pump1SyncEnabled, activateEvent, wrapStyle, VALUE("Off", false, pump1Disabled, enterEvent), VALUE("On", true, pump1Enabled, enterEvent));
TOGGLE(pumpController1.isInverted, pumpController1Invert, "Dir: ", doNothing, noEvent, wrapStyle, VALUE("Right", false, doNothing, noEvent), VALUE("Left", true, doNothing, noEvent));

// FIELD(pumpController1.speedMlPerMs, "Speed", "ml/sec", 1, 255, 1, 1, doNothing, enterEvent, wrapStyle),
MENU(pumpController1CalibrationMenu, "Calibration", doNothing, noEvent, wrapStyle,
     OP("Start 100 sec calibration", pump1calibration, enterEvent),
     BARFIELD(pumpController1.power, "Power", "%", 1, 255, 10, 1, doNothing, noEvent, wrapStyle),
     OP("Stop pump", pump1Stop, enterEvent),
     OP("Start pump", pump1Start, enterEvent),
     altOP(PumpSpeedPrompt, "", doNothing, noEvent),
     FIELD(pumpController1.calibration100SecMl, "Water per 100sec", "ml", 1, 999, 10, 1, doNothing, noEvent, wrapStyle),
     OP("Finish calibration", pump1FinishCalib, enterEvent),
     OP("Save", pump1Save, enterEvent),
     EXIT("<Back"));

// custom field print
// implementing a customized menu component
// this numeric field prints formatted number with leading zeros
template <typename T>
class IntervalField : public menuField<T>
{
public:
    using menuField<T>::menuField;
    Used printTo(navRoot &root, bool sel, menuOut &out, idx_t idx, idx_t len, idx_t panelNr = 0) override
    {
        menuField<T>::reflex = menuField<T>::target();
        prompt::printTo(root, sel, out, idx, len);
        bool ed = this == root.navFocus;
        out.print((root.navFocus == this && sel) ? (menuField<T>::tunning ? '>' : ':') : ' ');
        out.setColor(valColor, sel, menuField<T>::enabled, ed);

        int day = menuField<T>::reflex / (unsigned long)(60 * 60 * 24);
        unsigned long tmpTime = menuField<T>::reflex - day * (60 * 60 * 24);
        int h = tmpTime / (60 * 60);
        tmpTime = tmpTime - h * (60 * 60);
        int m = tmpTime / (60);

        char buffer[] = "00d00h00m ";
        snprintf(buffer, sizeof(buffer), "%02dd%02dh%02dm", day, h, m);
        out.print(buffer);
        out.setColor(unitColor, sel, menuField<T>::enabled, ed);
        print_P(out, menuField<T>::units(), len);
        return len;
    }
};

MENU(pumpController1Menu, "Pump1", doNothing, noEvent, wrapStyle,
     SUBMENU(pumpController1OnOff),
     altOP(PumpModePrompt, "", doNothing, noEvent),
     FIELD(pumpController1.mlAtTime, "Count", "ml", 10, 999, 10, 1, doNothing, enterEvent, wrapStyle),
     altFIELD(IntervalField, pumpController1.tmrAction.duration, "Interval", "", 60, 31 * 24 * 60 * 60, 5 * 60, 60, doNothing, noEvent, wrapStyle),

     OP("Stop pump", pump1Stop, enterEvent),
     OP("Start pump", pump1Start, enterEvent),
     OP("Emergency Stop", pump1EmergencyStop, enterEvent),
     SUBMENU(pumpController1CalibrationMenu),
     SUBMENU(pumpController1Invert),
     OP("Save", pump1Save, enterEvent),
     EXIT("<Back"));

MENU(dispensersMenu, "Dispensers", doNothing, noEvent, wrapStyle,
     SUBMENU(pumpController1Menu),
     EXIT("<Back"));
// --------

// ------------ Notification
MENU(notificationMenu, "Notification[STUB]", doNothing, noEvent, wrapStyle,
     OP("On/Off", action1, noEvent),
     EXIT("<Back"));
// --------
// ------------ Buzzer
TOGGLE(buzzerDevice.isEnabled, buzzerOnOff, "Enabled: ", doNothing, noEvent, wrapStyle, VALUE("Off", false, doNothing, noEvent), VALUE("On", true, doNothing, noEvent));
MENU(buzzerSettingMenu, "Buzzer", doNothing, noEvent, wrapStyle,
     SUBMENU(buzzerOnOff),
     OP("DoNotDisturb", doNothing, noEvent),
     EXIT("<Back"));
// --------
// ------------ LED
TOGGLE(ledDevice.enabled, ledOnOff, "Enabled: ", doNothing, noEvent, wrapStyle, VALUE("Off", false, doNothing, noEvent), VALUE("On", true, doNothing, noEvent));
MENU(ledSettingMenu, "LED", doNothing, noEvent, wrapStyle,
     SUBMENU(ledOnOff),
     EXIT("<Back"));
// --------
// --------------- Network
MENU(networkMenu, "Network[STUB]", doNothing, noEvent, wrapStyle,
     OP("WiFi", doNothing, noEvent),
     OP("Telegram", doNothing, noEvent),
     OP("MQTT", doNothing, noEvent),
     EXIT("<Back"));
// --------- Version & info
MENU(versionInfoMenu, "Version & info[STUB]", doNothing, noEvent, wrapStyle,
     OP(VERSION, doNothing, noEvent),
     OP(BUILD_TIMESTAMP, doNothing, noEvent),
     EXIT("<Back"));
// ---------------
// ---------------

int brightnessValue = 15; // Default LED brightness value
result adjustBrightness()
{
    if (ledCtrl == HIGH)
    {
        int pwm = int(2.55 * float(brightnessValue));
        digitalWrite(LED_PIN, pwm);
    }
    return proceed;
}

TOGGLE(ledCtrl, setLed, "Led: ", doNothing, noEvent, wrapStyle //,doExit,enterEvent,wrapStyle
       ,
       VALUE("On", HIGH, ledOn, enterEvent) // ledOn function is called
       ,
       VALUE("Off", LOW, ledOff, enterEvent) // ledOff function is called
);

// int selTest = 0;
// SELECT(selTest, selMenu, "Select", doNothing, noEvent, wrapStyle, VALUE("Zero", 0, doNothing, noEvent), VALUE("One", 1, doNothing, noEvent), VALUE("Two", 2, doNothing, noEvent));

// int chooseTest = -1;
// CHOOSE(chooseTest, chooseMenu, "Choose", doNothing, noEvent, wrapStyle, VALUE("First", 1, doNothing, noEvent), VALUE("Second", 2, doNothing, noEvent), VALUE("Third", 3, doNothing, noEvent), VALUE("Last", -1, doNothing, noEvent));

// MENU(subMenu, "Sub-Menu", showEvent, anyEvent, wrapStyle, OP("Sub1", showEvent, anyEvent), OP("Sub2", showEvent, anyEvent),
// OP("Sub3", showEvent, anyEvent), altOP(altPrompt, "", showEvent, anyEvent), EXIT("<Back"));

/*SUBMENU(subMenu),
 SUBMENU(setLed),

OP("LED On", internalLedOn, enterEvent) // will turn on built-in LED
,
OP("LED Off", internalLedOff, enterEvent) // will turn off built-in LED
*/

MENU(mainMenu, "Main menu", doNothing, noEvent, wrapStyle,

     SUBMENU(WaterBottlesMenu),
     SUBMENU(dateTimeMenu),
     SUBMENU(dispensersMenu),
     SUBMENU(notificationMenu),
     SUBMENU(buzzerSettingMenu),
     SUBMENU(ledSettingMenu),
     SUBMENU(networkMenu),
     SUBMENU(versionInfoMenu),
     EXIT("<Back"));

/*

MENU(mainMenu, "Main menu", doNothing, noEvent, wrapStyle,
     SUBMENU(WaterBottlesMenu),
     SUBMENU(dateTimeMenu),

     OP("Op1", action1, anyEvent), OP("Op2", action2, enterEvent),
     FIELD(brightnessValue, "Brightness", "%", 0, 100, 5, 5, adjustBrightness, enterEvent, wrapStyle),

     ,
     SUBMENU(selMenu), SUBMENU(chooseMenu),
     OP("Alert test", doAlert, enterEvent), EXIT("<Back"));

*/

// define output device
idx_t serialTops[MAX_DEPTH] = {0};
serialOut outSerial(Serial, serialTops);

// describing a menu output device without macros
// define at least one panel for menu output
//  const panel panels[] MEMMODE = {{0, 0, gfxWidth / fontX, gfxHeight / fontY}};
//  navNode* nodes[sizeof(panels) / sizeof(panel)]; //navNodes to store navigation status
//  panelsList pList(panels, nodes, 1); //a list of panels and nodes
//  idx_t tops[MAX_DEPTH] = {0, 0}; //store cursor positions for each level
//  SSD1306Out outOLED(&display, tops, pList, fontX, fontY+1); //oled output device menu driver
//  menuOut* constMEM outputs[] MEMMODE = {&outOLED, &outSerial}; //list of output devices
//  outputsList out(outputs, sizeof(outputs) / sizeof(menuOut*)); //outputs list

// build a map of keys to menu commands
// keyMap joystickBtn_map[] = {
//     {BTN_SEL, defaultNavCodes[enterCmd].ch, INPUT_PULLDOWN},
//     {BTN_UP, defaultNavCodes[upCmd].ch, INPUT_PULLDOWN},
//     {BTN_DOWN, defaultNavCodes[downCmd].ch, INPUT_PULLDOWN},
//     {ButtonLeftPin, defaultNavCodes[leftCmd].ch, INPUT_PULLDOWN},
//     {ButtonRightPin, defaultNavCodes[rightCmd].ch, INPUT_PULLDOWN},
// };
// keyIn<TOTAL_NAV_BUTTONS> joystickBtns(joystickBtn_map); // the input driver

//{0,0,14,8},{14,0,14,8}
//{0, 0, gfxWidth / fontX, gfxHeight / fontY}
MENU_OUTPUTS(out, MAX_DEPTH, ADAGFX_OUT(display, colors, fontX, fontY, {0, 0, SCREEN_WIDTH / fontX, SCREEN_HEIGHT / fontY}), SERIAL_OUT(Serial));
serialIn serial(Serial);
NAVROOT(nav, mainMenu, MAX_DEPTH, serial, out);

result alert(menuOut &o, idleEvent e)
{
    if (e == idling)
    {
        o.setCursor(0, 0);
        o.print("alert test");
        o.setCursor(0, 1);
        o.print("press [select]");
        o.setCursor(0, 2);
        o.print("to continue...");
    }
    return proceed;
}

result doAlert(eventMask e, prompt &item)
{
    nav.idleOn(alert);
    return proceed;
}

// when menu is suspended
result idle(menuOut &o, idleEvent e)
{
    o.clear();
    switch (e)
    {
    case idleStart:
        o.println("suspending menu!");
        break;
    case idling:
        o.println("suspended...");
        applicationState.currentPage = mainPage;
        break;
    case idleEnd:
        o.println("resuming menu.");
        break;
    }
    return proceed;
}

void menuSetup()
{
    nav.idleTask = idle; // point a function to be used when menu is suspended
}

void menuLoop()
{
    nav.doInput();
    if (applicationState.buttonClick)
    {
        nav.doNav(navCmd(enterCmd));
    }
    if (applicationState.buttonLeft)
    {
        nav.doNav(navCmd(leftCmd));
    }

    if (applicationState.buttonUp)
    {
        nav.doNav(navCmd(upCmd));
    }
    if (applicationState.buttonDown)
    {
        nav.doNav(navCmd(downCmd));
    }
    if (applicationState.buttonRight)
    {
        nav.doNav(navCmd(rightCmd));
    }

    if (nav.changed(0))
    { // only draw if changed
        nav.doOutput();
    }
}
