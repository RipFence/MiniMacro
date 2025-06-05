#include <string.h>
#include "Adafruit_TinyUSB.h"
#include "keyboardHelper.h"
#include "config.h"

// Global variables
unsigned long keyUpTime = 0;
unsigned long nextCharacterTime = 0;
char macroBuffer[MACRO_LENGTH + 1] = {0};
bool isSending = false;

// HID report descriptor using TinyUSB's template
// Single Report (no ID) descriptor
uint8_t const desc_hid_report[] = {TUD_HID_REPORT_DESC_KEYBOARD()};

// USB HID object. For ESP32 these values cannot be changed after this declaration
// desc report, desc len, protocol, interval, use out endpoint
Adafruit_USBD_HID usb_hid;

// charToKeycode() converts a character to the corresponding HID keycode
void charToKeycode(const char c, uint8_t keycode[])
{
  // Convert the character to the corresponding HID keycode
  // HID keycode can hold up to 6 simultaneous key presses
  // memset(keycode, 0, sizeof(keycode)); // Clear the array

  if (c >= 'a' && c <= 'z')
  {
    // Lowercase letters
    keycode[0] = HID_KEY_A + (c - 'a');
  }
  else if (c >= 'A' && c <= 'Z')
  {
    // Uppercase letters (requires Shift modifier)
    keycode[0] = HID_KEY_A + (c - 'A');
    keycode[1] = HID_KEY_SHIFT_LEFT;
  }
  else if (c >= '0' && c <= '9')
  {
    // Numbers 0-9
    keycode[0] = HID_KEY_0 + (c - '0');
  }
  else
  {
    // Special characters
    switch (c)
    {
    case ' ':
      keycode[0] = HID_KEY_SPACE;
      break;
    case '\n':
      keycode[0] = HID_KEY_ENTER;
      break;
    case '.':
      keycode[0] = HID_KEY_PERIOD;
      break;
    case ',':
      keycode[0] = HID_KEY_COMMA;
      break;
    case '-':
      keycode[0] = HID_KEY_MINUS;
      break;
    case '=':
      keycode[0] = HID_KEY_EQUAL;
      break;
    case ';':
      keycode[0] = HID_KEY_SEMICOLON;
      break;
    case '\'':
      keycode[0] = HID_KEY_APOSTROPHE;
      break;
    case '/':
      keycode[0] = HID_KEY_SLASH;
      break;
    case '[':
      keycode[0] = HID_KEY_BRACKET_LEFT;
      break;
    case ']':
      keycode[0] = HID_KEY_BRACKET_RIGHT;
      break;
    case '\\':
      keycode[0] = HID_KEY_BACKSLASH;
      break;
    case '`':
      keycode[0] = HID_KEY_GRAVE;
      break;
    case ':':
      keycode[0] = HID_KEY_SEMICOLON;
      keycode[1] = HID_KEY_SHIFT_LEFT;
      break;
    case '"':
      keycode[0] = HID_KEY_APOSTROPHE;
      keycode[1] = HID_KEY_SHIFT_LEFT;
      break;
    case '?':
      keycode[0] = HID_KEY_SLASH;
      keycode[1] = HID_KEY_SHIFT_LEFT;
      break;
    case '{':
      keycode[0] = HID_KEY_BRACKET_LEFT;
      keycode[1] = HID_KEY_SHIFT_LEFT;
      break;
    case '}':
      keycode[0] = HID_KEY_BRACKET_RIGHT;
      keycode[1] = HID_KEY_SHIFT_LEFT;
      break;
    case '|':
      keycode[0] = HID_KEY_BACKSLASH;
      keycode[1] = HID_KEY_SHIFT_LEFT;
      break;
    case '_':
      keycode[0] = HID_KEY_MINUS;
      keycode[1] = HID_KEY_SHIFT_LEFT;
      break;
    case '+':
      keycode[0] = HID_KEY_EQUAL;
      keycode[1] = HID_KEY_SHIFT_LEFT;
      break;
    case '~':
      keycode[0] = HID_KEY_GRAVE;
      keycode[1] = HID_KEY_SHIFT_LEFT;
      break;
    case '!':
      keycode[0] = HID_KEY_1;
      keycode[1] = HID_KEY_SHIFT_LEFT;
      break;
    case '@':
      keycode[0] = HID_KEY_2;
      keycode[1] = HID_KEY_SHIFT_LEFT;
      break;
    case '#':
      keycode[0] = HID_KEY_3;
      keycode[1] = HID_KEY_SHIFT_LEFT;
      break;
    case '$':
      keycode[0] = HID_KEY_4;
      keycode[1] = HID_KEY_SHIFT_LEFT;
      break;
    case '%':
      keycode[0] = HID_KEY_5;
      keycode[1] = HID_KEY_SHIFT_LEFT;
      break;
    case '^':
      keycode[0] = HID_KEY_6;
      keycode[1] = HID_KEY_SHIFT_LEFT;
      break;
    case '&':
      keycode[0] = HID_KEY_7;
      keycode[1] = HID_KEY_SHIFT_LEFT;
      break;
    case '*':
      keycode[0] = HID_KEY_8;
      keycode[1] = HID_KEY_SHIFT_LEFT;
      break;
    case '(':
      keycode[0] = HID_KEY_9;
      keycode[1] = HID_KEY_SHIFT_LEFT;
      break;
    case ')':
      keycode[0] = HID_KEY_0;
      keycode[1] = HID_KEY_SHIFT_LEFT;
      break;
    }
  }
}

void _processMacroBuffer()
{
  // Read Position in the Macro Buffer
  static int bufferIndex = 0;

  // Don't send the next key if keypress is already in progress
  // or too soon after last keypress
  if (keyUpTime > 0 || millis() < nextCharacterTime)
    return;

  // Get the current character in the buffer and advance
  char c = macroBuffer[bufferIndex];
  bufferIndex++;
  if (bufferIndex >= strlen(macroBuffer))
  {
    bufferIndex = 0;
    macroBuffer[0] = 0; // empty the buffer
  }

  // Convert the character to the corresponding HID keycode
  uint8_t keycode[6] = {0};
  charToKeycode(c, keycode);

  // "Press" the key by sending the keycode
  if (keycode[0] != 0)
  {
    const uint8_t modifier = 0;
    usb_hid.keyboardReport(RID_KEYBOARD, modifier, keycode); // Pass the array
  }

  // Set the time to wait for lifting the keypress
  keyUpTime = millis() + KEYDOWN_DELAY;
}

void _processKeyUp()
{
  // If the key is not pressed or the release time has not yet passed, do nothing
  if (keyUpTime == 0 || millis() < keyUpTime)
    return;

  // Release the key to signal key up
  usb_hid.keyboardRelease(RID_KEYBOARD);
  keyUpTime = 0;

  // If there are more characters to send, Set the time to wait for the next character
  if (macroBuffer[0] != 0)
    nextCharacterTime = millis() + INTER_CHARACTER_DELAY;
  else
  {
    isSending = false;
    nextCharacterTime = 0;
  }
}

// keyboardSetup() should be called once in the main setup() function
void keyboardSetup()
{
  if (!TinyUSBDevice.isInitialized())
  {
    TinyUSBDevice.begin(0);
  }

  // Set up HID
  usb_hid.setBootProtocol(HID_ITF_PROTOCOL_KEYBOARD);
  usb_hid.setPollInterval(2); // Default 10
  usb_hid.setReportDescriptor(desc_hid_report, sizeof(desc_hid_report));
  usb_hid.setStringDescriptor("MiniMacro Keyboard");
  // Set up output report (on control endpoint) for Capslock indicator
  // usb_hid.setReportCallback(NULL, hid_report_callback);

  usb_hid.begin();

  // If already enumerated, hid won't take effect until re-enumeration
  if (TinyUSBDevice.mounted())
  {
    TinyUSBDevice.detach();
    delay(10);
    TinyUSBDevice.attach();
  }
}

// processHID() should be called in the main loop
void processHID()
{
  if (isSending)
  {
    _processMacroBuffer();
    _processKeyUp();
  }
}

// sendString() sends a string of characters to the keyboard
bool sendString(char inputString[])
{
  if (isSending)
  {
    return false;
  }

  // Validate that the input string is not too long
  if (strlen(inputString) > MACRO_LENGTH)
  {
    return false;
  }

  isSending = true;
  // Copy the input string to the macro buffer
  strcpy(macroBuffer, inputString);
  return true;
}
