#include <Adafruit_NeoPixel.h>
#include "multi-effector_homeostasis_device.h"
#include "encoder.h"
//set up the pins ands store the current values for the encoder

//check https://playground.arduino.cc/Main/RotaryEncoders/#OnInterrupts
//TO DO: Change to pin change interrupts and do software debouncing. 


void _encoder::initialize(void){
  pinMode(ENCODERPINA, INPUT_PULLUP);
  pinMode(ENCODERPINB, INPUT_PULLUP);
  prevAVal = digitalRead(ENCODERPINA);
  prevBVal = digitalRead(ENCODERPINB);
}

//polls the encoder pins and evaluates if the encoder has moved forward, backward, or stayed the same. 
//returns 0 if no change, 1 if a CW movement was detected, -1 if a CCW movement was detected. 
//this also removes the need for filtering/debouncing as only valid state changes map to a return value besides 0. 
//http://makeatronics.blogspot.com/2013/02/efficiently-reading-quadrature-with.html to see how it works
char _encoder::returnDelta(void){
pinAVal = digitalRead(ENCODERPINA);
pinBVal = digitalRead(ENCODERPINB);
unsigned char lookupval = 0;
char lookupVal = (prevAVal << 3) | (prevBVal << 2) | (pinAVal << 1) | pinBVal;
prevAVal = pinAVal;
prevBVal = prevBVal;
return quadratureLookupTable[lookupVal];
}

//we want to limit the crank to 1 RPM, as the encoder is only rated to that. It will
//also make the device last longer as students wont be wild with it. 
int calculateProductionRate(float count, float time){
	if(count > CRANKRATEMAX) {
		return(CRANKRATESCALER); //this is the max rate allowed
	}
	return(count/CRANKRATEMAX * CRANKRATESCALER);
}