#include "ledstrip.h"
#include <Adafruit_NeoPixel.h>
#include "multi-effector_homeostasis_device.h"

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

enum GAMESTATUS {
  notstarted,
  started,
  lost
}gameStatus;

float _indicatorstrip::calculatePosition(int devnum){
  if(devnum == HANDGRIPDEVNUM){
    float squeezeDeltaPercentage = (squeezeProductionRate-consumptionRate)/consumptionRate;
    return(squeezeIndicatorPosition+(LEDMAXINCREMENT * squeezeDeltaPercentage));  
  }
  if(devnum == CRANKDEVNUM){
    float crankDeltaPercentage = (crankProductionRate-consumptionRate)/consumptionRate;
    return(crankIndicatorPosition+(LEDMAXINCREMENT * crankDeltaPercentage));
  }
}

bool _indicatorstrip::indicatorsWithinBounds(void){
  //checking for the squeeze indicator
  // if(squeezeIndicatorPosition <= boxLowerBound){
  //   losingColor = SQUEEZEINDICATORCOLOR;
  //   return false;
  // }

  // if(squeezeIndicatorPosition > boxLowerBound 
  //     && squeezeIndicatorPosition < boxUpperBound) {
  //   return true;
  // }

  // if(squeezeIndicatorPosition >= boxUpperBound) {
  //   losingColor = SQUEEZEINDICATORCOLOR;
  //   return false;
  // }

  //checking for the hand crank indicator
  if(crankIndicatorPosition <= boxLowerBound){
    losingColor = CRANKINDICATORCOLOR;
    return false;
  }

  if(crankIndicatorPosition > boxLowerBound 
      && crankIndicatorPosition < boxUpperBound) {
    return true;
  }

  if(crankIndicatorPosition >= boxUpperBound) {
    losingColor = CRANKINDICATORCOLOR;
    return false;
  }
}

void _indicatorstrip::initialize(void){
  strip.begin();
  //strip.fill(strip.Color(0,0,128),0,LED_COUNT);
  strip.show();
  //setBoundingbox(BOXPOSITION, BOXSIZE);
}

int _indicatorstrip::setBoundingBox(int boxstart, int boxsize){
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
  return 0;
}
//used to set the consumption rate
void _indicatorstrip::setConsumptionRate(float consrate){
  consumptionRate = 1+consrate/100.;
}

void _indicatorstrip::setIndicatorPosition(float position, int devnum){
  // if(prevSqueezeIndPos != -1){
  if(devnum == HANDGRIPDEVNUM){
    strip.setPixelColor(prevSqueezeIndPos, 0,0,0);
    // strip.setPixelColor(prevSqueezeIndPos + 1, 0,0,0);
    if(position < LED_COUNT && position >= 0){
      squeezeIndicatorPosition = position;
      strip.setPixelColor(position, SQUEEZEINDICATORCOLOR);
      prevSqueezeIndPos = (int)position;
    }
  }
  if(devnum == CRANKDEVNUM){
    strip.setPixelColor(prevCrankIndPos, 0,0,0);
  // strip.setPixelColor(prevSqueezeIndPos + 1, 0,0,0);
    if(position < LED_COUNT && position >= 0){
      crankIndicatorPosition = position;
      strip.setPixelColor(position, CRANKINDICATORCOLOR);
      prevCrankIndPos = (int)position;
    }
  }
  return 0;
}

//used to set the production rate. Production rate is a number between 0 and 100. 
void _indicatorstrip::setProductionRate(float prodrate, int devnum) {
  if(devnum == HANDGRIPDEVNUM){
    squeezeProductionRate = 1+prodrate/100.;
  }
  if(devnum == CRANKDEVNUM){
    crankProductionRate = 1+prodrate/100.;
  }
}
//used to set the consumption rate and the production rate
void _indicatorstrip::setRates(float prodrate, float consrate, int devnum){
  if(devnum == HANDGRIPDEVNUM){
    squeezeProductionRate = 1+prodrate/100.;
  }
  if(devnum == CRANKDEVNUM){
    crankProductionRate = 1+prodrate/100.;
  }

  consumptionRate = 1+consrate/100.;
}

void _indicatorstrip::update(void){
  strip.show();
}


