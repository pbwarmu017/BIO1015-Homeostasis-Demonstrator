#ifndef _ENCODER
#define _ENCODER
#define CRANKRATECALCDELAY 500 //value is in milliseconds
#define CRANKRATEMAX 24
#define CRANKRATESCALER 60
class _encoder {
public:
  char returnDelta(void);
  void initialize(void);
  int calculateProductionRate(float count, float time);
private:
  volatile bool prevAVal;
  volatile bool prevBVal;
  volatile bool pinAVal;
  volatile bool pinBVal;
  // char quadratureLookupTable[16] = {0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0};
  char quadratureLookupTable[16] = {0,0,0,0,0,0,0,-1,0,0,0,0,0,1,0,0};
  // http://makeatronics.blogspot.com/2013/02/efficiently-reading-quadrature-with.html

};

#endif