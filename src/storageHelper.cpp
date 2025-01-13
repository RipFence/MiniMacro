#include "config.h"
#include <Adafruit_TinyUSB.h>
#include <LittleFS.h>
#include "storageHelper.h"
#include "buttonHelper.h"

bool storageSetup()
{
  // Initialize LittleFS
  if (!LittleFS.begin())
  {
    // Failed to mount filesystem
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

bool saveMacros(const char *macros[], const char *fileName)
{
  // Save macros to file
  File macroFile = LittleFS.open(fileName, "w");
  if (macroFile)
  {
    for (int i = 0; i < BUTTONCOUNT; i++)
    {
      size_t len = strlen(macros[i]);
      macroFile.write((uint8_t *)&len, sizeof(size_t));
      macroFile.write((uint8_t *)macros[i], len);
    }
    macroFile.close();
    return true;
  }
  else
  {
    return false;
  }
}

void loadMacros(char *macroArray[], const char *fileName)
{
  // Load macros from storage
  File macroFile = LittleFS.open(fileName, "r");
  if (macroFile)
  {
    for (int i = 0; i < BUTTONCOUNT; i++)
    {
      size_t len;
      if (macroFile.read((uint8_t *)&len, sizeof(size_t)) == sizeof(size_t))
      {
        if (len < MACRO_LENGTH)
        {
          macroFile.read((uint8_t *)macroArray[i], len);
          macroArray[i][len] = '\0'; // Ensure null termination
        }
      }
    }
    macroFile.close();
  }
  else
  {
    // File not found, initialize with empty strings
    for (int i = 0; i < BUTTONCOUNT; i++)
    {
      macroArray[i] = EMPTY_BUTTON_TEXT;
    }
  }
}