#ifndef _ENCODER
#define _ENCODER

class _encoder {
public:
  unsigned char encoderDelta(void);
  void initialize(void);
private:
  bool prevAVal;
  bool prevBVal;
  bool pinAVal;
  bool pinBVal;
  char quadratureLookupTable[] = {0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0};
  // http://makeatronics.blogspot.com/2013/02/efficiently-reading-quadrature-with.html

}

#endif