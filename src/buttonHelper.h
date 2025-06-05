#pragma once

enum Button
{
  RED,
  GREEN,
  BLUE,
  YELLOW,
  BLACK,
  WHITE,
  BUTTONCOUNT
};
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

void buttonSetup(char *singleClickBuffer[], char *doubleClickBuffer[]);
void buttonLoop();
