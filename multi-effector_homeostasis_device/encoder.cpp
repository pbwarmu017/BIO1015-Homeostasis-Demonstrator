#include "headers.h"
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
//using this in conjuction with a low pass filter helps clean up switch bounce
//http://makeatronics.blogspot.com/2013/02/efficiently-reading-quadrature-with.html to see how it works
char _encoder::returnDelta(void){
  pinAVal = digitalRead(ENCODERPINA);
  pinBVal = digitalRead(ENCODERPINB);
  unsigned char lookupVal = (prevAVal << 3) | (prevBVal << 2) | (pinAVal << 1) | pinBVal;
  prevAVal = pinAVal;
  prevBVal = prevBVal;
  return quadratureLookupTable[lookupVal];
}

//we want to limit the crank to 1 RPM, as the encoder is only rated to that. It will
//also make the device last longer as students wont be wild with it.
//return a number between 0 and CRANKRATESCALER
int _encoder::calculateProductionRate(int crankSum){
	// calculates current moving average efficiently
  movingAverage += -movingAverage/movingAveragePeriod + crankSum;
	//make it pointless to spin the crank faster than the max spec RPM of 60
	//per the data sheet. It is a 24 position encoder and the moving average
  	//is calculated over a period of 1 second. So we are trimming any pulses over 
  	//24 in this function
  if(movingAverage > 24) movingAverage = 24;
  //prevent excessively small carryover
  if(movingAverage < 0.01) movingAverage = 0;
  // productionRate = movingAverage/24 * CRANKRATESCALER;
  return(movingAverage/24 * CRANKRATESCALER);
}