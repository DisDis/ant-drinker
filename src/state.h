#ifndef STATE_h
#define STATE_h

#include <Arduino.h>

enum ScreenType
{
  graphTPage = 1,
  mainPage,
  graphHPage,
  menuPage,
  idlePage
};

class AppState
{
public:
  bool isDisplayOn = true;
  unsigned long lastActionMillis = millis();
  unsigned long lastSaveTHData = 0;
  ScreenType currentPage = mainPage;
  bool buttonClick = false;
  bool buttonLeft = false;
  bool buttonRight = false;
  bool buttonUp = false;
  bool buttonDown = false;
  
  // status
  bool isWifi = false;
};
extern AppState applicationState;

#endif