/**
 *  Simple example of using NanoViews
 */
#include "NanoViews.h"
#include "Led.h"
#include "AnalogNavigationKeypad.h"
#include "MyViews.h"

/**
 * These are connections between the hardware components
 */

/** built-in LED is on this pin */
const uint8_t pinLed = 13;

/** Keypad connection */
const uint8_t pinKeyPad1 = A0;
const uint8_t pinKeyPad2 = A1;

/** display is on I2C which are these pins */
const uint8_t pinI2C_SDA = A4;
const uint8_t pinI2C_SCL = A5;
const uint8_t addressI2Cdisplay = 0x78;

/**
 * Global objects representing hardware components
 */
Led g_led(pinLed);
Display g_display(addressI2Cdisplay);

/**
 * Need to inherit from AnalogNavigationKeypad in order to get callbacks
 */
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

/**
 * Global object representing hardware components
 */
MyNavKeyPad g_kp;

/**
 * Entry points
 */
void setup() 
{
  Serial.begin(115200);  
  delay(1000);   
  //while(!Serial)  ; // wait for serial port to connect. Needed for Leonardo only
  DEBUG_PRINTLN("Simple NanoViews Test!");
  View::setup(&g_display);
  //delay(1000); 
  View::activate(&g_viewLed);
}

void loop() 
{
  delay(1000);   
  DEBUG_PRINTLN("loop!");
  
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

