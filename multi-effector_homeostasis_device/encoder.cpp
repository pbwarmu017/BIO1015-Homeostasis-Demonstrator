#include "multi-effector_homeostasis_device.h"
#include "encoder.h"
//set up the pins ands store the current values for the encoder
void _encoder::initialize(void){
  pinMode(ENCODERPINA, input);
  pinMOde(ENCODERPINB, input);
  prevAVal = digitalRead(ENCODERPINA);
  prevBVal = digitalRead(ENCODERPINB);
}

//polls the encoder pins and evaluates if the encoder has moved forward, backward, or stayed the same. 
//returns 0 if no change, 1 if a CW movement was detected, -1 if a CCW movement was detected. 
//http://makeatronics.blogspot.com/2013/02/efficiently-reading-quadrature-with.html to see how it works
unsigned char _encoder::encoderDelta(void){
 pinAVal = digitalRead(ENCODERPINA);
 pinBVal = digitalRead(ENCODERPINB);
 unsigned char lookupval = 0;
 lookupVal = (prevAVal << 3) | (prevBVal << 2) | (pinAVal << 1) | pinBVal;
 prevAVal = pinAVal;
 prevBVal = prevBVal;
 return quadratureLookupTable[lookupVal];
}