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
#include <SimpleRotary.h>

#include "display_oled.h"
#include "temp_humid_loop.h"

//DHT temp humid sensor setup
static constexpr uint8_t DHT_SENSOR_TYPE PROGMEM {DHT_TYPE_11};
static constexpr uint8_t DHT_SENSOR_PIN PROGMEM {7};
DHT_nonblocking dh(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);
TempHumidLoop thl(dh);

//display stuff
static constexpr uint8_t WIDTH PROGMEM {128}; // OLED display width, in pixels
static constexpr uint8_t HEIGHT PROGMEM {64}; // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
static constexpr uint8_t OLED_RESET PROGMEM { 4 }; // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 ssd(WIDTH, HEIGHT, &Wire, OLED_RESET);
DisplayOled displayManager(ssd);

//wifi stuff TODO

//relay stuff
static constexpr uint8_t RELAY_PIN PROGMEM {10};
int TEMP_LIMIT{80};

//rotary encoder stuff
static constexpr int8_t PROGMEM EncoderPinA{ 2 }; 
static constexpr int8_t PROGMEM EncoderPinB{ 3 }; 
static constexpr int8_t PROGMEM EncoderPinP{ 4 };
SimpleRotary rotary(EncoderPinA,EncoderPinB,EncoderPinP);
int rotaryCount{};

// a function object for performing the relay toggle functionality
//based on data in the temp_humid_loop file.
//I think it's better than just a function pointer type callback,
//as it may have encapsulated data or functions of it's own.
class RelayCallback : public TempHumidLoop::FunctorBase
{
public:
  virtual void operator()(const bool isAboveTemp) override
  {
  if(isAboveTemp)
    digitalWrite(RELAY_PIN, HIGH);
  else
     digitalWrite(RELAY_PIN, LOW);
  }
};

// Updates the temp limit variable using info from a rotary encoder.
void UpdateTempLimitWithEncoder(int &tempLimit)
{
  const int rotaryEnumValue = rotary.rotate();
  int updateValue{};
  if( rotaryEnumValue == 1 )
  updateValue = -1;
  if( rotaryEnumValue == 2 )
  updateValue = 1;
  tempLimit += updateValue;
}

/* Initialize the serial port. */
void setup( )
{
  Serial.begin( 9600 );
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
  displayManager.InitDisplay();
  rotary.setTrigger(HIGH);
  rotary.setDebounceDelay(5);
  rotary.setErrorDelay(5);
  rotaryCount = rotary.rotate();
}

/* Main program loop. */
void loop()
{
  const String LineSep = "\n -- \n";
  const auto [tempString, humidString] = thl.DoIteration(RelayCallback{}, TEMP_LIMIT);
  String tempHumidString = tempString + LineSep + humidString;
  String encoderString = LineSep + "Encoder report: " + String(rotaryCount);
  String msg = tempHumidString + encoderString + LineSep + "Relay temp limit: " + TEMP_LIMIT;
  UpdateTempLimitWithEncoder(TEMP_LIMIT);
  displayManager.DrawMessage(msg);
}
