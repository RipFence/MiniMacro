#pragma once

// setupKeyboard() should be called once in the main setup() function
void setupKeyboard();

// processHID() should be called in the main loop
void processHID();

// sendString() sends a string of characters to the keyboard
bool sendString(char inputString[]);
