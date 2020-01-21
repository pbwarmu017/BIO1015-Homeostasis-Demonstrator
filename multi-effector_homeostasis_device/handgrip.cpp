#ifndef HANDGRIP_CPP
#define HANDGRIP_CPP

#include "superclasses.cpp"
class _handgrip: public _affector 
{
  public:
    int portnum;
    _handgrip(const int port, _device* ptr)
    { //constructor
      main_ptr = ptr;
      switch(port)
      {
        case ACON1_PORTNUM:
          handGripPin = ACON1_PINNUM;
          main_ptr->ACON1_mode = 1;
          break;
        case ACON2_PORTNUM:
          handGripPin = ACON2_PINNUM;
          main_ptr->ACON2_mode = 1;
          break;
        case DACON1_PORTNUM:
          handGripPin = DACON1_PINNUM;
          main_ptr->ACON1_mode = 1;
          break;
        case DACON2_PORTNUM:
          handGripPin = DACON2_PINNUM;
          main_ptr->ACON2_mode = 1;
          break;
      }
      color = portnum = port;
    }

    ~_handgrip()
    { //destructor (virtualized in _device)
      switch(color) 
      {//color was used to store portnum earlier. Destructors cannot take parameters. 
        case ACON1_PORTNUM:
          main_ptr->ACON1_mode = 0;
          break;
        case ACON2_PORTNUM:
          main_ptr->ACON2_mode = 0;
          break;
        case DACON1_PORTNUM:
          main_ptr->ACON1_mode = 0;
          break;
        case DACON2_PORTNUM:
          main_ptr->ACON2_mode = 0;
          break;
      }
      handGripPin = -1;
      color = 0;
    }
    float voltageValue(void) 
    {
      return 5*analogRead(handGripPin)/1023.; //simply returns the voltage read off the handgrip
    }
    //calculates the productionrate to feed into _indicatorstrip.setRate()
    //returns a value between 0 and HANDGRIPPRESCALER
    int calculateProductionRate(float pinADCval, _handgrip *Handgrip) 
    {
      float voltagedelta = voltageValue()-Handgrip->handgripMinVoltage;
      float delta = (voltagedelta /(Handgrip->handgripMaxVoltage-Handgrip->handgripMinVoltage)) * 
        Handgrip->handgripPrescaler;
      return delta;
    }

      int calibrationState = 0; //standby state
      float handgripMinVoltage;
      float handgripMaxVoltage;
      float handgripPrescaler = 75;
      float productionRate = 0;
  private:
    int handGripPin = -1;
    int color = 0;
    _device *main_ptr;
};

#endif