#include "NanoViews.h"

/**
 *  RECT Class Implementation
 */

bool RECT::doesIntersect(const RECT r) const
{
 // If one rectangle is on left side of other
 if(left > r.right || r.left > right)
   return false; 
  // If one rectangle is above other
  if(top < r.bottom || r.top < bottom)
    return false;  
  return true;
}

RECT RECT::intersect(const RECT r) const
{
  RECT res;
  res.top = max(top, r.top);
  res.left = max(left, r.left);
  res.bottom = min(bottom, r.bottom);
  res.right = min(right, r.right);
  return res;
}

 
#ifdef DEBUG
void RECT::DUMP(const char *szText /* = 0*/) const
{
  if(szText != 0) {
    DEBUG_PRNT(szText);
  }
  DEBUG_PRINT(" RECT@"); DEBUG_PRINTDEC((int)this); 
  DEBUG_PRINT(" l="); DEBUG_PRINTDEC((int)left);
  DEBUG_PRINT(" r="); DEBUG_PRINTDEC((int)right);
  DEBUG_PRINT(" t="); DEBUG_PRINTDEC((int)top);
  DEBUG_PRINT(" b="); DEBUG_PRINTDEC((int)bottom);
  DEBUG_PRINTLN("");  
}
#endif

/**
 * Class Implementation
 */
/**
 *  Display initialization
 */
 void Display::setup()
 {
   begin();
   //fillScreen(ILI9341_BLACK);
   //setRotation(uDisplayOrientation); // for PCB v0.9 and later
   //setTextWrap(false);

   DUMP("Display::setup()");
 }
 
/*void Display::drawButton(const RECT rButton, const ILI9341_t3_font_t *pFont, const char *szLabel, bool bEraseBkgnd)
{ 
  RECT rLoc = rButton;
  setClipRect(rLoc);
  drawRoundRect(rLoc.left, rLoc.top, rLoc.width(), rLoc.height(), iButtonCornerRadius, uButtonBorderColor);
  rLoc.deflate(iButtonCornerRadius);
  setClipRect(rLoc);
  printText(szLabel, uButtonLabelColor, uButtonFaceColor, rLoc, haCenter, vaCenter, pFont, bEraseBkgnd);
}*/

/**
 * Print text with appropriate alignement in rLocation.
 * rLocation.bottom can be 0 if va == vaTop
 */
void Display::printText(
  const char *szText, 
  uint8_t c, 
  uint8_t bg, 
  RECT rLocation, 
  HorizontalAlignment ha, 
  VerticalAlignment va /*= vaTop*/, 
  const uint8_t *pFont /*= 0*/, 
  bool bEraseBkgnd /*= false*/)
{
  //DEBUG_PRINT("Display::printText('"); DEBUG_PRINT(szText); DEBUG_PRINT("' ha="); DEBUG_PRINTDEC((int)ha); DEBUG_PRINT(" va="); DEBUG_PRINTDEC((int)va); rLocation.DUMP(" rLocation");
  setDrawColor(c);
  if(pFont != 0) {
    /*g_display.*/setFont(pFont);
  }
  if(rLocation.bottom == 0)
    rLocation.bottom = rLocation.top + /*g_display.*/getFontHeight();
  
  //RECT rOldClip = getClipRect();  
  //RECT rClip = rOldClip.intersect(rLocation); 
  //setClipRect(rClip);

  if(bEraseBkgnd || (szText == 0) || (szText[0] == '\0'))
    fillRect(rLocation, bg); // clear the entire button face
    
  if((szText != 0) && (szText[0] != '\0'))
  {
    int16_t dx = rLocation.width() - /*g_display.*/getStrWidth(szText);
    int16_t dy = rLocation.height() - /*g_display.*/getFontHeight();
    
    RECT rFill = rLocation;
    switch(ha) {
      case haLeft:
        rFill.right = rFill.left;
        break;
      case haCenter:
        rFill.right = rFill.left + (dx/2);
        break;
      case haRight:
        rFill.right = rFill.left + dx;
        break;
    }
    if((!bEraseBkgnd) && (ha != haLeft))
      fillRect(rFill, bg);
    
    rFill.left = rFill.right;
    rFill.right += /*g_display.*/getStrWidth(szText);
    switch(va) {
      case vaTop:
        setCursor(rFill.left, rFill.top);
        print(szText);
        rFill.top = rFill.bottom - dy;
        //fillRect(rFill, bg);
        break;
      case vaCenter:
        rFill.bottom = rFill.top + (dy/2);
        fillRect(rFill, bg);              
        setCursor(rFill.left, rFill.bottom);
        print(szText);
        rFill.bottom = rLocation.bottom;
        rFill.top = rFill.bottom - (dy/2);
        //fillRect(rFill, bg);        
        break;
      case vaBottom:
        rFill.bottom = rFill.top + dy;
        //fillRect(rFill, bg);
        setCursor(rFill.left, rFill.bottom);
        print(szText);
        break;
    }    
    
    rFill = rLocation;
    switch(ha) {
      case haLeft:
        rFill.left = rFill.right - dx;
        break;
      case haCenter:
        rFill.left = rFill.right - (dx/2);
        break;
      case haRight:
        rFill.left = rFill.right;
        break;
    }
    if((!bEraseBkgnd) && (ha != haRight))
      fillRect(rFill, bg); // clear the entire button face
  }
  //setClipRect(rOldClip);
  //if(pOldFont != 0) setFont(*pOldFont);
}



#ifdef DEBUG
void Display::DUMP(const char *szText /* = 0*/) const
{
  if(szText != 0) {
    DEBUG_PRNT(szText);
  }
  DEBUG_PRINT(" Display@"); DEBUG_PRINTDEC((int)this); DEBUG_PRINT(" _width="); DEBUG_PRINTDEC(getDisplayWidth()); DEBUG_PRINT(" _height="); DEBUG_PRINTDEC(getDisplayHeight()); 
  
  //int16_t  cursor_x, cursor_y;
  /*RECT r;
  r.left = _clipx1;
  r.right = _clipx2;
  r.top = _clipy1;
  r.top = _clipy2;
  r.DUMP(" clipRect: ");*/

  //int16_t  origin_x, origin_y;
  
  /*r.left = rel_clipx1;
  r.right = rel_clipx2;
  r.top = rel_clipy1;
  r.top = rel_clipy2;
  r.DUMP(" rel_clipRect: ");*/
  
  //uint16_t textcolor, textbgcolor;
  //uint8_t textsize, rotation;  
  DEBUG_PRINTLN("");
}
#endif



