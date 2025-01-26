#include "config.h"
#include <Adafruit_TinyUSB.h>
#include <LittleFS.h>
#include <ArduinoJson.h> // JSON 7.x
#include "storageHelper.h"
#include "displayHelper.h"
#include "buttonHelper.h"
#include "web/index.h"

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

bool storageSetup()
{
  // Initialize LittleFS
  if (!LittleFS.begin())
  {
    // Failed to mount filesystem
    displayShowFor("Formatting...", 10000, 1, 0, 0);
    LittleFS.format(); // Try formatting and mounting again
    if (!LittleFS.begin())
    {
      return false; // Still failed, give up
    }
  }

  // Format if necessary (first time use)
  if (!LittleFS.exists("/version"))
  {
    File versionFile = LittleFS.open("/version", "w");
    if (versionFile)
    {
      versionFile.write((uint8_t *)&STORAGE_VERSION, sizeof(STORAGE_VERSION));
      versionFile.close();
    }
  }
  return true;
}

bool saveMacros(char *macros[], const char *fileName)
{
  displayShowFor("Saving...", 3000, 2, 0, 0);
  File macroFile = LittleFS.open(fileName, "w");
  if (!macroFile)
  {
    return false;
  }

  JsonDocument doc;

  // Create JSON structure
  for (int i = 0; i < BUTTONCOUNT; i++)
  {
    doc[buttonNames[i]] = macros[i];
  }

  // Serialize JSON to file
  if (serializeJson(doc, macroFile) == 0)
  {
    macroFile.close();
    return false;
  }
  macroFile.close();
  return true;
}

void loadMacros(char *macroArray[], const char *fileName)
{
  displayShowFor("Loading...", 3000, 2, 0, 0);
  File macroFile = LittleFS.open(fileName, "r");
  if (!macroFile)
  {
    displayPrint("Failed to load", 1, 0, 0);
    return;
  }

  JsonDocument doc;

  // Parse JSON from file
  DeserializationError error = deserializeJson(doc, macroFile);
  macroFile.close();

  if (error)
  {
    return;
  }

  // Load values into macro arrays
  for (int i = 0; i < BUTTONCOUNT; i++)
  {
    if (doc.containsKey(buttonNames[i]))
    {
      const char *macro = doc[buttonNames[i]].as<const char *>();
      if (macro)
      {
        strncpy(macroArray[i], macro, MACRO_LENGTH - 1);
        macroArray[i][MACRO_LENGTH - 1] = '\0'; // Ensure null termination
      }
    }
  }
}
