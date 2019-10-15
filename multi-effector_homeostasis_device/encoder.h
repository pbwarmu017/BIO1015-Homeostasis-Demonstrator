#ifndef _ENCODER
#define _ENCODER
#define CRANKRATECALCDELAY 250 //value is in milliseconds
#define CRANKRATEMAX 24
#define CRANKRATESCALER 30
class _encoder {
public:
  char returnDelta(void);
  void initialize(void);
  int calculateProductionRate(int crankSum);
private:
  volatile bool prevAVal;
  volatile bool prevBVal;
  volatile bool pinAVal;
  volatile bool pinBVal;
  float movingAverage = 0; //holds the moving average for the production of the hand crank. 
  float movingAveragePeriod = 1000/CRANKRATECALCDELAY; 
  // char quadratureLookupTable[16] = {0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0};
  //removed extraneous values to help prevent bouncing, and inverted the polarity
  char quadratureLookupTable[16] = {0,0,0,0,0,0,0,1,0,0,0,0,0,-1,0,0};
  // http://makeatronics.blogspot.com/2013/02/efficiently-reading-quadrature-with.html

};

#endif