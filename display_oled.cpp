#include "display_oled.h"

DisplayOled::DisplayOled(Adafruit_SSD1306 &disp) : displayManager(disp)
{  }

void DisplayOled::InitDisplay()
{
  const String MSG_DISPLAY_ALLOC = "SSD1306 allocation failed.";
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!displayManager.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    Serial.print(MSG_DISPLAY_ALLOC.c_str());
    //don't proceed, infinite loop
    while (true)
    { }
  }
  //clear display
  displayManager.clearDisplay();
}

void DisplayOled::DrawMessage(String message)
{
  if ( dmgr.IsElapsed() )
  {
    displayManager.clearDisplay();
    displayManager.setTextSize(1);              // Normal 1:1 pixel scale
    displayManager.setTextColor(SSD1306_WHITE); // Draw white text
    displayManager.setCursor(0, 0);             // Start at top-left corner
    displayManager.cp437(true);                 // Use full 256 char 'Code Page 437' font
    for (const auto elem : message)
      displayManager.write(elem);
    displayManager.display();
    dmgr.Reset(DELAY_UPDATE_MS);
  }
}
