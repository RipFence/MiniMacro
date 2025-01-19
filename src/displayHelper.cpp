#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "displayHelper.h"
#include "config.h"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
unsigned long ClearDisplayTime = 0;

void displaySetup()
{
  Serial.println("Setting up display");
  // Setup I2C Pins
  pinMode(SDA_PIN, OUTPUT);
  pinMode(SCL_PIN, OUTPUT);
  pinMode(DISPLAY_POWER_PIN, OUTPUT); // Turns Display on/off
  digitalWrite(DISPLAY_POWER_PIN, HIGH);

  // Initialize the I2C interface
  Wire.setPins(SDA_PIN, SCL_PIN);
  Wire.begin();

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; // Don't proceed, loop forever
  }

  // Credits for me
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
}

void displayClear()
{
  display.clearDisplay();
  display.display();
}

void displayPrint(const char text[], int textSize, int x, int y)
{
  display.clearDisplay();
  display.setTextSize(textSize);
  display.setTextColor(WHITE);
  display.setCursor(x, y);
  display.print(text);
  display.display();
}

void displayShowFor(const char text[], unsigned long ms, int textSize, int x, int y)
{
  displayPrint(text, textSize, x, y);
  ClearDisplayTime = millis() + ms;
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
  // ClearDisplayTime = millis() + 10000;
}