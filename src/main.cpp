#include <Arduino.h>
#include "config.h"
#include "wifiHelper.h"
#include "buttonHelper.h"
#include "displayHelper.h"
#include "storageHelper.h"

// Global Variables
char singleRedBuffer[MACRO_LENGTH] = "Red Button";
char singleGreenBuffer[MACRO_LENGTH] = "Green Button";
char singleBlueBuffer[MACRO_LENGTH] = "Blue Button";
char singleYellowBuffer[MACRO_LENGTH] = "Yellow Button";
char singleBlackBuffer[MACRO_LENGTH] = "Black Button";
char singleWhiteBuffer[MACRO_LENGTH] = "White Button";
char *singleMacroBuffer[] = {
    singleRedBuffer,
    singleGreenBuffer,
    singleBlueBuffer,
    singleYellowBuffer,
    singleBlackBuffer,
    singleWhiteBuffer};

char doubleRedBuffer[MACRO_LENGTH] = "Red Button Double";
char doubleGreenBuffer[MACRO_LENGTH] = "Green Button Double";
char doubleBlueBuffer[MACRO_LENGTH] = "Blue Button Double";
char doubleYellowBuffer[MACRO_LENGTH] = "Yellow Button Double";
char doubleBlackBuffer[MACRO_LENGTH] = "Black Button Double";
char doubleWhiteBuffer[MACRO_LENGTH] = "White Button Double";
char *doubleMacroBuffer[] = {
    doubleRedBuffer,
    doubleGreenBuffer,
    doubleBlueBuffer,
    doubleYellowBuffer,
    doubleBlackBuffer,
    doubleWhiteBuffer};

void setup()
{
  Serial.begin(115200);
  while (!Serial)
    ;

  delay(200);

  // displaySetup();
  // storageSetup();
  wifiSetup();

  // loadMacros(singleMacroBuffer, SINGLE_MACRO_FILE);
  // loadMacros(doubleMacroBuffer, DOUBLE_MACRO_FILE);
  // buttonSetup(singleMacroBuffer, doubleMacroBuffer);
}

void loop()
{
  // buttonLoop();
  wifiLoop();
}
