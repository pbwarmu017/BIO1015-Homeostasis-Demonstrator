#include <Adafruit_NeoPixel.h>
#include "multi-effector_homeostasis_device.h"

#include "ledstrip.h"
#include "handgrip.h"
#include "encoder.h"

volatile int stripDelayCounter = 0; //used to track timer overflows for refreshing the strip
// volatile int encoderDelayCounter = 0; //used to track timer overlfows for polling the encoder position
// volatile unsigned long lastTriggerTime = micros(); //holds the time of the last trigger from micros()
volatile int prevOut = 0;
volatile float crankRateCalcDelayCounter = 0;
volatile float crankSum = 0;

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
_encoder Handcrank; //object for the encoder

//this is the interrupt handler for Timer0 output conpare match. 
SIGNAL(TIMER0_COMPA_vect) { //this executes every 1 millisecond
  stripDelayCounter++;
  crankRateCalcDelayCounter++;
  if(stripDelayCounter == STRIPREFRESHDELAY) {  

    //Set Rates based on affector positions (one for each affector)
    if(HANDGRIPACTIVE){
      Indicatorstrip.setProductionRate(Handgrip.calculateProductionRate(
        analogRead(HANDGRIPPIN)), HANDGRIPDEVNUM);
      //set the indicator positions based on the rates 
      //calculated above (one for each indicator)
      Indicatorstrip.setIndicatorPosition(
        Indicatorstrip.calculatePosition(HANDGRIPDEVNUM), HANDGRIPDEVNUM);
    }
    if(CRANKACTIVE){
      if(crankRateCalcDelayCounter == CRANKRATECALCDELAY){
        Indicatorstrip.setProductionRate(Handcrank.calculateProductionRate(
          crankSum, crankRateCalcDelayCounter), CRANKDEVNUM);
      }
      Indicatorstrip.setIndicatorPosition(
        Indicatorstrip.calculatePosition(CRANKDEVNUM), CRANKDEVNUM);
    }

    //set the bounding box. 
    Indicatorstrip.setBoundingBox(BOXSTART, BOXSIZE);
    Indicatorstrip.update();

    stripDelayCounter = 0; //reset the timer counter for the next run. 
  }


}


ISR (PCINT2_vect) { // handle pin change interrupt for D0 to D7 here
  if(CRANKACTIVE){
    int currentOut = Handcrank.returnDelta();
      if(currentOut){ //make sure it's not an invalid state change
        if(currentOut == prevOut){
          crankSum++;
          // Serial.println(currentOut);
        }
        prevOut = currentOut; //update the previous value
    }
  }
}

void setup() {

  //setup timer0 to call interrupt OCR0A every REFRESHTIMEVAL
  OCR0A = 0xFA; //set to trigger TIMER0_COMPA_vect every millisecond
  TIMSK0 |= _BV(OCIE0A); //enable the output compare interrupt on timer0

  //setup interrupt on pin change for the ender pins.
  //This interrupt is enabled per group (digital pins 1-7, digtial pins 8-13)
  // enable pin change interrupt for encoder pin A
  *digitalPinToPCMSK(ENCODERPINA) |= bit (digitalPinToPCMSKbit(ENCODERPINA)); 
  // enable ping change interrupt for encoder pin B
  *digitalPinToPCMSK(ENCODERPINB) |= bit (digitalPinToPCMSKbit(ENCODERPINB)); 

  PCIFR  |= bit (digitalPinToPCICRbit(ENCODERPINA)); // clear any outstanding interrupt flag
  PCICR  |= bit (digitalPinToPCICRbit(ENCODERPINA)); // enable interrupt for the GROUP 

  systemMode = NONE;
  Indicatorstrip.initialize();
  Handcrank.initialize();
  Serial.begin(9600); //init serial for debugging  
}

void loop() {
//its all handled by interrrupts. 
}
