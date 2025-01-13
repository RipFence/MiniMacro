// config.h
#ifndef CONFIG_H
#define CONFIG_H

// WiFi Options
#define WIFI_SSID "MiniMacro_5D4C"
#define WIFI_PASSWORD "password"

// Macro Options
#define MACRO_LENGTH 2048
#define EMPTY_BUTTON_TEXT "Empty Button"

// Button Options
#define DEBOUNCE_DELAY 20
#define ACTIVE_LOW true // true if button is pulled low upon press

// Storage Options
#define STORAGE_VERSION "1.0.0"
#define SINGLE_MACRO_FILE "/single.txt"
#define DOUBLE_MACRO_FILE "/double.txt"

// Display Options
#define TIMER_NUMBER 0
#define TIMER_PRESCALER 8000 // (80MHz / 8000) = 10kHz = 0.01ms

// Keyboard Options
#define INTER_CHARACTER_DELAY 250 // Wait in ms between characters
#define KEYDOWN_DELAY 10          // Wait in ms before key is released
#define MAX_MACRO_LENGTH 1023     // Maximum length of a macro
#define RID_KEYBOARD 0            // Keyboard report ID

#endif // CONFIG_H