#ifndef LCD_CPP
#define LCD_CPP
#include "superclasses.cpp"
class _lcd: public _device {
  public: 
    Adafruit_RGBLCDShield *lcd_obj;
    _lcd(){
      // Serial.print("TEST");
      lcd_obj = new Adafruit_RGBLCDShield();
    }
};
#endif