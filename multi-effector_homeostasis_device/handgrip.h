#ifndef _HANDGRIP
#define _HANDGRIP

#define GRIPPIN A0

#define HANDGRIPRATESCALER 45 
#define HANDGRIPMAXVOLTAGE 2.66
#define HANDGRIPMINVOLTAGE  2.45


class _handgrip {
  public:
   float voltageValue(void); //used to return a reading from the handgrip strain gauge on demand
   int calculateProductionRate(float pinVoltage, int devnum);
   // int calculateConsumptionRate(int currentConsumptionRate);
  private:
  
};
#endif
