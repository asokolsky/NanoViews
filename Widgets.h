
#ifndef Widgets_h
#define Widgets_h

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
  uint16_t m_uStyle;

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


#endif // Widgets_h

