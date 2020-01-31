
#ifndef INDICATORSTRIP_CPP
#define INDICATORSTRIP_CPP

#include "superclasses.cpp"
//LED COLOR CODE DEFINITIONS. 
#define COLORRED strip->Color(128,0,0) //USED FOR PRE GAME INDICATION
#define COLORPINK strip->Color(64,26,45)
#define COLORWHITE strip->Color(64,64,64)
#define COLORORANGE strip->Color(70,32,0) //USED FOR SECONDINDICATORCOLOR
#define COLORYELLOW strip->Color(64,64,0)
#define COLORGREEN strip->Color(0,64,0) //USED FOR "GAME ON" INDICATION
#define COLORBLUE strip->Color(0,0,64) //USED FOR SQUEEZEINDICATORCOLOR
#define COLORCYAN strip->Color(0,64,64)
#define COLORVIOLET strip->Color(25,0,51)

//PORT COLOR SELECTIONS
#define DCON1COLOR COLORWHITE
#define ACON1COLOR COLORORANGE
#define DACON1COLOR COLORYELLOW
#define DCON2COLOR COLORBLUE
#define ACON2COLOR COLORCYAN
#define DACON2COLOR COLORVIOLET

//DEFAULT RATES
#define DEFAULTINDICATORPOSITION 0//BOXSTART+round((float)BOXSIZE/2)

//REFRESH SPEEDS
#define STRIPREFRESHDELAY 43 //in milliseconds
#define LEDMAXINCREMENT 5 //Max # of LEDs to jump per cycle


class _indicatorstrip: public _device 
{

  public:
     /*VARIABLES*/

    //MAKE THE BOX CONFIGURABLE IN THE MENU
    Adafruit_NeoPixel *strip;
    int boxSize = 3; //the size of the box centered around boxposition (must be at least 3)
    int boxStart = BOXSTART; //the position of the bounding box as an LED number the box is centered around. 
    int boxLowerBound;
    int boxUpperBound;
    int LED_PIN = 6;
    int LED_COUNTa = 60; //the number of LEDs on the strip
    int BOXSTART = 22; //bounding box default starting position
    int BOXSIZE = 10; //bounding box default size
    // bool handgripStatus[2] = {0};
    // bool handcrankStatus[2] = {0};
    // int HANDGRIPACTIVE = 0;
    // int CRANKACTIVE = 0;

    // double prevSqueezeIndPos = -1;
    // double prevCrankIndPos = -1;
    /*FUNCTIONS*/
    // float calculatePosition(int devnum) 
    // // {
    // //   if(devnum == HANDGRIPDEVNUM)
    // //   {
    // //     float delta = squeezeProductionRate-consumptionRate;
    // //     return(squeezeIndicatorPosition+(LEDMAXINCREMENT * delta));  
    // //   }
    // //   if(devnum == CRANKDEVNUM)
    // //   {
    // //     float delta = crankProductionRate-consumptionRate;
    // //     return(crankIndicatorPosition+(LEDMAXINCREMENT * delta));
    // //   }
    // //   return 0;
    // // }
    // bool indicatorsWithinBounds(void) 
    // // {
    // //   //checking for the squeeze indicator
    // //   bool statusVariable = true;

    // //   if(HANDGRIPACTIVE == 1)
    // //   {
    // //     if(squeezeIndicatorPosition <= boxLowerBound)
    // //     {
    // //       losingColor = SQUEEZEINDICATORCOLOR;
    // //       statusVariable = false;
    // //     }
    // //     if(squeezeIndicatorPosition >= boxUpperBound) 
    // //     {
    // //       losingColor = SQUEEZEINDICATORCOLOR;
    // //       statusVariable = false;
    // //     }
    // //   }

    // //   //checking for the hand crank indicator
    // //   if(CRANKACTIVE == 1)
    // //   {
    // //     if(crankIndicatorPosition <= boxLowerBound)
    // //     {
    // //       losingColor = CRANKINDICATORCOLOR;
    // //       statusVariable =  false;
    // //     }

    // //     if(crankIndicatorPosition >= boxUpperBound) 
    // //     {
    // //       losingColor = CRANKINDICATORCOLOR;
    // //       statusVariable =  false;
    // //     }
    // //   }
    // //   return(statusVariable);
    // // }
    // void initialize(void) 
    // {
    //   strip->begin();
    //   strip->show();
    //   return;
    // }
    // void setBoundingBox(int boxstart, int boxsize) 
    // {
    //   boxLowerBound = boxstart;
    //   boxUpperBound = boxstart+boxsize+1;


    //   if(gameStatus == started && indicatorsWithinBounds())
    //   {
    //     strip->setPixelColor(boxstart, COLORGREEN);
    //     strip->setPixelColor(boxstart+boxsize+1, COLORGREEN);
    //   }
    //   if(gameStatus == started && !indicatorsWithinBounds())
    //   {
    //     gameStatus = lost;
    //     strip->setPixelColor(boxstart, losingColor);
    //     strip->setPixelColor(boxstart+boxsize+1, losingColor);
    //   }

    //   if(gameStatus == notstarted && indicatorsWithinBounds())
    //   {
    //     gameStatus = started;
    //     strip->setPixelColor(boxstart, COLORGREEN);
    //     strip->setPixelColor(boxstart+boxsize+1, COLORGREEN);
    //   }
    //   if(gameStatus == notstarted && !indicatorsWithinBounds())
    //   {
    //     strip->setPixelColor(boxstart, COLORRED);
    //     strip->setPixelColor(boxstart+boxsize+1, COLORRED);
    //   }

    //   if(gameStatus == lost)
    //   {
    //     strip->setPixelColor(boxstart, losingColor);
    //     strip->setPixelColor(boxstart+boxsize+1, losingColor);
    //   }

    //   strip->show();
    //   return;
    // }
    // void setConsumptionRate(float consrate) 
    // {
    //   consumptionRate = 1+consrate/100.;
    //   return;
    // }
    // void setIndicatorPosition(float position, int devnum) 
    // {
    //   if(devnum == HANDGRIPDEVNUM)
    //   {
    //     strip->setPixelColor(prevSqueezeIndPos, 0,0,0);
    //     if(position < LED_COUNTa && position >= 0)
    //     {
    //       squeezeIndicatorPosition = position;
    //       strip->setPixelColor(position, SQUEEZEINDICATORCOLOR);
    //       prevSqueezeIndPos = (int)position;
    //     }
    //   }
    //   if(devnum == CRANKDEVNUM)
    //   {
    //     strip->setPixelColor(prevCrankIndPos, 0,0,0);
    //     if(position < LED_COUNTa && position >= 0)
    //     {
    //       crankIndicatorPosition = position;
    //       strip->setPixelColor(position, CRANKINDICATORCOLOR);
    //       prevCrankIndPos = (int)position;
    //     }
    //   }
    //   return;
    // }
    void returnDelta(float rate, int devnum)
    {
      return;
    }

    void updatePositions(void)
    {
      return;
    }
    //used to set the production rate. 
    //prodrate: a number between 0 and 100.
    //the final set rate will be a number between 0 and 1. 
    // float setProductionRate(float prodrate, int devnum) 
    // {
    //   if(devnum == HANDGRIPDEVNUM)
    //   {
    //     squeezeProductionRate = prodrate/100.;
    //   }
    //   if(devnum == CRANKDEVNUM)
    //   {
    //     crankProductionRate = prodrate/100.;
    //   }
    // }
    // float returnConsumptionRate() 
    // {
    //   return consumptionRate;
    // }
    //this is used to set the rates of production and consumption for homeostasis. 
    // void setRates(float productionrate, float consumptionrate, int devnum); 
    void update(void) 
    {
      strip->show();
    }
    _indicatorstrip()
    {
      strip = new Adafruit_NeoPixel(LED_COUNTa, LED_PIN, NEO_GRB + NEO_KHZ800);
      strip->begin();
      strip->setPixelColor(20, COLORRED);
      strip->show();
    }
    // int updateStatus(int objtype)
    // {
    //   int status = 0;
    //   if(objtype == HANDGRIP_TYPE)
    //   {
    //     for(int i = 0; i < sizeof(handgripStatus)/sizeof(handgripStatus[0]); i++)
    //     {
    //       status |= handgripStatus[i];
    //       return(status);
    //     }
    //   }

    //   if(objtype == HANDCRANK_TYPE)
    //   {
    //     for(int i = 0; i < sizeof(handcrankStatus)/sizeof(handcrankStatus[0]); i++)
    //     {
    //       status |= handcrankStatus[i];
    //       return(status);
    //     }
    //   }
    // }
    ~_indicatorstrip()
    {
      //must define a destructor, but I have no use for it here. `
    }
  private:
    unsigned long losingColor;
};

#endif