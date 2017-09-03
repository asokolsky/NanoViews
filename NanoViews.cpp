#include "NanoViews.h"

static Display *g_pDisplay = 0;

/**
 *  Widget Class Implementation
 */
void Widget::draw() 
{
  // just to make sure we overwrite this....
  if(g_pDisplay == 0)
    return;
  g_pDisplay->fillRect(m_position, Display::COLOR_RED);
  g_pDisplay->fillRect(m_rectClient, Display::COLOR_CYAN);
}

void Widget::setPosition(const RECT rLoc)
{
  DUMP("Widget::setPosition()"); rLoc.DUMP(" rLoc: "); 
  //if((left == m_position.left) && (m_position.top == top) && (m_position.right == right) && (m_position.bottom == bottom)) return;
  m_position = rLoc;
  onPosition();
}
 
#ifdef DEBUG
void Widget::DUMP(const char *szText /*= 0*/) const
{
  if(szText != 0) {
    DEBUG_PRNT(szText);
  }
  DEBUG_PRINT(" Widget@"); DEBUG_PRINTHEX((int)this); DEBUG_PRINT(" m_uStyle="); DEBUG_PRINTHEX(m_uStyle); 
  m_position.DUMP(" m_position: ");
  m_rectClient.DUMP(" m_rectClient: ");  
}
#endif
 
/** 
*  m_position was just changed.  
*  Default implementation updates m_rectClient leaving no space for decorations.
*/
void Widget::onPosition()
{
  m_rectClient = m_position;
}

static const char szSeparator[] = ": ";

/**
* Print Key1: Val1   Key2: Val2
* with Key in ILI9341_DARKGREY
* and Val in ILI9341_WHITE
* x is the position of the first ':'
*/
void Widget::printKeyVal(
  uint16_t x, uint16_t y, 
  const char *szKey1, long lVal1, 
  bool bSelected, 
  const char *szKey2, long lVal2)
{
  if(g_pDisplay == 0)
    return;

  char szText[80];

  RECT rLocation;
  rLocation.left = m_rectClient.left;
  rLocation.right = x;
  rLocation.top = y;
  rLocation.bottom = 0;
  //rLocation.DUMP("Widget::printKeyVal rLocation");

  sprintf(szText, "%s%s", szKey1, szSeparator); 
  g_pDisplay->printText(szText, Display::COLOR_DARKGREY, Display::COLOR_BLACK, rLocation, Display::haRight, Display::vaTop);

  rLocation.left = x;
  rLocation.right = x + g_pDisplay->measureTextWidth("-0000");
  sprintf(szText, "%ld", lVal1);                     // print val1
  g_pDisplay->printText(szText, Display::COLOR_WHITE, Display::COLOR_BLACK, rLocation, Display::haLeft, Display::vaTop);

  if(bSelected) 
  {
    RECT r;
    r.left = x-1;
    r.right = r.left + g_pDisplay->measureTextWidth(szText) + 3;
    r.top = y-1;
    r.bottom = r.top + g_pDisplay->getFontHeight() + 1;
    g_pDisplay->drawRect(r, Display::COLOR_DARKGREY);
    x += 3;
  }  
  x += g_pDisplay->measureTextWidth(szText);
  if(szKey2 != 0)
  {
    rLocation.left = x;
    x = rLocation.right = (m_rectClient.width() / 4) * 3;         // position of second ':'
    //rLocation.top = y;
    //rLocation.bottom = 0;
    
    sprintf(szText, "%s%s", szKey2, szSeparator);
    g_pDisplay->printText(szText, Display::COLOR_DARKGREY, Display::COLOR_BLACK, rLocation, Display::haRight, Display::vaTop);   
    rLocation.left = x;
    rLocation.right = x + g_pDisplay->measureTextWidth("-0000");
    sprintf(szText, "%ld", lVal2);
    g_pDisplay->printText(szText, Display::COLOR_WHITE, Display::COLOR_BLACK, rLocation, Display::haLeft, Display::vaTop);
  }
}
 
  
/**
*  TextWidget Class Implementation
*/
void TextWidget::draw()
{
  DUMP("TextWidget::draw()");
  if(g_pDisplay == 0)
    return;
  g_pDisplay->printText(
    m_szText, Display::COLOR_WHITE, Display::COLOR_BLACK, 
    m_rectClient, Display::haCenter, Display::vaCenter);
}

#ifdef DEBUG
void TextWidget::DUMP(const char *szText /*= 0*/) const
{
  Widget::DUMP(szText);
  DEBUG_PRINT(" TextWidget@"); DEBUG_PRINTHEX((int)this); DEBUG_PRINT(" m_szText='"); DEBUG_PRNT(m_szText); DEBUG_PRINTLN("'");
}
#endif
 
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
  TextWidget(0)
{
  setText(szTitle);
  m_uStyle |= WS_TITLEBAR;
  
  for(uint8_t i = 0; i < (sizeof(m_zChildren)/sizeof(m_zChildren[0])); i++)
    m_zChildren[i] = 0;

  m_position.top = 0;
  m_position.left = 0;
  m_position.bottom = (g_pDisplay == 0) ? 0 : g_pDisplay->getDisplayWidth(); // the order is important! 
  m_position.right = (g_pDisplay == 0) ? 0 : g_pDisplay->getDisplayHeight();
  setPosition(m_position);  
}

/**
 * Called once to set things up.
 */
void View::setup(Display *p)
{
  DEBUG_PRINTLN("View::setup()");     
  g_pDisplay = p;
  g_pDisplay->setup();
}

/** 
 * to be called from the main loop on the active view.  Do nothing by default. Return TRUE to update display
 */
boolean View::loop(unsigned long now)
{
  return false;
}

/** 
 * Static member to activate view p calling the notifiers 
 */
void View::activate(View *p) 
{
  DEBUG_PRINTLN("View::activate()");
  //delay(1000); 
  p->DUMP("About to activate: ");  
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
  DUMP("View::onDeActivate");
}

void View::onActivate(View *pPrevActive)
{
  DUMP("View::onActivate");
  assert(g_pDisplay != 0);
  //g_pDisplay->resetClipRect();
  RECT rPos;
  rPos.top = 0;
  rPos.left = 0;
  rPos.right = g_pDisplay->getDisplayWidth();
  rPos.bottom = g_pDisplay->getDisplayHeight();
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
    m_zChildren[0]->setPosition(m_rectClient);
}

/** 
 * Entire screen redraw
 * Update non-clietn area and then call a virtual update of the client in a safe sandbox
 */
void View::update(unsigned long now)
{
  //DUMP("View::update()");

  g_pDisplay->firstPage();
  do 
  {  
    if(m_bEraseBkgnd)
    {
      RECT rFill = m_position;
      //rFill.bottom -= iBottomBarHeight;
      g_pDisplay->fillRect(rFill, 0); //, ILI9341_OLIVE);
    }
    drawTitleBar();
    m_bEraseBkgnd = false;
    //
    // try to protect non-client area  
    // 
    //g_pDisplay->setClipRect(m_rectClient);
    
    //
    // set defaults for use in the client area
    //
    //g_pDisplay->setFont(LiberationSans_16);  
    //g_pDisplay->setTextSize(1);
    //g_pDisplay->setTextColor(ILI9341_WHITE, ILI9341_BLACK);
    //g_pDisplay->setCursor(m_rectClient.left, m_rectClient.top);
    
    updateClient(now);

  } while (g_pDisplay->nextPage());
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
  for(uint8_t i = 0; i < (sizeof(m_zChildren)/sizeof(m_zChildren[0])); i++)
    if(m_zChildren[i] != 0)
      m_zChildren[i]->draw();
}

/**
 *  draws the title bar
 */
void View::drawTitleBar()
{
  // DEBUG_PRINT("View::drawTitleBar("); DEBUG_PRINT(m_szText);  DEBUG_PRINTLN(")");
  RECT r;
  r.top = m_position.top + 2;
  r.bottom = 0;
  r.left = m_position.left + 2;
  r.right = m_position.right;
  g_pDisplay->printText(m_szText, 1/*ILI9341_YELLOW*/, 0/*ILI9341_BLACK*/, r, Display::haLeft, Display::vaTop); // , &LiberationSans_18);
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
  TextWidget::DUMP(szText);
  DEBUG_PRINT("View@"); DEBUG_PRINTHEX((int)this); DEBUG_PRINTLN(""); 
}
#endif


