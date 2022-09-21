/**************************************************************************
  Sketch: MENU NAVIGATION WITH JUST 4 BUTTONS

  This Sketch displays Menu without any Serial Communication and the
  navigation is performed by 4 buttons attached to D3, D8, D4 and D1.
  Also, attach led on D11 to control brightness from menu.
  Default brightness is 15% (check line 78)

   NOTE: By default, navigation buttons use INTERNAL_PULLUP feature.
         This can be changed by commenting the line 45 in "config.h" file

   Uses SSD1306Ascii Library(https://github.com/greiman/SSD1306Ascii)
   by Bill Grieman

   Created by Tamojit Saha
    Github: https://github.com/TamojitSaha
    Website: https://www.tamojitsaha.info/
***************************************************************************/
// https://github.com/neu-rah/ArduinoMenu/blob/master/examples/adafruitGfx_lcdMono/lcdMono/lcdMono.ino
#include <Arduino.h>

// #include <Wire.h>
#include "config.h"
#include "display.h"
#include <menu.h>
#include <menuIO/adafruitGfxOut.h>
#include <menuIO/serialOut.h>
#include <menuIO/keyIn.h>
#include "state.h"
#include "i18n/en.h"
//#include <menuIO/chainStream.h>
using namespace Menu;

//https://github.com/neu-rah/ArduinoMenu/blob/master/examples/targetSel/targetSel/targetSel.ino

const colorDef<uint16_t> colors[6] MEMMODE = {
    {{WHITE, BLACK}, {WHITE, BLACK, BLACK}}, // bgColor
    {{BLACK, WHITE}, {BLACK, WHITE, WHITE}}, // fgColor
    {{BLACK, WHITE}, {BLACK, WHITE, WHITE}}, // valColor
    {{BLACK, WHITE}, {BLACK, WHITE, WHITE}}, // unitColor
    {{BLACK, WHITE}, {WHITE, WHITE, WHITE}}, // cursorColor
    {{BLACK, WHITE}, {WHITE, BLACK, BLACK}}, // titleColor
};

#define gfxWidth 128
#define gfxHeight 64
#define fontX 6
// 5
#define fontY 9

#define LARGE_FONT Verdana12

// Navigate buttons
#define BTN_SEL ButtonClickPin // Select button
#define BTN_UP ButtonUpPin     // Up Button
#define BTN_DOWN ButtonDownPin // Down Button
#define BTN_ESC ButtonLeftPin  // Exit Button
// Comment the following to disable internal pullup for Navigate buttons
//#define NAV_BUTTONS_INPUT_PULLUP

#define TOTAL_NAV_BUTTONS 4 // Total Navigation Button used

#define MAX_DEPTH 2

#ifdef LOC
// #define LARGE_FONT
#define INV
#endif

/*Do not change the values(recomended)*/
#ifdef LARGE_FONT
#define menuFont LARGE_FONT
#define fontW 8
#define fontH 16
#else
// #define menuFont System5x7
#define menuFont lcd5x7
#define fontW 5
#define fontH 8
#endif

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

TOGGLE(ledCtrl, setLed, "Led: ", doNothing, noEvent, noStyle //,doExit,enterEvent,noStyle
       ,
       VALUE("On", HIGH, ledOn, enterEvent) // ledOn function is called
       ,
       VALUE("Off", LOW, ledOff, enterEvent) // ledOff function is called
);

int selTest = 0;
SELECT(selTest, selMenu, "Select", doNothing, noEvent, noStyle, VALUE("Zero", 0, doNothing, noEvent), VALUE("One", 1, doNothing, noEvent), VALUE("Two", 2, doNothing, noEvent));

int chooseTest = -1;
CHOOSE(chooseTest, chooseMenu, "Choose", doNothing, noEvent, noStyle, VALUE("First", 1, doNothing, noEvent), VALUE("Second", 2, doNothing, noEvent), VALUE("Third", 3, doNothing, noEvent), VALUE("Last", -1, doNothing, noEvent));

// customizing a prompt look!
// by extending the prompt class
class altPrompt : public prompt
{
public:
    altPrompt(constMEM promptShadow &p) : prompt(p) {}
    Used printTo(navRoot &root, bool sel, menuOut &out, idx_t idx, idx_t len, idx_t) override
    {
        return out.printRaw(F("special prompt!"), len);
    }
};

MENU(subMenu, "Sub-Menu", showEvent, anyEvent, noStyle, OP("Sub1", showEvent, anyEvent), OP("Sub2", showEvent, anyEvent), OP("Sub3", showEvent, anyEvent), altOP(altPrompt, "", showEvent, anyEvent), EXIT("<Back"));


MENU(waterTanksMenu, "Water tanks", showEvent, anyEvent, noStyle, 
    FIELD(appConfig.wTank1.capacity, "Capacity", "ml", 10, 9999, 10, 1, doNothing, enterEvent, wrapStyle), EXIT("<Back"));


MENU(mainMenu, "Main menu", doNothing, noEvent, wrapStyle, OP("Op1", action1, anyEvent), OP("Op2", action2, enterEvent)
     /* FIELD Parameters :

        Action Name(function name), Action Heading, Action Heading Unit,
        range_lowest, range_highest, range_increment_step,
        range_decrement_step
     */
     ,
     FIELD(brightnessValue, "Brightness", "%", 0, 100, 5, 5, adjustBrightness, enterEvent, wrapStyle), SUBMENU(subMenu), SUBMENU(setLed),
     SUBMENU(waterTanksMenu),
      OP("LED On", internalLedOn, enterEvent) // will turn on built-in LED
     ,
     OP("LED Off", internalLedOff, enterEvent) // will turn off built-in LED
     ,
     SUBMENU(selMenu), SUBMENU(chooseMenu), OP("Alert test", doAlert, enterEvent), EXIT("<Back"));

// describing a menu output device without macros
// define at least one panel for menu output
const panel panels[] MEMMODE = {{0, 0, 128 / fontW, 64 / fontH}};
navNode *nodes[sizeof(panels) / sizeof(panel)]; // navNodes to store navigation status
panelsList pList(panels, nodes, 1);             // a list of panels and nodes
idx_t tops[MAX_DEPTH] = {0, 0};                 // store cursor positions for each level

// #ifdef LARGE_FONT
// SSD1306AsciiOut outOLED(&display, tops, pList, 8, 2); //oled output device menu driver

// #else
// SSD1306AsciiOut outOLED(&oled, tops, pList, 5, 1); //oled output device menu driver
// #endif

// menuOut* constMEM outputs[]  MEMMODE  = {&display}; //list of output devices
// outputsList out(outputs, 1); //outputs list

#ifdef NAV_BUTTONS_INPUT_PULLUP
// build a map of keys to menu commands
keyMap joystickBtn_map[] = {
    {-ButtonClickPin, defaultNavCodes[enterCmd].ch},
    {-ButtonUpPin, defaultNavCodes[upCmd].ch},
    {-ButtonDownPin, defaultNavCodes[downCmd].ch},
    {-ButtonLeftPin, defaultNavCodes[escCmd].ch},
};
keyIn<TOTAL_NAV_BUTTONS> joystickBtns(joystickBtn_map); // the input driver
#else
// build a map of keys to menu commands
keyMap joystickBtn_map[] = {
    {BTN_SEL, defaultNavCodes[enterCmd].ch},
    {BTN_UP, defaultNavCodes[upCmd].ch},
    {BTN_DOWN, defaultNavCodes[downCmd].ch},
    {BTN_ESC, defaultNavCodes[escCmd].ch},
};
keyIn<TOTAL_NAV_BUTTONS> joystickBtns(joystickBtn_map); // the input driver
#endif

MENU_OUTPUTS(out, MAX_DEPTH, ADAGFX_OUT(display, colors, fontX, fontY, {0, 0, gfxWidth / fontX, gfxHeight / fontY}), SERIAL_OUT(Serial));

NAVROOT(nav, mainMenu, MAX_DEPTH, joystickBtns, out);

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
    joystickBtns.begin();
    // pinMode(LED_PIN, OUTPUT);
    // pinMode(LED_BUILTIN, OUTPUT);
    // oled.begin(&Adafruit128x64, OLED_I2C_ADDRESS); //check config
    // oled.setFont(menuFont);
    // oled.clear();
    nav.idleTask = idle; // point a function to be used when menu is suspended
}

void menuLoop()
{
    //    nav.poll();
    nav.doInput();
    if (nav.changed(0))
    { // only draw if changed
        nav.doOutput();
        display.display();
    }
}
