#include "includes.h"
#include "parents.cpp"

class _lcd: public _device {
  public: 
    Adafruit_RGBLCDShield *lcd_ptr;
    _lcd(){
      lcd_ptr = new Adafruit_RGBLCDShield();
    }
};