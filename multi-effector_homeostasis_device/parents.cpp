#ifndef PARENTS_CPP
#define PARENTS_CPP

#include "multi-effector_homeostasis_device.h"

class _device {
  public:
};

class _affector: public _device {
  public:
    float returnPosition(float currentPosition){
      return(currentPosition);
    }
};
#endif