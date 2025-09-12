#include <Arduino.h>
#include "config.h"
#include "wifiHelper.h"
#include "buttonHelper.h"
#include "displayHelper.h"
#include "storageHelper.h"
#include "configHelper.h"
#include "keyboardHelper.h"

void setup()
{
  Serial.begin(115200);
  displaySetup();
  keyboardSetup(); // Initialize HID Keyboard

  storageSetup();
  loadMacros(singleMacroBuffer, SINGLE_MACRO_FILE);
  loadMacros(doubleMacroBuffer, DOUBLE_MACRO_FILE);

  buttonSetup(singleMacroBuffer, doubleMacroBuffer);
  // wifiSetup();
  // configSetup();
  displayReady();
  Serial.println(F("Main Setup Done"));
}

void loop()
{
  displayLoop();
  processHID();
  buttonLoop();
  if (configMode)
  {
    wifiLoop();
    configLoop();
  }
}
