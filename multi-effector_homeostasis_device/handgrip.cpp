#include "superclasses.cpp"

#define HANDGRIPPIN A0

class _handgrip: public _affector {
  public:
    // _handgrip();
    float voltageValue(void) {
      return 5*analogRead(HANDGRIPPIN)/1023.; //simply returns the voltage read off the handgrip
    }
    //calculates the productionrate to feed into _indicatorstrip.setRate()
    //returns a value between 0 and HANDGRIPPRESCALER
    int calculateProductionRate(float pinADCval, _handgrip *Handgrip) {
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
};