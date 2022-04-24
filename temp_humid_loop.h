#ifndef TEMP_HUMID_LOOP_H
#define TEMP_HUMID_LOOP_H

#include "temp_humid.h"

class TempHumidLoop
{
private:
  //other constants
  static constexpr unsigned long POLL_DELAY_MS{3000};
  // Temp and Humidity manager class
  TempHumid th;
public:
   TempHumidLoop(DHT_nonblocking &dh) : th(dh, POLL_DELAY_MS)
   {
    
   }
  String DoIteration()
  {
    th(); // functor to attempt update if new value available
    const auto res = th.GetUpdatedF(); // get value, new or old.
    const String result = "Temp = " + String(res.temp) + "deg. F\n -- \nHumidity = " + String(res.humid) + "%";
    return result;
  }
  bool IsAboveF(const int16_t farenTempValue)
  {
    const auto dataPack = th.GetUpdatedF();
    return dataPack.temp > farenTempValue;
  }
};

#endif
