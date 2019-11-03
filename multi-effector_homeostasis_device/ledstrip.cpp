#include <Adafruit_NeoPixel.h>
#include "ledstrip.h"
#include "multi-effector_homeostasis_device.h"

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

float _indicatorstrip::calculatePosition(int devnum){
  if(devnum == HANDGRIPDEVNUM){
    float delta = squeezeProductionRate-consumptionRate;
    return(squeezeIndicatorPosition+(LEDMAXINCREMENT * delta));  
  }
  if(devnum == CRANKDEVNUM){
    float delta = crankProductionRate-consumptionRate;
    return(crankIndicatorPosition+(LEDMAXINCREMENT * delta));
  }
  return 0;
}

bool _indicatorstrip::indicatorsWithinBounds(void){
  //checking for the squeeze indicator
  bool statusVariable = true;

  if(HANDGRIPACTIVE == 1){
    if(squeezeIndicatorPosition <= boxLowerBound){
      losingColor = SQUEEZEINDICATORCOLOR;
      statusVariable = false;
    }

    if(squeezeIndicatorPosition >= boxUpperBound) {
      losingColor = SQUEEZEINDICATORCOLOR;
      statusVariable = false;
    }
  }

  //checking for the hand crank indicator
  if(CRANKACTIVE == 1){
    if(crankIndicatorPosition <= boxLowerBound){
      losingColor = CRANKINDICATORCOLOR;
      statusVariable =  false;
    }

    if(crankIndicatorPosition >= boxUpperBound) {
      losingColor = CRANKINDICATORCOLOR;
      statusVariable =  false;
    }
  }
  return(statusVariable);
}

void _indicatorstrip::initialize(void){
  strip.begin();
  strip.show();
  return;
}

void _indicatorstrip::setBoundingBox(int boxstart, int boxsize){
  boxLowerBound = boxstart;
  boxUpperBound = boxstart+boxsize+1;


  if(gameStatus == started && indicatorsWithinBounds()){
    strip.setPixelColor(boxstart, COLORGREEN);
    strip.setPixelColor(boxstart+boxsize+1, COLORGREEN);
  }
  if(gameStatus == started && !indicatorsWithinBounds()){
    gameStatus = lost;
    strip.setPixelColor(boxstart, losingColor);
    strip.setPixelColor(boxstart+boxsize+1, losingColor);
  }

  if(gameStatus == notstarted && indicatorsWithinBounds()){
    gameStatus = started;
    strip.setPixelColor(boxstart, COLORGREEN);
    strip.setPixelColor(boxstart+boxsize+1, COLORGREEN);
  }
  if(gameStatus == notstarted && !indicatorsWithinBounds()){
    strip.setPixelColor(boxstart, COLORRED);
    strip.setPixelColor(boxstart+boxsize+1, COLORRED);
  }

  if(gameStatus == lost){
    strip.setPixelColor(boxstart, losingColor);
    strip.setPixelColor(boxstart+boxsize+1, losingColor);
  }

  strip.show();
  return;
}
//used to set the consumption rate
void _indicatorstrip::setConsumptionRate(float consrate){
  consumptionRate = 1+consrate/100.;
  return;
}

void _indicatorstrip::setIndicatorPosition(float position, int devnum){
  if(devnum == HANDGRIPDEVNUM){
    strip.setPixelColor(prevSqueezeIndPos, 0,0,0);
    if(position < LED_COUNT && position >= 0){
      squeezeIndicatorPosition = position;
      strip.setPixelColor(position, SQUEEZEINDICATORCOLOR);
      prevSqueezeIndPos = (int)position;
    }
  }
  if(devnum == CRANKDEVNUM){
    strip.setPixelColor(prevCrankIndPos, 0,0,0);
    if(position < LED_COUNT && position >= 0){
      crankIndicatorPosition = position;
      strip.setPixelColor(position, CRANKINDICATORCOLOR);
      prevCrankIndPos = (int)position;
    }
  }
  return;
}

//used to set the production rate. 
//prodrate: a number between 0 and 100.
//the final set rate will be a number between 0 and 1. 
void _indicatorstrip::setProductionRate(float prodrate, int devnum) {
  if(devnum == HANDGRIPDEVNUM){
    squeezeProductionRate = prodrate/100.;
  }
  if(devnum == CRANKDEVNUM){
    crankProductionRate = prodrate/100.;
  }
  return;
}

void _indicatorstrip::update(void){
  strip.show();
  if(STRIPTESTMODE){
    if(HANDGRIPACTIVE == 1){
      Serial.print(" GRIP LED POSITION: ");
      
    }
    if(CRANKACTIVE == 1){
      Serial.print("CRANK LED POSITION: ");
      Serial.println(crankIndicatorPosition);
    }
  }
  return;
}


