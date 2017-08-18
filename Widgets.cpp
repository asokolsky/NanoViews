#include "NanoViews.h"

/**
 *  Widget Class Implementation
 */
void Widget::draw() 
{
  // just to make sure we overwrite this....
  g_display.fillRect(m_position, Display::COLOR_RED);
  g_display.fillRect(m_rectClient, Display::COLOR_CYAN);
}

void Widget::setPosition(const RECT rLoc)
{
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
  DEBUG_PRINT(" Widget@"); DEBUG_PRINTDEC((int)this); 
  m_position.DUMP(" m_position: ");
  m_rectClient.DUMP("m_rectClient: ");  
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
  char szText[80];

  RECT rLocation;
  rLocation.left = m_rectClient.left;
  rLocation.right = x;
  rLocation.top = y;
  rLocation.bottom = 0;
  //rLocation.DUMP("Widget::printKeyVal rLocation");

  sprintf(szText, "%s%s", szKey1, szSeparator); 
  g_display.printText(szText, Display::COLOR_DARKGREY, Display::COLOR_BLACK, rLocation, Display::haRight, Display::vaTop);
 
  rLocation.left = x;
  rLocation.right = x + g_display.measureTextWidth("-0000");
  sprintf(szText, "%ld", lVal1);                     // print val1
  g_display.printText(szText, Display::COLOR_WHITE, Display::COLOR_BLACK, rLocation, Display::haLeft, Display::vaTop);

  if(bSelected) 
  {
    RECT r;
    r.left = x-1;
    r.right = r.left + g_display.measureTextWidth(szText) + 3;
    r.top = y-1;
    r.bottom = r.top + g_display.getFontHeight() + 1;
    g_display.drawRect(r, Display::COLOR_DARKGREY);
    x += 3;
  }  
  x += g_display.measureTextWidth(szText);
  if(szKey2 != 0)
  {
    rLocation.left = x;
    x = rLocation.right = (m_rectClient.width() / 4) * 3;         // position of second ':'
    //rLocation.top = y;
    //rLocation.bottom = 0;
    
    sprintf(szText, "%s%s", szKey2, szSeparator);
    g_display.printText(szText, Display::COLOR_DARKGREY, Display::COLOR_BLACK, rLocation, Display::haRight, Display::vaTop);   
    rLocation.left = x;
    rLocation.right = x + g_display.measureTextWidth("-0000");
    sprintf(szText, "%ld", lVal2);
    g_display.printText(szText, Display::COLOR_WHITE, Display::COLOR_BLACK, rLocation, Display::haLeft, Display::vaTop);
  }
}

 
/**
 *  TextWidget Class Implementation
 */
/*void TextWidget::draw()
{
  DEBUG_PRINT("TextWidget::draw: "); DEBUG_PRINTLN(m_strText.c_str());
  m_position.DUMP("TextWidget m_position: ");
  m_rectClient.DUMP("TextWidget m_rectClient: ");

  printTextCenter(m_strText.c_str(), 0, m_pFont, 0);
}*/

