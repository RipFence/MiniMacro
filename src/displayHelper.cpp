#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "displayHelper.h"
#include "config.h"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
hw_timer_t *timer3 = NULL;

void displaySetup()
{
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; // Don't proceed, loop forever
  }
  display.clearDisplay();
  display.display();

  // Set up timer for displayShowFor()
  timer3 = timerBegin(TIMER_NUMBER, TIMER_PRESCALER, true); // Timer 0, Ticking at 100kHz or 0.001ms
  timerAttachInterrupt(timer3, &displayClear, true);        // Attach the interrupt

  // Credits for me
  displayShowFor("MiniMacro from Dad", 5000);
}

void displayLoop()
{
  // Most display commands operate on just the buffer and aren't
  // sent to the display until display() is called.
  display.display();
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
  display.println(text);
  display.display();
}

void displayShowFor(const char text[], long ms, int textSize, int x, int y)
{
  displayPrint(text, textSize, x, y);

  timerAlarmWrite(timer3, ms * 10, false); // 10 ticks = 1 ms, single-shot mode
  timerAlarmEnable(timer3);                // Enable the timer alarm
}