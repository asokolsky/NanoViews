#ifndef Views_h
#define Views_h

/**
 * This is a NANO-sized windowing system for Arduino with TFT displays.
 * Low level support provided by ILI9341_t3 or rather the fork of it by blackketter
 * For sample of use see Views2.h
 */

#include "Display.h"
#include "Widgets.h"


/**
 * View is a widget with a title and bottom bar.  
 * Kinda like Application or Top Level window in Windows.
 */
class View : public Widget
{
protected:
 
  bool m_bEraseBkgnd = true;
  const char *m_szTitle;

  /**  update period 1/2 sec */
  static const unsigned long ulUpdatePeriod = 100;
  /** when the view has to be updated */
  static unsigned long m_ulToUpdate;

  static bool needsUpdate(unsigned long now) {
    return (now >= m_ulToUpdate);
  }

  /** 
   *  Parent of a modal dialog.
   *  Move it to Widget?
   */
  View *m_zParent = 0;
  
  const static uint8_t MAXKIDS = 4;
  
  Widget *m_zChildren[MAXKIDS];

  bool addChild(Widget *pChild) {
    for(uint8_t i = 0; i < MAXKIDS; i++) {
      if(m_zChildren[i] == 0) {
        m_zChildren[i] = pChild;
        return true;
      }
    }
    // we failed to add a child!
    // melt the core!
    return false;
  }
  
public:  
  View(const char *szTitle);
  
  /** The Active View */
  static View *g_pActiveView;
  /** The Previous View */
  static View *g_pPreviousView;
  
  /** once in application lifetime initialization */  
  static void setup();
  
  /** Activate the View.  Will call onDeActivate and onActivate */
  static void activate(View *p);
  /** view DEactivation call-back */
  virtual void onDeActivate(View *pAboutToBeActive);
  /** view activation call-back */
  virtual void onActivate(View *pPrevActive);

  /** analog keyboard APIs where vk is one of VK_xxx */
  virtual bool onKeyDown(uint8_t vk);
  virtual bool onKeyAutoRepeat(uint8_t vk);
  virtual bool onLongKeyDown(uint8_t vk);
  virtual bool onKeyUp(uint8_t vk);
  virtual bool onKeysInactive();

  /** ThumbStick APIs where vk is one of VK_xxx */
  //virtual void onThumbDown() = 0;
  //virtual void onThumbUp() = 0;
  //virtual void onLongThumbDown() = 0;
  //virtual void onThumbStickX(int16_t x) = 0;
  //virtual void onThumbStickY(int16_t y) = 0;
  
  /** to be called from the main loop on the active view.  Do nothing by default. return TRUE to update the display.  */
  virtual bool loop(unsigned long now);

  /** recalc the client rect */
  void onPosition();


  /** might as well update GUI is its time*/
  void updateMaybe(unsigned long now);
  /** entire screen redraw */
  void update(unsigned long now);

  /** 
   *  redraw client area only, not including title and bottom bar 
   *  Refer to g_rectClient for dimensions - changing those in updateClient is not kosher!
   */
  virtual void updateClient(unsigned long now);

#ifdef DEBUG
  void DUMP(const char *szText = 0) const;
#else
  void DUMP(const char *szText = 0) {}
#endif

protected:

  /** draws the title bar. */
  virtual void drawTitleBar();

};





#endif

