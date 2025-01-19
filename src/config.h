// config.h
#ifndef CONFIG_H
#define CONFIG_H

// WiFi Access Point Options
#define WIFI_SSID "MiniMacro"
#define WIFI_PASSWORD ""
#define WIFI_CONNECT_TIMEOUT 10 // in seconds
#define WIFI_CONFIG_FILENAME "/wifi.dat"
#define TRIGGER_PIN 0

// Macro Options
#define MACRO_LENGTH 2048
#define EMPTY_BUTTON_TEXT "Empty Button"

// Button Options
#define DEBOUNCE_DELAY 20
#define ACTIVE_LOW true // true if button is pulled low upon press
// Avoid GPIO 0, 2, 4, 12, 13, 14, 15, 25, 26 and 27 as they're used by WIFI
#define RED_BUTTON_PIN 1
#define GREEN_BUTTON_PIN 21
#define BLUE_BUTTON_PIN 40
#define YELLOW_BUTTON_PIN 6
#define BLACK_BUTTON_PIN 8
#define WHITE_BUTTON_PIN 10

// Storage Options
#define STORAGE_VERSION "1.0.0"
#define SINGLE_MACRO_FILE "/single.txt"
#define DOUBLE_MACRO_FILE "/double.txt"

// Display Options
#define DISPLAY_POWER_PIN 38
#define SDA_PIN 34 // I2C
#define SCL_PIN 36 // I2C
#define SPLASHMESSAGE "MiniMacro"
#define TIMER_NUMBER 0
#define TIMER_PRESCALER 8000 // (80MHz / 8000) = 10kHz = 0.01ms

// Keyboard Options
#define INTER_CHARACTER_DELAY 250 // Wait in ms between characters
#define KEYDOWN_DELAY 10          // Wait in ms before key is released
#define MAX_MACRO_LENGTH 1023     // Maximum length of a macro
#define RID_KEYBOARD 0            // Keyboard report ID

#endif // CONFIG_H