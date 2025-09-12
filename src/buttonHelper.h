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

extern const char *buttonNames[];

extern bool configMode;

void buttonSetup(char *singleClickBuffer[], char *doubleClickBuffer[]);
void buttonLoop();
