#include "display_oled.h"


//static DisplayOled::Adafruit_SSD1306 displayManager(128, 64, &Wire, 4);
DisplayOled::DisplayOled(Adafruit_SSD1306 &disp) : displayManager(disp)
{  }

void DisplayOled::DrawMessage(String message)
{
  displayManager.clearDisplay();
  displayManager.setTextSize(1);              // Normal 1:1 pixel scale
  displayManager.setTextColor(SSD1306_WHITE); // Draw white text
  displayManager.setCursor(0, 0);             // Start at top-left corner
  displayManager.cp437(true);                 // Use full 256 char 'Code Page 437' font
  for(const auto elem : message)
     displayManager.write(elem);
  displayManager.display();
}

void DisplayOled::InitDisplay()
{
  const String MSG_DISPLAY_ALLOC = "SSD1306 allocation failed.";
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!displayManager.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) 
  {
    Serial.print(MSG_DISPLAY_ALLOC.c_str());
    //don't proceed, infinite loop
    while(true)
    { }
  }
  //clear display
  displayManager.clearDisplay();
}
