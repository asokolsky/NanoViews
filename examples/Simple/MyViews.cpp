#include "NanoViews.h"
#include "Led.h"
#include "MyViews.h"
#include "AnalogNavigationKeypad.h"

/**
 * ViewLed implementation
 */

ViewLed g_viewLed;

bool ViewLed::onKeyUp(uint8_t vk) 
{
  switch(vk)
  {
    case VK_UP:
      m_bLedOn = true;
      break;
    case VK_DOWN:
      m_bLedOn = false;
      break;
    default:
      return false;
  }
  if(m_bLedOn)
  {
    g_led.turnOn();
    m_tw.setText("LED is ON");
  }
  else
  {
    g_led.turnOff();
    m_tw.setText("LED is OFF");
  }
  return true;
}

/**
 * ViewAbout implementation
 */

 ViewAbout g_viewAbout;