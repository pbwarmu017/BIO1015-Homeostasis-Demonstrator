#include "includes.h"
#include "parents.cpp"

#define LED_PIN 6 
#define LED_COUNT 60 //the number of LEDs on the strip
#define BOXSTART 22 //bounding box default starting position
#define BOXSIZE 10 //bounding box default size

//LED COLOR CODE DEFINITIONS. 
#define COLORRED strip.Color(128,0,0) //USED FOR PRE GAME INDICATION
#define COLORORANGE strip.Color(128,64,0) //USED FOR SECONDINDICATORCOLOR
#define COLORYELLOW strip.Color(128,128,0)
#define COLORGREEN strip.Color(0,128,0) //USED FOR "GAME ON" INDICATION
#define COLORBLUE strip.Color(0,0,128) //USED FOR SQUEEZEINDICATORCOLOR
#define COLORPURPLE strip.Color(25,0,51)
#define COLORCYAN strip.Color(0,128,128)

//AFFECTOR INDICATOR COLOR SELECTIONS
#define SQUEEZEINDICATORCOLOR COLORBLUE
#define CRANKINDICATORCOLOR COLORORANGE
 //DEFAULT RATES
#define DEFAULTINDICATORPOSITION 0//BOXSTART+round((float)BOXSIZE/2)
#define DEFAULTPRODUCTIONRATE 25 
#define DEFAULTCONSUMPTIONRATE 50
//REFRESH SPEEDS
#define STRIPREFRESHDELAY 50 //in milliseconds
#define LEDMAXINCREMENT 5 //Max # of LEDs to jump per cycle

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

class _indicatorstrip: public _device {
  public:
   /*VARIABLES*/

    int boxSize = 3; //the size of the box centered around boxposition (must be at least 3)
    int boxStart = BOXSTART; //the position of the bounding box as an LED number the box is centered around. 
    int boxLowerBound;
    int boxUpperBound;
    float crankIndicatorPosition = DEFAULTINDICATORPOSITION;
    double prevSqueezeIndPos = -1;
    double prevCrankIndPos = -1;
    float squeezeIndicatorPosition = DEFAULTINDICATORPOSITION; 
    /*FUNCTIONS*/
    float calculatePosition(int devnum){
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
    bool indicatorsWithinBounds(void){
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
    void initialize(void){
      strip.begin();
      strip.show();
      return;
    }
    void setBoundingBox(int boxstart, int boxsize){
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
    void setConsumptionRate(float consrate){
      consumptionRate = 1+consrate/100.;
      return;
    }
    void setIndicatorPosition(float position, int devnum){
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
    float setProductionRate(float prodrate, int devnum) {
      if(devnum == HANDGRIPDEVNUM){
        squeezeProductionRate = prodrate/100.;
      }
      if(devnum == CRANKDEVNUM){
        crankProductionRate = prodrate/100.;
      }
      return;
    }
    float returnConsumptionRate(){
      return consumptionRate;
    }
    //this is used to set the rates of production and consumption for homeostasis. 
    void setRates(float productionrate, float consumptionrate, int devnum); 
    void update(void){
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
  private:
    // _indicatorstrip() {
    // }
    /* VARIABLES */
    float consumptionRate = DEFAULTCONSUMPTIONRATE/100.; //stays the same for all players
    float crankProductionRate = DEFAULTPRODUCTIONRATE/100.;
    unsigned long losingColor;
    float squeezeProductionRate = DEFAULTPRODUCTIONRATE/100.;
};


