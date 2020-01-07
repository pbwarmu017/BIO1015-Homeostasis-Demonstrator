#ifndef _HANDGRIP
#define _HANDGRIP

// #define HANDGRIPRATESCALER 75
// #define HANDGRIPMINVOLTAGE 2.62 
// #define HANDGRIPMAXVOLTAGE 2.79 

class _handgrip: public _affector {
  public:
  	_handgrip();
   float voltageValue(void); //used to return a reading from the handgrip strain gauge on demand
   int calculateProductionRate(float pinVoltage, _handgrip *handgrip);
  	int calibrationState = 0; //standby state
  	float handgripMinVoltage;
  	float handgripMaxVoltage;
    float handgripPrescaler = 75;
    float productionRate = 0;
  private:
};
#endif
