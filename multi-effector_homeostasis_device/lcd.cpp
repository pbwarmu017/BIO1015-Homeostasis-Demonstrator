#include "headers.h"

//constructor
_lcd::_lcd() {
  lcd_ptr = new Adafruit_RGBLCDShield();
}