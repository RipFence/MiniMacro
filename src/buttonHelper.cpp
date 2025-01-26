#include "config.h"
#include <Button2.h>
#include "buttonHelper.h"
#include "keyboardHelper.h"
#include "displayHelper.h"

// Global Variables
const char *buttonNames[] = {"RED", "GREEN", "BLUE", "YELLOW", "BLACK", "WHITE"};
const int buttonPins[] = { // GPIO pins, same order as above
    RED_BUTTON_PIN,
    GREEN_BUTTON_PIN,
    BLUE_BUTTON_PIN,
    YELLOW_BUTTON_PIN,
    BLACK_BUTTON_PIN,
    WHITE_BUTTON_PIN};

char **singleBuffer;
char **doubleBuffer;

Button2 buttons[BUTTONCOUNT];

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

  singleBuffer = singleClickBuffer;
  doubleBuffer = doubleClickBuffer;
}

void buttonLoop()
{
  for (int i = 0; i < BUTTONCOUNT; i++)
  {
    buttons[i].loop();
  }
}
