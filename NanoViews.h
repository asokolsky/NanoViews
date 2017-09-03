/**
 * NanoViews
 * Tiny GUI C++ library for Arduino
 *
 * This is a NANO-sized windowing system for Arduino with TFT displays.
 * Low level support provided by either
 *  - ILI9341_t3 or rather the fork of it by blackketter
 *  - or u8g2
 *
 */
#ifndef NanoViews_h
#define NanoViews_h

#include <Arduino.h>
#include <Wire.h>
#include <U8g2lib.h>
#include <assert.h>

#include "Trace.h"
#include "Display.h"


/**
 * This is a NANO-sized windowing system for Arduino with TFT displays.
 * Low level support provided by either
 *  - ILI9341_t3 or rather the fork of it by blackketter
 *  - or u8g2
 */

 class View;
 
 const uint16_t WS_VISIBLE   =0x0001;
 const uint16_t WS_CHILD     =0x0002;
 const uint16_t WS_BORDER    =0x0004;
 const uint16_t WS_TITLEBAR  =0x0008;
 const uint16_t WS_HSCROLL   =0x0010;
 const uint16_t WS_VSCROLL   =0x0020;
 const uint16_t WS_LEFTTEXT  =0x0100;
 const uint16_t WS_CENTERTEXT=0x0200;
 const uint16_t WS_HASFOCUS  =0x1000;
  /**
   *  Root object that can be displayed on the screen
   */
 class Widget
 {
 protected:
   uint16_t m_uStyle = 0;
   
 public:
   /** this is position of the widget */
   RECT m_position;
   /** this is client area - not including title, border, etc */
   RECT m_rectClient;
   /** Text font - if any  */
   const uint8_t *m_pFont = 0;
   
   Widget(const uint8_t *pFont = 0, uint16_t uStyle = 0) : m_pFont(pFont) {}
 
   bool hasFocus() const {
     return (m_uStyle & WS_HASFOCUS) != 0;
   }
   void hasFocus(bool bHasFocus) {
     if(bHasFocus)
       m_uStyle |= WS_HASFOCUS;
     else
       m_uStyle &= ~WS_HASFOCUS;
   }
   bool hasBorder() const {
     return (m_uStyle & WS_BORDER) != 0;
   }
   void hasBorder(bool bHasBorder) {
     if(bHasBorder)
       m_uStyle |= WS_BORDER;
     else
       m_uStyle &= ~WS_BORDER;
   }
 
   virtual void draw();
   /** 
    *  m_position was just changed.  Default implementation updates m_rectClient
    */
   virtual void onPosition();
 
   /**
    *  Print Key: Val with ':' being at x
    */
   void printKeyVal(uint16_t x, uint16_t y, const char *szKey1, long lVal1, bool bSelected = false, const char *szKey2 = 0, long lVal2 = 0);
   
   /** 
    * move the widget here.  Will change m_position and trigger on Position 
    */
   void setPosition(const RECT r);
 
   void setFont(const uint8_t *pFont = 0) {
     m_pFont = pFont; 
   }
   const uint8_t *getFont() const {
     return m_pFont;
   }
   /*unsigned char getLineSpace() {
     return (m_pFont != 0) ? m_pFont->line_space : 0;
   }*/
 
#ifdef DEBUG
  virtual void DUMP(const char *szText = 0) const;
#else
  void DUMP(const char *szText = 0) {}
#endif
};
 
 /**
  * Static Text Widget with no scroll
  */
class TextWidget : public Widget
{
public:
  TextWidget(const uint8_t *pFont = 0) : 
    Widget(pFont) 
  {
    m_uStyle |= WS_CHILD;
    m_szText[0] = '\0';
  }
  /** Set window title */
  void setText(const char *msg)
  {
    strncpy(m_szText, msg, sizeof(m_szText));
    m_szText[sizeof(m_szText) - 1] = '\0';
  }
  /** By default text is painted centered */
  void draw();

#ifdef DEBUG
  void DUMP(const char *szText = 0) const;
#else
  void DUMP(const char *szText = 0) {}
#endif  
protected:
  char m_szText[40];
};
 
/**
 * View is a widget with a title and bottom bar.  
 * Kinda like Application or Top Level window in Windows.
 */
class View : public TextWidget
{ 
public:  
  View(const char *szTitle);
  
  /** The Active View */
  static View *g_pActiveView;
  /** The Previous View */
  static View *g_pPreviousView;
  
  /** once in application lifetime initialization */  
  static void setup(Display *g_pDisplay);
  
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

  
  bool m_bEraseBkgnd = true;

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
  
  //const static uint8_t MAXKIDS = 4;
  
  Widget *m_zChildren[2];

  bool addChild(Widget *pChild) 
  {
    for(uint8_t i = 0; i < (sizeof(m_zChildren)/sizeof(m_zChildren[0])); i++) 
    {
      if(m_zChildren[i] == 0) 
      {
        m_zChildren[i] = pChild;
        return true;
      }
    }
    // we failed to add a child!
    // melt the core!
    return false;
  }
 
};

#endif //NanoViews_h
