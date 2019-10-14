#ifndef _HANDGRIP
#define _HANDGRIP

#define GRIPPIN A0

#define HANDGRIPRATESCALER 12 
#define HANDGRIPMAXVOLTAGE 2.38
#define HANDGRIPMINVOLTAGE  2.17


class _handgrip {
  public:
   float voltageValue(void); //used to return a reading from the handgrip strain gauge on demand
   int calculateProductionRate(float pinVoltage);
  private:
  
};
#endif
