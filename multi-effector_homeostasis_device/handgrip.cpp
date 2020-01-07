#include "headers.h"

float _handgrip::voltageValue(void){
  return 5*analogRead(HANDGRIPPIN)/1023.; //simply returns the voltage read off the handgrip
}

//calculates the productionrate to feed into _indicatorstrip.setRate()
//returns a value between 0 and HANDGRIPPRESCALER
int _handgrip::calculateProductionRate(float pinADCval, _handgrip *Handgrip) {
  float voltagedelta = voltageValue()-Handgrip->handgripMinVoltage;
  float delta = (voltagedelta /(Handgrip->handgripMaxVoltage-Handgrip->handgripMinVoltage)) * 
    Handgrip->handgripPrescaler;
  return delta;
}

_handgrip::_handgrip(){
}
