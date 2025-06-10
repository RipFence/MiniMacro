#include "config.h"
#include <Button2.h>
#include "buttonHelper.h"
#include "keyboardHelper.h"
#include "displayHelper.h"
#include "wifiHelper.h"
#include "configHelper.h"

// Global Variables
char **singleBuffer;
char **doubleBuffer;

const char *buttonNames[] = {"RED", "GREEN", "BLUE", "YELLOW", "BLACK", "WHITE"};

const int buttonPins[] = { // GPIO pins, same order as above
  RED_BUTTON_PIN,
  GREEN_BUTTON_PIN,
  BLUE_BUTTON_PIN,
  YELLOW_BUTTON_PIN,
  BLACK_BUTTON_PIN,
  WHITE_BUTTON_PIN
};

bool configMode = false;
Button2 buttons[BUTTONCOUNT];

void startConfigMode()
{
  wifiSetup();
  configSetup();
  disableScreenTimeout();
  // Wait for Buttons to be released
  while (buttons[YELLOW].isPressed())
  {
    buttons[YELLOW].loop();
  }
  while (buttons[WHITE].isPressed())
  {
    buttons[WHITE].loop();
  }
  buttons[YELLOW].resetPressedState();
  buttons[WHITE].resetPressedState();
  configMode = true;
}

void stopConfigMode()
{
  // Wait for Buttons to be released
  while (buttons[YELLOW].isPressed())
  {
    buttons[YELLOW].loop();
  }
  while (buttons[WHITE].isPressed())
  {
    buttons[WHITE].loop();
  }
  buttons[YELLOW].resetPressedState();
  buttons[WHITE].resetPressedState();
  configMode = false;
  // configStop();
  // wifiStop();
  enableScreenTimeout();
  // Restart the ESP because of bug:
  // softAP won't start after configStop() and wifiStop()
  ESP.restart();
}

void onClick(Button2 &btn)
{
  int btnNum = -1;
  // Detect button number
  for (int i = 0; i < BUTTONCOUNT; i++)
  {
    if (&btn == &buttons[i])
    {
      btnNum = i;
      break;
    }
  }
  // Detect click type
  switch (btn.getType())
  {
  case single_click:
    {
      char displayBuffer[32];
      snprintf(displayBuffer, sizeof(displayBuffer), "%s Click", buttonNames[btnNum]);
      displayShowFor(displayBuffer, 5000, 1);
      sendString(singleBuffer[btnNum]);
      break;
    }
  case double_click:
    {
      char displayBuffer[32];
      snprintf(displayBuffer, sizeof(displayBuffer), "%s Double Click", buttonNames[btnNum]);
      displayShowFor(displayBuffer, 5000, 1);
      sendString(doubleBuffer[btnNum]);
      break;
    }
  case triple_click:
    return;
  case long_click:
    // Handle White/Yellow button combo
    if ((btnNum == WHITE && buttons[YELLOW].isPressed())
        || (btnNum == YELLOW && buttons[WHITE].isPressed()))
    {
      if (configMode)
      {
        displayReady();
        stopConfigMode();
      }
      else
      {
        displayPrint("Config...", 2, 0, 0);
        startConfigMode();
      }
    }
    break;
  case empty:
    return;
  }
}

void buttonSetup(char *singleClickBuffer[], char *doubleClickBuffer[])
{
  // You can define your own timeouts by using these setter functions:
  // void setDebounceTime(unsigned int ms) // Default is 50 ms
  // void setLongClickTime(500); // Default is 200 ms
  // void setDoubleClickTime(unsigned int ms) // Default is 300 ms

  // Set up GPIO for buttons
  for (int i = 0; i < BUTTONCOUNT; i++)
  {
    pinMode(buttonPins[i], INPUT_PULLUP);
    buttons[i].begin(buttonPins[i], INPUT_PULLUP, ACTIVE_LOW);
    buttons[i].setClickHandler(onClick);
    buttons[i].setDoubleClickHandler(onClick);
    buttons[i].setLongClickTime(500);
  }

  // Set Triggers for White/Yellow button combo
  buttons[WHITE].setLongClickDetectedHandler(onClick);
  buttons[YELLOW].setLongClickDetectedHandler(onClick);

  singleBuffer = singleClickBuffer;
  doubleBuffer = doubleClickBuffer;
  Serial.println(F("Button Setup Done"));
}

void buttonLoop()
{
  for (int i = 0; i < BUTTONCOUNT; i++)
  {
    buttons[i].loop();
  }
}
