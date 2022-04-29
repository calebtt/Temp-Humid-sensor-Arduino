#ifndef TEMP_HUMID_LOOP_H
#define TEMP_HUMID_LOOP_H

#include "temp_humid.h"

//program specific class for doing an iteration of temp and humid checking
//and also activate the relay with the values.
class TempHumidLoop
{
public:
  //function object base class for callbacks
  class FunctorBase
  {
  public:
    //pure virtual operator() to be overridden in a derived class for use
    virtual void operator()(const bool) = 0;
  };
  //pair type because no <utility> header available
  struct TempHumidPair
  {
    const String tempString;
    const String humidString;
  };
private:
  //other constants
  static constexpr unsigned long POLL_DELAY_MS{3000};
  // Temp and Humidity manager class
  TempHumid th;
public:
   TempHumidLoop(DHT_nonblocking &dh) : th(dh, POLL_DELAY_MS)
   {
    
   }
  TempHumidPair DoIteration(FunctorBase &&fn, uint8_t relayTempValue)
  {
    th(); // operator overload to attempt update if new value available
    const auto res = th.GetUpdatedF(); // get value, new or old.
    TempHumidPair retVal { ("Temp = " + String(res.temp)), ("Humidity = " + String(res.humid) + "%") };
    fn(IsAboveF(relayTempValue));
    return retVal;
  }
private:
  bool IsAboveF(const int16_t farenTempValue)
  {
    const auto dataPack = th.GetUpdatedF();
    return dataPack.temp > farenTempValue;
  }
};

#endif
