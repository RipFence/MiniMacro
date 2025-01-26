#include <Arduino.h>
#include "config.h"
#include "hwTest.h"
#include "displayHelper.h"

// Global Variables
const char *buttnNames[] = {"RED", "GREEN", "BLUE", "YELLOW", "BLACK", "WHITE"};
const int buttnPins[] = { // GPIO pins, same order as above
    RED_BUTTON_PIN,
    GREEN_BUTTON_PIN,
    BLUE_BUTTON_PIN,
    YELLOW_BUTTON_PIN,
    BLACK_BUTTON_PIN,
    WHITE_BUTTON_PIN};

static int ActiveState = 0;

void hwSetup()
{
  screenOffTimeout = 0;
  // Set up GPIO for buttons
  for (int i = 0; i < BUTTNCOUNT; i++)
  {
    pinMode(buttnPins[i], INPUT_PULLUP);
  }
  if (ACTIVE_LOW)
  {
    ActiveState = LOW;
  }
  else
  {
    ActiveState = HIGH;
  }
}

void hwLoop()
{
  static bool allButtonsReleased = true;
  allButtonsReleased = true;

  for (int i = 0; i < BUTTNCOUNT; i++)
  {
    int buttonState = digitalRead(buttnPins[i]);
    if (buttonState == ActiveState)
    {
      allButtonsReleased = false;
      displayPrint(buttnNames[i], 1);
    }
  }
  if (allButtonsReleased)
  {
    displayClear();
  }
  delay(50);
}