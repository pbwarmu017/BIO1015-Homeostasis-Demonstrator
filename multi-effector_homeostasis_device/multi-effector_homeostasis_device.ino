#include <Adafruit_NeoPixel.h>
#include "multi-effector_homeostasis_device.h"

#include "ledstrip.h"
#include "handgrip.h"
#include "encoder.h"

volatile int stripDelayCounter = 0; //used to track timer overflows for refreshing the strip
volatile int encoderDelayCounter = 0; //used to track timer overlfows for polling the encoder position

enum SYSTEMMODE {
  NONE, //this is the default mode, but "default" is reserved by the compiler. 
  HANDGRIP,
  HANDCRANK,
  AuDIOVOLUME,
  AUDIOPITCH,
  ANOMETER
}systemMode;

_indicatorstrip Indicatorstrip; //object for the indicatorstrip
_handgrip Handgrip; //object for the handgrip
_encoder Encoder; //object for the encoder

//this is the interrupt handler for Timer0 output conpare match. 
SIGNAL(TIMER0_COMPA_vect) { //this executes every 1 millisecond
  stripDelayCounter++;
  if(stripDelayCounter == STRIPREFRESHDELAY) {  

    //Set Rates based on affector positions (one for each affector)
    Indicatorstrip.setRates(Handgrip.calculateProductionRate(
      analogRead(HANDGRIPPIN), HANDGRIPDEVNUM), 35, HANDGRIPDEVNUM);

    Indicatorstrip.setRates(Handgrip.calculateProductionRate( //THIS WILL NEED TO CHANGE WHEN I BUILD THE HANDGRIP
      analogRead(CRANKPIN), CRANKDEVNUM), 35, CRANKDEVNUM);

    //set the indicator positions based on the rates 
    //calculated above (one for each indicator)
    Indicatorstrip.setIndicatorPosition(Indicatorstrip.calculatePosition(HANDGRIPDEVNUM), 
      HANDGRIPDEVNUM);

    Indicatorstrip.setIndicatorPosition(Indicatorstrip.calculatePosition(CRANKDEVNUM), 
      CRANKDEVNUM);

    //set the bounding box. 

    Indicatorstrip.setBoundingBox(BOXSTART, BOXSIZE);
    Indicatorstrip.update();

    tmr0counter = 0; //reset the timer counter for the next run. 
  }
}

void setup() {

  //setup timer0 to call interrupt OCR0A every REFRESHTIMEVAL
  OCR0A = 0xFA; //set to trigger TIMER0_COMPA_vect every millisecond
  TIMSK0 |= _BV(OCIE0A); //enable the output compare interrupt on timer0

  systemMode = NONE;
  Indicatorstrip.initialize();
  Encoder.initialize();
  Serial.begin(9600); //init serial for debugging  
}

void loop() {
//its all handled by interrrupts. 
}
