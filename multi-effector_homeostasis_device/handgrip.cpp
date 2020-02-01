#ifndef HANDGRIP_CPP
#define HANDGRIP_CPP

#include "superclasses.cpp"
#include "indicatorstrip.cpp"
#include "menu.cpp"

// #define GRIPRATECALCDELAY 100 //value is in milliseconds
#define GRIPRATESCALER 25.0

class _handgrip: public _affector 
{
  public:
    _handgrip(const int port, _device* ptr, _indicatorstrip *indptr, _lcd *lcdptr, _menu * menuptr)
    { //constructor
      main_ptr = ptr;
      indicatorstrip_ptr = indptr;
      lcd_ptr = lcdptr;
      menu_ptr = menuptr;
      portNum = port;
      switch(portNum)
      {
        case ACON1_PORTNUM:
          handGripPin = ACON1_PINNUM;
          main_ptr->ACON1_mode = HANDGRIP_TYPE;
          break;
        case ACON2_PORTNUM:
          handGripPin = ACON2_PINNUM;
          main_ptr->ACON2_mode = HANDGRIP_TYPE;
          break;
        case DACON1_PORTNUM:
          handGripPin = DACON1_PINNUM;
          main_ptr->ACON1_mode = HANDGRIP_TYPE;
          break;
        case DACON2_PORTNUM:
          handGripPin = DACON2_PINNUM;
          main_ptr->ACON2_mode = HANDGRIP_TYPE;
          break;
      }
      //calibrate handgrip
      (lcd_ptr->lcd_obj)->clear();
      (lcd_ptr->lcd_obj)->setCursor(0,0);
      (lcd_ptr->lcd_obj)->print("Release Grip And");
      (lcd_ptr->lcd_obj)->setCursor(0,1);
      (lcd_ptr->lcd_obj)->print("Press Select");

      while( !((lcd_ptr->lcd_obj)->readButtons() & BUTTON_SELECT) ); //wait for select

      handgripMinVoltage = voltageValue();


      (lcd_ptr->lcd_obj)->clear();

      delay(500);

      (lcd_ptr->lcd_obj)->setCursor(0,0);
      (lcd_ptr->lcd_obj)->print("Compress Grip");
      (lcd_ptr->lcd_obj)->setCursor(0,1);
      (lcd_ptr->lcd_obj)->print("And Press Select");

      while( !((lcd_ptr->lcd_obj)->readButtons() & BUTTON_SELECT) ); //wait for select

      handgripMaxVoltage = voltageValue();


      voltageRange = handgripMaxVoltage - handgripMinVoltage;


      (lcd_ptr->lcd_obj)->clear();

      delay(500);

      (lcd_ptr->lcd_obj)->clear();
      menu_ptr->printMenu(lcd_ptr);

    }

    ~_handgrip()
    { //destructor (virtualized in _device)
      switch(portNum) 
      {//color was used to store portnum earlier. Destructors cannot take parameters. 
        case ACON1_PORTNUM:
          main_ptr->ACON1_mode = 0;
          indicatorstrip_ptr->deviceIndicatorPosition[ACON1_PORTNUM] = -2;
          break;
        case ACON2_PORTNUM:
          main_ptr->ACON2_mode = 0;
          indicatorstrip_ptr->deviceIndicatorPosition[ACON2_PORTNUM] = -2;
          break;
        case DACON1_PORTNUM:
          main_ptr->DACON1_mode = 0;
          indicatorstrip_ptr->deviceIndicatorPosition[DACON1_PORTNUM] = -2;
          break;
        case DACON2_PORTNUM:
          main_ptr->DACON2_mode = 0;
          indicatorstrip_ptr->deviceIndicatorPosition[DACON2_PORTNUM] = -2;
          break;
      }
      // handGripPin = -1;
      menu_ptr->printMenu(lcd_ptr);
    }

    float voltageValue(void) 
    {
      return 5*analogRead(handGripPin)/1023.; //simply returns the voltage read off the handgrip
    }

    void calculateRate(int modifier)
    {
      if(modifier == GENERAL_RATETYPE)
      {
        float delta = (voltageValue() - handgripMinVoltage );
        if(delta < 0) delta = 0; //make sure the value can't go negative
        overallRate = ((delta/voltageRange*maxProductionRate)*2 - consumptionRate) * (0.0002 * GRIPRATESCALER);
        indicatorstrip_ptr->updatePosition(overallRate, portNum);
      } 
    }

  private:
    _device *main_ptr;
    _indicatorstrip *indicatorstrip_ptr;
    _lcd *lcd_ptr;
    _menu *menu_ptr;
    float overallRate = 0;
    float voltageRange;
    float handgripMinVoltage;
    float handgripMaxVoltage;
    // float handgripPrescaler = 75;

    float maxProductionRate = 70;
    float consumptionRate = 60;


    int portNum = -1; //used to save the port number that this object is instantiated on.
    int handGripPin = -1;
};

#endif