#ifndef LCD_CPP
#define LCD_CPP

#include "superclasses.cpp"
 //LCD background color defines
//the LCD library doesnt do any PWM on the three different backlight channels
//this is a hardware limitation, so only these 8 different colors are available. 
#define OFF 0x0
#define RED 0x1
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7

class _lcd: public _device 
{
  public: 
    Adafruit_RGBLCDShield *lcd_obj;
    _lcd()
    {
      lcd_obj = new Adafruit_RGBLCDShield();
      lcd_obj->begin(16,2);
      lcd_obj->setBacklight(WHITE);
    }
    ~_lcd()
    { //destructor (virtualized in _device)

    }
};
#endif