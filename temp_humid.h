#ifndef TEMP_HUMID_H
#define TEMP_HUMID_H

#include <DHT.h>
#include <DHT_U.h>
#include <dht_nonblocking.h>
#include <Wire.h>
#include <assert.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SD.h>

class TempHumid
{
public:
//nested struct to hold some temphumid sensor data
  struct TempHumidData
  {
    using SensType = float;
    SensType temp{};
    SensType humid{};
  };
private:
  using TimeType = unsigned long;
  DHT_nonblocking &m_dht_sensor;
  const TimeType DELAY_TIMESTAMP_MS;
  TimeType m_last_time{0ul};
  TempHumidData m_previous_data{};
  bool m_is_elapsed{false};
public:
//public interface
  TempHumid() = delete;
  explicit TempHumid(DHT_nonblocking &sens, TimeType interval) : m_dht_sensor(sens), DELAY_TIMESTAMP_MS(interval)
  { 
    assert(interval > 0); 
  }
  bool operator()() noexcept
  {
    if( millis( ) - m_last_time > DELAY_TIMESTAMP_MS )
    {
      if( m_dht_sensor.measure(&m_previous_data.temp, &m_previous_data.humid) )
      {
        m_last_time = millis( );
        return true;
       }
    }
    return false;
  }
  TempHumidData GetUpdated() const
  {
    return m_previous_data;
  }
  TempHumidData GetUpdatedF() const
  {
    const auto far = (9.0f/5.0f) * m_previous_data.temp + 32.0f;
    TempHumidData returnVal;
    returnVal.temp = far;
    returnVal.humid = m_previous_data.humid;
    return returnVal;;
  }
};

#endif
