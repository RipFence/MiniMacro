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
extern const int buttonPins[];
extern const char *buttonNames[];

void buttonSetup(char *singleClickBuffer[], char *doubleClickBuffer[]);
void buttonLoop();
