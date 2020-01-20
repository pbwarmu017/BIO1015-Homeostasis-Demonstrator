#ifndef HANDGRIP_CPP
#define HANDGRIP_CPP

#include "superclasses.cpp"
class _handgrip: public _affector 
{
  public:

    _handgrip(const int portnum)
    { //constructor
      switch(portnum)
      {
        case ACON1_PORTNUM:
          handGripPin = ACON1_PINNUM;
          ACON1_status = 1;
          break;
        case ACON2_PORTNUM:
          handGripPin = ACON2_PINNUM;
          ACON2_status = 1;
          break;
        case DACON1_PORTNUM:
          handGripPin = DACON1_PINNUM;
          DACON1_status = 1;
          break;
        case DACON2_PORTNUM:
          handGripPin = DACON2_PINNUM;
          DACON2_status = 1;
          break;
      }
      color = portnum;
    }

    ~_handgrip(const int portnum)
    { //destructor (virtualized in _device)
      switch(portnum)
      {
        case ACON1_PORTNUM:
          ACON1_status = 0;
          break;
        case ACON2_PORTNUM:
          ACON2_status = 0;
          break;
        case DACON1_PORTNUM:
          DACON1_status = 0;
          break;
        case DACON2_PORTNUM:
          DACON2_status = 0;
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
};

#endif