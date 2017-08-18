#include "NanoViews.h"

/**
 * Generic Views (Top Level Windows) Implementation
 */


/**
 * Globals: views
 */
View *View::g_pActiveView = 0;
View *View::g_pPreviousView = 0;
unsigned long View::m_ulToUpdate = 0;



const int16_t iTitleBarHeight = 30; // 27;

/**
 * Class View
 */
View::View(const char *szTitle) :
  m_szTitle(szTitle)
{
  for(uint8_t i = 0; i < MAXKIDS; i++)
    m_zChildren[i] = 0;

  m_position.top = 0;
  m_position.left = 0;
  m_position.bottom = g_display.getDisplayWidth(); // the order is important! 
  m_position.right = g_display.getDisplayHeight();
  setPosition(m_position);  
}

/**
 * Called once to set things up.
 */
void View::setup()
{
  g_display.setup();
}

/** 
 * to be called from the main loop on the active view.  Do nothing by default. Return TRUE to update display
 */
boolean View::loop(unsigned long now)
{
  return false;
}

void View::activate(View *p) 
{
  if(g_pActiveView != 0)
  {
    g_pActiveView->onDeActivate(p);
  }
  if(p != 0)
  {
    p->onActivate(g_pPreviousView = g_pActiveView);
    g_pActiveView = p;
    p->update(millis());
  }
  else
  {
    DEBUG_PRINTLN("BUMMER! View::activate(0!!!)");   
  }
}

void View::onDeActivate(View *pNewActive)
{
  DEBUG_PRINTLN("View::onDeActivate");
}

void View::onActivate(View *pPrevActive)
{
  DEBUG_PRINTLN("View::onActivate");
  //g_display.resetClipRect();
  RECT rPos;
  rPos.top = 0;
  rPos.left = 0;
  rPos.right = g_display.getDisplayWidth();
  rPos.bottom = g_display.getDisplayHeight();
  setPosition(rPos); // done in the constructor but let's reiterate
  // erase the entire background
  m_bEraseBkgnd = true;
}


void View::updateMaybe(unsigned long now)
{
  if(now < m_ulToUpdate)
    return;
  m_ulToUpdate = now + ulUpdatePeriod;
  update(now);
}

/** 
 *  m_position was just changed.  
 *  Default implementation updates m_rectClient.
 *  If there is exactly one child - stretch it to cover the client.
 *  Derived class can overwrite this behaviour in oPosition or onActivate
 */
void View::onPosition()
{
  m_rectClient = m_position;
  m_rectClient.top += iTitleBarHeight;
  //m_rectClient.bottom -= iBottomBarHeight;
  if((m_zChildren[0] != 0) && (m_zChildren[1] == 0))
    m_zChildren[1]->setPosition(m_rectClient);
}

/** 
 * Entire screen redraw
 * Update non-clietn area and then call a virtual update of the client in a safe sandbox
 */
void View::update(unsigned long now)
{
  //DUMP("View::update()");
  if(m_bEraseBkgnd)
  {
    RECT rFill = m_position;
    //rFill.bottom -= iBottomBarHeight;
    g_display.fillRect(rFill, 0); //, ILI9341_OLIVE);
  }
  drawTitleBar();
  m_bEraseBkgnd = false;
  //
  // try to protect non-client area  
  // 
  //g_display.setClipRect(m_rectClient);
  
  //
  // set defaults for use in the client area
  //
  //g_display.setFont(LiberationSans_16);  
  //g_display.setTextSize(1);
  //g_display.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  //g_display.setCursor(m_rectClient.left, m_rectClient.top);
  
  updateClient(now);
  //
  m_ulToUpdate = now + ulUpdatePeriod;
}

/** 
 *  redraw client area only, not including title and bottom bar 
 */
void View::updateClient(unsigned long now)
{
  DEBUG_PRINTLN("View::updateClient()");
  // entire background erase - does the job but blinks!
  // m_lcd.fillRect(m_rectClient, ILI9341_BLUE);
  // redraw children!
  for(uint8_t i = 0; i < MAXKIDS; i++)
    if(m_zChildren[i] != 0)
      m_zChildren[i]->draw();
}

/**
 *  draws the title bar
 */
void View::drawTitleBar()
{
  // DEBUG_PRINT("View::drawTitleBar("); DEBUG_PRINT(m_szTitle);  DEBUG_PRINTLN(")");
  RECT r;
  r.top = m_position.top + 2;
  r.bottom = 0;
  r.left = m_position.left + 2;
  r.right = m_position.right;
  g_display.printText(m_szTitle, 1/*ILI9341_YELLOW*/, 0/*ILI9341_BLACK*/, r, Display::haLeft, Display::vaTop); // , &LiberationSans_18);
}

/** dummy defaults, children to overwrite */
bool View::onKeyDown(uint8_t vk) 
{
  return false;
}
bool View::onKeyAutoRepeat(uint8_t vk) 
{
  return false;
}
bool View::onLongKeyDown(uint8_t vk) 
{
  return false;
}
bool View::onKeyUp(uint8_t vk) 
{
  return false;
}
bool View::onKeysInactive()
{
  DEBUG_PRINTLN("View::onKeysInactive() => true");
  return true;
}


#ifdef DEBUG
void View::DUMP(const char *szText /*= 0*/) const
{
  Widget::DUMP(szText);
  DEBUG_PRINT("View@"); DEBUG_PRINTDEC((int)this); DEBUG_PRINT(" m_szTitle="); DEBUG_PRNTLN(m_szTitle); 
}
#endif


