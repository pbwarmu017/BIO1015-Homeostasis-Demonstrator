#include <Adafruit_NeoPixel.h>
#include "handgrip.h"
#include "multi-effector_homeostasis_device.h"
#include <math.h> //used for abs()
float _handgrip::voltageValue(void){
  return 1023./analogRead(A0); //simply returns the voltage read off the handgrip
}

//calculates the productionrate to feed into _indicatorstrip.setRate()
int _handgrip::calculateProductionRate(float pinADCval, int devnum) {
  
  // Serial.println(5*pinADCval/1023.);
  if(devnum == HANDGRIPDEVNUM){
    float voltagedelta = abs(HANDGRIPMINVOLTAGE-(5*pinADCval)/1023.);
    float delta = (voltagedelta /(HANDGRIPMAXVOLTAGE-HANDGRIPMINVOLTAGE)) * 
        HANDGRIPRATESCALER;
    return delta;
  }
  if(devnum == CRANKDEVNUM){
    float voltagedelta = abs(HANDGRIPMINVOLTAGE-(5*pinADCval)/1023.);
    float delta = (voltagedelta /(HANDGRIPMAXVOLTAGE-HANDGRIPMINVOLTAGE)) * 
        HANDGRIPRATESCALER;
    return delta;
  }
}

  //right now this function doesnt do much. It's a placeholder for future functionality
  //which will modify the consumption rate on the fly. 
// int _handgrip::calculateConsumptionRate(int currentConsumptionRate){
  // return currentConsumptionRate;
// }
