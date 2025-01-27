#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "displayHelper.h"
#include "config.h"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
unsigned long ClearDisplayTime = 0;
unsigned long screenOffTimeout = 1;

void displaySetup()
{
  // Setup I2C Pins
  pinMode(SDA_PIN, OUTPUT);
  pinMode(SCL_PIN, OUTPUT);
  pinMode(DISPLAY_POWER_PIN, OUTPUT);    // Turns Display on/off
  digitalWrite(DISPLAY_POWER_PIN, HIGH); // turn on display

  // Initialize the I2C interface
  Wire.setPins(SDA_PIN, SCL_PIN);
  Wire.begin();

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    displayPrint("SSD1306 failed", 1);
    for (;;)
      ; // Don't proceed, loop forever
  }

  // Credits for me
  displayOn();
  displayLogo();
}

void displayLoop()
{
  // Time to clear the display?
  if (ClearDisplayTime > 0 && millis() >= ClearDisplayTime)
  {
    ClearDisplayTime = 0;
    displayClear();
  }

  // Time to turn the display off?
  if (screenOffTimeout > 1 && millis() >= screenOffTimeout)
  {
    displayOff();
  }
}

void displayClear()
{
  display.clearDisplay();
  display.display();
}

void displayPrint(const char text[], int textSize, int x, int y)
{
  ClearDisplayTime = 0;
  if (!displayIsOn())
  {
    displayOn();
  }
  display.clearDisplay();
  display.setTextSize(textSize);
  display.setTextColor(WHITE);
  display.setCursor(x, y);
  display.print(text);
  display.display();
  if (screenOffTimeout != 0)
    screenOffTimeout = millis() + SCREEN_OFF_TIMEOUT;
}

void displayShowFor(const char text[], unsigned long ms, int textSize, int x, int y)
{
  if (!displayIsOn())
  {
    displayOn();
  }
  displayPrint(text, textSize, x, y);
  ClearDisplayTime = millis() + ms;
  if (screenOffTimeout != 0)
  {
    screenOffTimeout = millis() + ms + SCREEN_OFF_TIMEOUT;
  }
}

void displayLogo()
{
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(7, 0);
  display.print(SPLASHMESSAGE);
  display.setTextSize(1);
  display.setCursor(32, 24);
  display.print("Thanks Dad!");
  display.display();
  delay(3000);
  ClearDisplayTime = millis() + 10000;
  if (screenOffTimeout != 0)
    screenOffTimeout = ClearDisplayTime + SCREEN_OFF_TIMEOUT;
}
void displayReady()
{
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(20, 0);
  display.print("Ready");
  display.setTextSize(1);
  display.setCursor(0, 24);
  display.print("Setup:minimacro.local");
  display.display();
  ClearDisplayTime = millis() + 30000;
  if (screenOffTimeout != 0)
    screenOffTimeout = ClearDisplayTime + SCREEN_OFF_TIMEOUT;
}
void displayOn()
{
  digitalWrite(DISPLAY_POWER_PIN, HIGH);
  displayClear();
  if (screenOffTimeout != 0)
    screenOffTimeout = millis() + SCREEN_OFF_TIMEOUT;
}
void displayOff()
{
  digitalWrite(DISPLAY_POWER_PIN, LOW);
  if (screenOffTimeout != 0)
    screenOffTimeout = 1;
}
bool displayIsOn() { return digitalRead(DISPLAY_POWER_PIN) == HIGH; }
