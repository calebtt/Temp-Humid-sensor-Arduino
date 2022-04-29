#ifndef DISPLAYOLED_H
#define DISPLAYOLED_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <Wire.h>
#include <SD.h>
#include "delay_manager.h"


// Class for interacting with my SSD1306 display, program specific logic
// for say, writing some temp and humidity info gathered from a sensor.
class DisplayOled
{
private:
  static constexpr uint8_t PROGMEM SCREEN_ADDRESS{ 0x3C };
  static constexpr uint16_t PROGMEM DELAY_UPDATE_MS{ 200 };
  Adafruit_SSD1306 &displayManager;
  DelayManager dmgr{DELAY_UPDATE_MS};
public:
  //ctor
  DisplayOled(Adafruit_SSD1306 &disp);
  void DrawMessage(String message);
  void InitDisplay();
};

#endif
