#ifndef DELAY_MANAGER_H
#define DELAY_MANAGER_H

class DelayManager
{
    using TimeType = unsigned long;
    TimeType m_start_time{ millis() };
    TimeType m_duration{ 1 };
    bool m_has_fired{ false };
  public:
    DelayManager() = delete;
    //ms is milliseconds
    explicit DelayManager(size_t duration_ms) : m_duration(duration_ms) { }
    DelayManager(const DelayManager& other) = default;
    DelayManager(DelayManager&& other) = default;
    DelayManager& operator=(const DelayManager& other) = default;
    DelayManager& operator=(DelayManager&& other) = default;
    ~DelayManager() = default;
    /// Check for elapsed.
    bool IsElapsed() noexcept
    {
      if (millis() -m_start_time > m_duration)
      {
        m_has_fired = true;
        return true;
      }
      return false;
    }
    /// Reset delay for elapsing.
    void Reset(TimeType millisec_delay) noexcept
    {
      m_start_time = millis();
      m_has_fired = false;
      m_duration = millisec_delay;;
    }
};



#endif
