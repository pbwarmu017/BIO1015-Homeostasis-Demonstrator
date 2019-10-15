#include <Adafruit_NeoPixel.h>
#include "handgrip.h"
#include "multi-effector_homeostasis_device.h"
// #include <math.h> //used for abs()
float _handgrip::voltageValue(void){
  return 5*analogRead(A0)/1023.; //simply returns the voltage read off the handgrip
}

//calculates the productionrate to feed into _indicatorstrip.setRate()
//returns a value between 0 and HANDGRIPPRESCALER
int _handgrip::calculateProductionRate(float pinADCval) {
  float voltagedelta = voltageValue()-HANDGRIPMINVOLTAGE;
  float delta = (voltagedelta /(HANDGRIPMAXVOLTAGE-HANDGRIPMINVOLTAGE)) * 
      HANDGRIPRATESCALER;
  return delta;
}