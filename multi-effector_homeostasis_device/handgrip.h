#ifndef _HANDGRIP
#define _HANDGRIP

#define GRIPPIN A0

#define HANDGRIPRATESCALER 40
#define HANDGRIPMINVOLTAGE 2.47 
#define HANDGRIPMAXVOLTAGE  2.68 

class _handgrip {
  public:
   float voltageValue(void); //used to return a reading from the handgrip strain gauge on demand
   int calculateProductionRate(float pinVoltage);
  	bool calibrationState = false;
  	float handgripMinVoltage = 1.88;
  	float handgripMaxVoltage = 0;
    float productionRate = 0;
  private:
};
#endif
