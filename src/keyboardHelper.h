#pragma once

// keyboardSetup() should be called once in the main setup() function
void keyboardSetup();

// processHID() should be called in the main loop
void processHID();

// sendString() sends a string of characters to the keyboard
bool sendString(char inputString[]);
