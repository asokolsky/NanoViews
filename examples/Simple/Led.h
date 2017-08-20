
class Led
{
  /** the overheating led pin - use a built-in one */
  uint8_t m_pinLed;

public:
  Led(uint8_t pinLed) : m_pinLed(pinLed)
  {
    pinMode(m_pinLed, OUTPUT);
  }

  void turnOn()
  {
    digitalWrite(m_pinLed, HIGH);
  }
  void turnOff()
  {
    digitalWrite(m_pinLed, LOW);
  }
  
};

extern Led g_led;

