/**
 * Views used in this sample
 */

class ViewLed : public View
{
  bool m_bLedOn = false;
  TextWidget m_tw;

public:
  ViewLed() : View("LED Control")
  {
    addChild(&m_tw);
  }
  bool onKeyUp(uint8_t vk);

};
extern ViewLed g_viewLed;

/**
 * This view just displays firmware version information.
 */
class ViewAbout : public View
{
  TextWidget m_tw;
public:
  ViewAbout() : View("About")
  {
    addChild(&m_tw);
    m_tw.setText("NanoViews Test 0.1");
  }

};
extern ViewAbout g_viewAbout;

