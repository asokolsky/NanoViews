#ifndef Display_h
#define Display_h

/**
 * U8G2 derivative
 * Provides a display object to be used by all the viewes
 */
 
 class POINT
 {
public:  
  int16_t x = 0;
  int16_t y = 0;

  POINT() {}
  
#ifdef DEBUG
  void DUMP(const char *szText = 0) const;
#else
  void DUMP(const char *szText = 0) {}
#endif
 };
 
 class RECT 
 {
 public:
  RECT() {}
 
  int16_t left = 0;
  int16_t top = 0;
  int16_t right = 0;
  int16_t bottom = 0;

  int16_t width() const {
    return right - left;
  }
  int16_t height() const {
    return bottom - top;
  }
  void inflate(int16_t px = 1) {
    left -= px;
    top -= px;
    right += px;
    bottom += px;
  }
  void deflate(int16_t px = 1) {
    left += px;
    top += px;
    right -= px;
    bottom -= px;
  }
  bool doesIntersect(const RECT r) const;
  RECT intersect(const RECT r) const;

#ifdef DEBUG
  void DUMP(const char *szText = 0) const;
#else
  void DUMP(const char *szText = 0) {}
#endif
};

/**
 * Extension of U8G2
 */
class Display: public 
  U8G2_SH1106_128X64_NONAME_1_HW_I2C     //  has an internal RAM of 132x64
  //U8G2_SH1106_128X64_NONAME_F_HW_I2C     //  has an internal RAM of 132x64
  // U8G2_SSD1306_128X64_NONAME_F_HW_I2C // SSD1306 only has 128x64
{
  /* backlight intensity 0..10 */
  //byte m_bl = 9;
public: 
  const static uint8_t COLOR_BLACK    = 0;
  const static uint8_t COLOR_WHITE    = 1;
  const static uint8_t COLOR_DARKGREY = 2;
  const static uint8_t COLOR_RED      = 3;
  const static uint8_t COLOR_CYAN     = 4;
  
  Display(uint8_t adr) : U8G2_SH1106_128X64_NONAME_1_HW_I2C(U8G2_R0, /* reset=*/ U8X8_PIN_NONE)
    //U8G2_SH1106_128X64_NONAME_F_HW_I2C(U8G2_R0, /* reset=*/ U8X8_PIN_NONE)
    //U8G2_SSD1306_128X64_NONAME_F_HW_I2C(U8G2_R0, /* reset=*/ U8X8_PIN_NONE)
    {
      setI2CAddress(adr);
    }  

  /*void setClipRect(const RECT r) {
    _clipx1 = r.left; _clipy1 = r.top; _clipx2 = r.right; _clipy2 = r.bottom;
  }*/
  /*RECT getClipRect() const
  {
    RECT r;
    r.left = _clipx1; r.top = _clipy1; r.right = _clipx2; r.bottom = _clipy2;
    return r;
  }*/
  /*void resetClipRect() {
    //ILI9341_t3::setClipRect();
  }*/

  uint16_t measureTextWidth(const char *t)
  {
    return getStrWidth(t);
  }
  uint8_t getTextColor() const 
  {
    return getColorIndex();
  }
  int8_t getFontHeight() const
  {
    // ascent is positive and descent is negative
    return getFontAscent() - getFontDescent();
  }
  void drawRect(const RECT r, uint8_t color) // const
  {
    uint8_t oldColor = getDrawColor();
    if(color != oldColor)
      setDrawColor(color);
    drawFrame(r.left, r.top, r.width(), r.height());
    if(color != oldColor)
      setDrawColor(oldColor);
  }

  void fillRect(const RECT r, uint8_t color) // const
  {
    /*if(color != ILI9341_BLACK) {
      DEBUG_PRINT("fillRect color:"); DEBUG_PRINTHEX(color); r.DUMP(" r:");
    }
    ILI9341_t3::fillRect(r.left, r.top, r.width(), r.height(), color);*/
    uint8_t oldColor = getDrawColor();
    if(color != oldColor)
      setDrawColor(color);
    drawBox(r.left, r.top, r.width(), r.height());
    if(color != oldColor)
      setDrawColor(oldColor);
  }

  /*const ILI9341_t3_font_t *getFont() const {
    return font;
  }*/
  
  /**
  * Called once to set things up.
  */
  void setup();

  //const int16_t iButtonCornerRadius = 4;
  //const uint16_t uButtonBorderColor = ILI9341_DARKGREY;
  //const uint16_t uButtonLabelColor = ILI9341_YELLOW;
  //const uint16_t uButtonFaceColor = ILI9341_BLACK; // ILI9341_DARKGREEN;

  //void drawButton(const RECT rButton, const ILI9341_t3_font_t *pFont, const char *szLabel, bool bEraseBkgnd);

  enum HorizontalAlignment { haLeft, haCenter, haRight };
  enum VerticalAlignment { vaTop, vaCenter, vaBottom };
  
  /**
   * Print text with appropriate alignement in rLocation.
   * rLocation.bottom can be 0 if va == vaTop - in this case rLocation.bottom will be changed/set
   */
  void printText(const char *szText, uint8_t c,  uint8_t bg, RECT rLoc, 
    HorizontalAlignment ha, VerticalAlignment va = vaTop, const uint8_t *pFont = 0, bool bEraseBkgnd = false);
  
  
#ifdef DEBUG
  void DUMP(const char *szText = 0) const;
#else
  void DUMP(const char *szText = 0) {}
#endif
};

//extern Display g_display;

#endif // Display_h
