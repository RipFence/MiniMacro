#pragma once

// Function to initialize LittleFS and load stored macros
bool storageSetup();

// Function to save macros to LittleFS
bool saveMacros(const char *macros[], const char *fileName);

// Function to load macros from LittleFS
void loadMacros(char *macroArray[], const char *fileName);
