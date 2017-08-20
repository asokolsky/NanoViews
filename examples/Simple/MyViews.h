/**
 * Views used in this sample
 */

class ViewLed : public View
{
  bool m_bLedOn = false;
public:
  ViewLed() : View("LED Control")
  {
  }
  bool onKeyUp(uint8_t vk);

};
extern ViewLed g_viewLed;

/**
 * This view just displays firmware version information.
 */
class ViewAbout : public View
{
public:
  ViewAbout() : View("About")
  {
  }

};
extern ViewAbout g_viewAbout;

