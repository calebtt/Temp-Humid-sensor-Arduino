#include <DHT.h>
#include <DHT_U.h>
#include <dht_nonblocking.h>
#include <assert.h>

static constexpr int DHT_SENSOR_TYPE{DHT_TYPE_11};
static constexpr int DHT_SENSOR_PIN{2};
static constexpr unsigned long POLL_DELAY_MS{3000};
DHT_nonblocking dht_sensor( DHT_SENSOR_PIN, DHT_SENSOR_TYPE );

//Temp and humidity manager
struct TempHumid
{
public:
  struct TempHumidData
  {
    using SensType = float;
    SensType temp{};
    SensType humid{};
  };
public:
  using TimeType = unsigned long;
  const TimeType DELAY_TIMESTAMP_MS;
  TimeType m_last_time{0ul};
  TempHumidData m_previous_data{};
  bool m_is_elapsed{false};
public:
  explicit TempHumid(TimeType interval) : DELAY_TIMESTAMP_MS(interval) { assert(interval > 0); }
  bool operator()() noexcept
  {
    if( millis( ) - m_last_time > DELAY_TIMESTAMP_MS )
    {
      if( dht_sensor.measure(&m_previous_data.temp, &m_previous_data.humid) )
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
};

// global Instance of Temp and Humidity manager class.
TempHumid th(POLL_DELAY_MS); // poll every 3000 milliseconds

/* Initialize the serial port. */
void setup( )
{
  Serial.begin( 9600 );
}

/* Main program loop. */
void loop()
{
  if( th() )
  {
    const auto res = th.GetUpdated();
    Serial.print( "T = " );
    Serial.print( res.temp, 1 );
    Serial.print( " deg. C, H = " );
    Serial.print( res.humid, 1 );
    Serial.println( "%" );
  }
}
