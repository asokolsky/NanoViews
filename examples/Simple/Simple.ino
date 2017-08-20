/**
 *  Simple example of using NanoViews
 */
#include "NanoViews.h"
#include "Led.h"
#include "PCB.h"
#include "AnalogNavigationKeypad.h"
#include "MyViews.h"
 
//
// nothing to customize below
//

Led g_led(pinLed);


class MyNavKeyPad: public AnalogNavigationKeypad
{
public:  
  /** this test if for a keyboard connected to A0 and A1 */
  MyNavKeyPad() : AnalogNavigationKeypad(pinKeyPad1, pinKeyPad2)
  {
    
  }
  bool onUserInActivity(unsigned long ulNow);
  bool onKeyAutoRepeat(uint8_t vks);
  bool onKeyDown(uint8_t vks);
  bool onLongKeyDown(uint8_t vks);
  bool onKeyUp(uint8_t vks);
};

bool MyNavKeyPad::onUserInActivity(unsigned long ulNow)
{
  DEBUG_PRINT("MyNavKeyPad::onUserInActivity ulNow="); DEBUG_PRINTDEC(ulNow); DEBUG_PRINTLN("");
  return false; 
}

bool MyNavKeyPad::onKeyAutoRepeat(uint8_t vks)
{
  DEBUG_PRINT("MyNavKeyPad::onKeyAutoRepeat vks="); DEBUG_PRNTLN(getKeyNames(vks));
  return false; 
}

bool MyNavKeyPad::onKeyDown(uint8_t vks)
{
  DEBUG_PRINT("MyNavKeyPad::onKeyDown vks="); DEBUG_PRNTLN(getKeyNames(vks));
  return false; 
}

bool MyNavKeyPad::onLongKeyDown(uint8_t vks)
{
  DEBUG_PRINT("MyNavKeyPad::onLongKeyDown vks="); DEBUG_PRNTLN(getKeyNames(vks));
  return false; 
}

bool MyNavKeyPad::onKeyUp(uint8_t vks)
{
  DEBUG_PRINT("MyNavKeyPad::onKeyUp vks="); DEBUG_PRNTLN(getKeyNames(vks));
  return false; 
}

MyNavKeyPad g_kp;


void setup() 
{
  Serial.begin(115200);  
  delay(1000);   
  //while(!Serial)  ; // wait for serial port to connect. Needed for Leonardo only
  DEBUG_PRINTLN("Simple NanoViews Test!");
    
  View::setup();
  View::activate(&g_viewLed);
}

void loop() 
{
  unsigned long ulNow = millis();

  bool bUpdateDisplay = false;
  if(View::g_pActiveView != 0)
    bUpdateDisplay = View::g_pActiveView->loop(ulNow);

  if(g_kp.getAndDispatchKey(ulNow)) 
  {
    bUpdateDisplay = true;
  } 
  /*else if(g_serialCommandInterpreter.available()) 
  {
    DEBUG_PRINTLN("Read a command from serial line!");   
    g_serialCommandInterpreter.readAndDispatch();
    bUpdateDisplay = true;
  } */
  //if(bUpdateDisplay) g_ci.updateDisplay(ulNow);   
  if(bUpdateDisplay && View::g_pActiveView != 0)
    View::g_pActiveView->update(ulNow);
  
}

