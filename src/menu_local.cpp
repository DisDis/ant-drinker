#include <Arduino.h>

// #include <Wire.h>
#include "config.h"
#include "display.h"
#include <menu.h>
#include <menuIO/adafruitGfxOut.h>
#include "SSD1306Out.h"
#include <menuIO/serialOut.h>
#include <menuIO/serialIO.h>
// #include <menuIO/u8g2Out.h>
//#include <menuIO/keyIn.h>
// #include <menuIO/altKeyIn.h>
#include <plugin/barField.h>
#include "state.h"
#include "i18n/en.h"
#include "common.h"
//#include <menuIO/chainStream.h>
using namespace Menu;

// https://github.com/neu-rah/ArduinoMenu/blob/master/examples/targetSel/targetSel/targetSel.ino
// define menu colors --------------------------------------------------------
// each color is in the format:
//  {{disabled normal,disabled selected},{enabled normal,enabled selected, enabled editing}}
const colorDef<uint16_t> colors[6] MEMMODE = {
    {{WHITE, BLACK}, {WHITE, BLACK, BLACK}}, // bgColor
    {{BLACK, WHITE}, {BLACK, WHITE, WHITE}}, // fgColor
    {{BLACK, WHITE}, {BLACK, WHITE, WHITE}}, // valColor
    {{BLACK, WHITE}, {BLACK, WHITE, WHITE}}, // unitColor
    {{BLACK, WHITE}, {WHITE, WHITE, WHITE}}, // cursorColor
    {{BLACK, WHITE}, {WHITE, BLACK, BLACK}}, // titleColor
};

#define gfxWidth 127
#define gfxHeight 63
#define fontX 7
#define fontY 9

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

result resetWaterTank1()
{
    waterTank1.reset();
    return proceed;
}

// ----------------- Water tanks
TOGGLE(waterTank1.enabled, waterTank1OnOff, "Enabled: ", doNothing, noEvent, wrapStyle, VALUE("Off", false, doNothing, noEvent), VALUE("On", true, doNothing, noEvent));

MENU(tank1Menu, "B1", showEvent, anyEvent, wrapStyle,
     SUBMENU(waterTank1OnOff),
     FIELD(waterTank1.capacity, "Capacity", "ml", 10, 99999, 10, 1, doNothing, enterEvent, wrapStyle),
     FIELD(waterTank1.value, "Left", "ml", 0, 99999, 10, 1, doNothing, enterEvent, wrapStyle),

     OP("Reset", resetWaterTank1, anyEvent),
     EXIT("<Back"));
// MENU(tank2Menu, "B2", showEvent, anyEvent, wrapStyle,  FIELD(waterTank2.capacity, "Capacity", "ml", 10, 9999, 10, 1, doNothing, enterEvent, wrapStyle),     EXIT("<Back"));

// SUBMENU(tank2Menu),

MENU(waterTanksMenu, "Water tanks", showEvent, anyEvent, wrapStyle,
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

MENU(dateTimeMenu, "Date/Time", showEvent, anyEvent, wrapStyle,
     altOP(DatePrompt, "", doNothing, anyEvent),
     altOP(TimePrompt, "", doNothing, anyEvent),
     OP("Time zone: +3", action1, anyEvent),
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

TOGGLE(pumpController1isEnabled, pumpController1OnOff, "Enabled: ", pump1SyncEnabled, activateEvent, wrapStyle, VALUE("Off", false, pump1Disabled, enterEvent), VALUE("On", true, pump1Enabled, enterEvent));
TOGGLE(pumpController1.isInverted, pumpController1Invert, "Dir: ", doNothing, noEvent, wrapStyle, VALUE("Right", false, doNothing, noEvent), VALUE("Left", true, doNothing, noEvent));

MENU(pumpController1CalibrationMenu, "Calibration", showEvent, anyEvent, wrapStyle,
     OP("Start 100 sec calibration", pump1calibration, enterEvent),
     BARFIELD(pumpController1.power, "Power", "%", 1, 255, 10, 1, doNothing, enterEvent, wrapStyle),
     OP("Stop pump", pump1Stop, enterEvent),
     OP("Start pump", pump1Start, enterEvent),
     OP("Amount water per 100sec", doNothing, enterEvent),
     FIELD(pumpController1.speedMlPerMs, "Speed", "Ml/Sec", 1, 255, 1, 1, doNothing, enterEvent, wrapStyle),
     EXIT("<Back"));

// custom field print
// implementing a customized menu component
// this numeric field prints formatted number with leading zeros
template <typename T>
class leadsField : public menuField<T>
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

        unsigned long day = menuField<T>::reflex / (unsigned long)(60 * 60 * 24);
        unsigned long tmpTime = menuField<T>::reflex - day * (60 * 60 * 24);
        unsigned long h = tmpTime / (60 * 60);
        tmpTime = tmpTime - h * (60 * 60);
        unsigned long m = tmpTime / (60);

        char buffer[] = "00d00h00m ";
        snprintf(buffer, sizeof(buffer), "%02dd%02dh%02dm", day, h, m);
        out.print(buffer);
        out.setColor(unitColor, sel, menuField<T>::enabled, ed);
        print_P(out, menuField<T>::units(), len);
        return len;
    }
};

MENU(pumpController1Menu, "Pump1", showEvent, anyEvent, wrapStyle,
     SUBMENU(pumpController1OnOff),
     altOP(PumpModePrompt, "", doNothing, anyEvent),
     FIELD(pumpController1.mlAtTime, "Count", "ml", 10, 999, 10, 1, doNothing, enterEvent, wrapStyle),
     altFIELD(leadsField, pumpController1.tmrAction.duration, "Interval", "", 60, 31 * 24 * 60 * 60, 5 * 60, 60, doNothing, enterEvent, wrapStyle),

     OP("Stop pump", pump1Stop, enterEvent),
     OP("Start pump", pump1Start, enterEvent),
     OP("Emergency Stop", pump1EmergencyStop, enterEvent),
     SUBMENU(pumpController1CalibrationMenu),
     SUBMENU(pumpController1Invert),
     EXIT("<Back"));

MENU(dispensersMenu, "Dispensers", showEvent, anyEvent, wrapStyle,
     SUBMENU(pumpController1Menu),
     EXIT("<Back"));
// --------

// ------------ Notification
MENU(notificationMenu, "Notification[STUB]", showEvent, anyEvent, wrapStyle,
     OP("On/Off", action1, anyEvent),
     EXIT("<Back"));
// --------
// ------------ Buzzer
TOGGLE(buzzerDevice.isEnabled, buzzerOnOff, "Enabled: ", doNothing, noEvent, wrapStyle, VALUE("Off", false, doNothing, noEvent), VALUE("On", true, doNothing, noEvent));
MENU(buzzerSettingMenu, "Buzzer", showEvent, anyEvent, wrapStyle,
     SUBMENU(buzzerOnOff),
     OP("DoNotDisturb", action1, anyEvent),
     EXIT("<Back"));
// --------
// ------------ LED
TOGGLE(ledDevice.enabled, ledOnOff, "Enabled: ", doNothing, noEvent, wrapStyle, VALUE("Off", false, doNothing, noEvent), VALUE("On", true, doNothing, noEvent));
MENU(ledSettingMenu, "LED", showEvent, anyEvent, wrapStyle,
     SUBMENU(ledOnOff),
     EXIT("<Back"));
// --------
// --------------- Network
MENU(networkMenu, "Network[STUB]", showEvent, anyEvent, wrapStyle,
     OP("WiFi", action1, anyEvent),
     OP("Telegram", action1, anyEvent),
     OP("MQTT", action1, anyEvent),
     EXIT("<Back"));
// --------- Version & info
MENU(versionInfoMenu, "Version & info[STUB]", showEvent, anyEvent, wrapStyle,
     OP(APP_VERSION, action1, anyEvent),
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

int selTest = 0;
SELECT(selTest, selMenu, "Select", doNothing, noEvent, wrapStyle, VALUE("Zero", 0, doNothing, noEvent), VALUE("One", 1, doNothing, noEvent), VALUE("Two", 2, doNothing, noEvent));

int chooseTest = -1;
CHOOSE(chooseTest, chooseMenu, "Choose", doNothing, noEvent, wrapStyle, VALUE("First", 1, doNothing, noEvent), VALUE("Second", 2, doNothing, noEvent), VALUE("Third", 3, doNothing, noEvent), VALUE("Last", -1, doNothing, noEvent));

// MENU(subMenu, "Sub-Menu", showEvent, anyEvent, wrapStyle, OP("Sub1", showEvent, anyEvent), OP("Sub2", showEvent, anyEvent),
// OP("Sub3", showEvent, anyEvent), altOP(altPrompt, "", showEvent, anyEvent), EXIT("<Back"));

/*SUBMENU(subMenu),
 SUBMENU(setLed),

OP("LED On", internalLedOn, enterEvent) // will turn on built-in LED
,
OP("LED Off", internalLedOff, enterEvent) // will turn off built-in LED
*/

MENU(mainMenu, "Main menu", doNothing, noEvent, wrapStyle,

     SUBMENU(waterTanksMenu),
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
     SUBMENU(waterTanksMenu),
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
MENU_OUTPUTS(out, MAX_DEPTH, ADAGFX_OUT(display, colors, fontX, fontY, {0, 0, gfxWidth / fontX, gfxHeight / (fontY + 1)}), SERIAL_OUT(Serial));
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
        // display.clearDisplay();
        // display.setCursor(0,0);
        nav.doOutput();
        display.display();
    }
}
