#include <Arduino.h>
#include "config.h"
#include "wifiHelper.h"
#include "buttonHelper.h"
#include "displayHelper.h"
#include "storageHelper.h"
#include "configHelper.h"

void setup()
{
  Serial.begin(115200);
  while (!Serial)
    ;

  delay(200);

  // displaySetup();
  storageSetup();
  wifiSetup();

  loadMacros(singleMacroBuffer, SINGLE_MACRO_FILE);
  loadMacros(doubleMacroBuffer, DOUBLE_MACRO_FILE);
  // buttonSetup(singleMacroBuffer, doubleMacroBuffer);

  configSetup();
  Serial.println("Setup complete!");
}

void loop()
{
  // displayLoop();
  // buttonLoop();
  wifiLoop();
  configLoop();
}
