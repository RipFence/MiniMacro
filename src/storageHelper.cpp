#include "config.h"
#include <Adafruit_TinyUSB.h>
#include <LittleFS.h>
#include <ArduinoJson.h> // JSON 7.x
#include "storageHelper.h"
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
  Serial.println("Saving macros to storage...");
  File macroFile = LittleFS.open(fileName, "w");
  if (!macroFile)
  {
    Serial.print("Failed to open ");
    Serial.println(fileName);
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
    Serial.println("Nothing to write to file");
    macroFile.close();
    return false;
  }
  Serial.println("Macros written to storage");
  macroFile.close();
  return true;
}

void loadMacros(char *macroArray[], const char *fileName)
{
  Serial.println("Loading macros from storage...");
  File macroFile = LittleFS.open(fileName, "r");
  if (!macroFile)
  {
    Serial.print("Failed to open ");
    Serial.println(fileName);
    return;
  }

  JsonDocument doc;

  // Parse JSON from file
  DeserializationError error = deserializeJson(doc, macroFile);
  macroFile.close();

  if (error)
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return;
  }

  // Load values into macro arrays
  for (int i = 0; i < BUTTONCOUNT; i++)
  {
    if (doc.containsKey(buttonNames[i]))
    {
      Serial.print("Loading macro for ");
      Serial.println(buttonNames[i]);
      const char *macro = doc[buttonNames[i]].as<const char *>();
      if (macro)
      {
        strncpy(macroArray[i], macro, MACRO_LENGTH - 1);
        macroArray[i][MACRO_LENGTH - 1] = '\0'; // Ensure null termination
      }
    }
  }
}

// // cost File dir = LittleFS.open("/");
// // ls (dir, 0);
// void ls(File dir, int numTabs)
// {
//   while (true)
//   {

//     File entry = dir.openNextFile();
//     if (!entry)
//     {
//       // no more files
//       break;
//     }
//     for (uint8_t i = 0; i < numTabs; i++)
//     {
//       Serial.print('\t');
//     }
//     Serial.print(entry.name());
//     if (entry.isDirectory())
//     {
//       Serial.println("/");
//       ls(entry, numTabs + 1);
//     }
//     else
//     {
//       // files have sizes, directories do not
//       Serial.print("\t\t");
//       Serial.println(entry.size(), DEC);
//     }
//     entry.close();
//   }
// }
