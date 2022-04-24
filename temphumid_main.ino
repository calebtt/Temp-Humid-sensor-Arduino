#include <DHT.h>
#include <DHT_U.h>
#include <dht_nonblocking.h>
#include <Wire.h>
#include <assert.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <splash.h>
#include <SD.h>
#include <WiFiNINA.h>

#include "display_oled.h"
#include "temp_humid_loop.h"

//constants
static constexpr uint16_t POLL_DELAY_MS{1000};

//DHT temp humid sensor setup
static constexpr uint8_t DHT_SENSOR_TYPE{DHT_TYPE_11};
static constexpr uint8_t DHT_SENSOR_PIN{7};
DHT_nonblocking dh(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);
TempHumidLoop thl(dh);

//display stuff
static constexpr uint8_t WIDTH = 128; // OLED display width, in pixels
static constexpr uint8_t HEIGHT = 64; // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
static constexpr uint8_t OLED_RESET{ 4 }; // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 ssd(WIDTH, HEIGHT, &Wire, OLED_RESET);
DisplayOled displayManager(ssd);

//wifi stuff TODO

//relay stuff
static constexpr uint8_t RELAY_PIN{10};
static uint8_t TEMP_LIMIT{80};


/* Initialize the serial port. */
void setup( )
{
  Serial.begin( 9600 );
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
  displayManager.InitDisplay();
}

/* Main program loop. */
void loop()
{
  String tempHumidString = thl.DoIteration();
  const bool isAboveTemp = thl.IsAboveF(TEMP_LIMIT);
  if(isAboveTemp)
    digitalWrite(RELAY_PIN, HIGH);
  else
     digitalWrite(RELAY_PIN, LOW);
  String msg = tempHumidString + "\n -- \nRelay temp limit: " + TEMP_LIMIT;
  displayManager.DrawMessage(msg);
  //displayManager.DrawMessage("Test...");
  delay(POLL_DELAY_MS);
}
