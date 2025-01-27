#pragma once
#include <LittleFS.h>

extern char *singleMacroBuffer[];
extern char *doubleMacroBuffer[];

// Function to initialize LittleFS and load stored macros
bool storageSetup();

// Function to save macros to LittleFS
bool saveMacros(char *macros[], const char *fileName);

// Function to load macros from LittleFS
void loadMacros(char *macroArray[], const char *fileName);

// Format LittleFS and write necessary files
void formatStorage();

void ls(File dir, int numTabs);
