#include "config.h"
#include <Button2.h>
#include "buttonHelper.h"
#include "keyboardHelper.h"

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
    }
  }
  // Detect click type
  switch (btn.getType())
  {
  case single_click:
    sendString(singleBuffer[btnNum]);
    break;
  case double_click:
    sendString(doubleBuffer[btnNum]);
    break;
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
  // Set up GPIO for buttons
  for (int i = 0; i < BUTTONCOUNT; i++)
  {
    pinMode(buttonPins[i], INPUT);
    buttons[i].begin(buttonPins[i], INPUT, ACTIVE_LOW);
    buttons[i].setClickHandler(onClick);
    buttons[i].setDoubleClickHandler(onClick);
  }

  singleBuffer = singleClickBuffer;
  doubleBuffer = doubleClickBuffer;

  // Initialize HID Keyboard
  setupKeyboard();
}

void buttonLoop()
{
  for (int i = 0; i < BUTTONCOUNT; i++)
  {
    buttons[i].loop();
  }
  processHID();
}
