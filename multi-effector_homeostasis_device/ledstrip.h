#include <math.h>
#ifndef _LEDSTRIP
#define _LEDSTRIP

#define LED_PIN 6 
#define LED_COUNT 60 //the number of LEDs on the strip
#define BOXSTART 22
#define BOXSIZE 10

//LED COLOR CODE DEFINITIONS. 
#define COLORRED strip.Color(255,0,0) //USED FOR PRE GAME INDICATION
#define COLORORANGE strip.Color(255,128,0) //USED FOR SECONDINDICATORCOLOR
#define COLORYELLOW strip.Color(255,255,0)
#define COLORGREEN strip.Color(0,255,0) //USED FOR "GAME ON" INDICATION
#define COLORBLUE strip.Color(0,0,255) //USED FOR SQUEEZEINDICATORCOLOR
#define COLORPURPLE strip.Color(51,0,102)
#define COLORCYAN strip.Color(0,255,255)


#define SQUEEZEINDICATORCOLOR COLORBLUE
#define CRANKINDICATORCOLOR COLORORANGE

#define DEFAULTINDICATORPOSITION 0//BOXSTART+round((float)BOXSIZE/2)
#define DEFAULTPRODUCTIONRATE 20
#define DEFAULTCONSUMPTIONRATE 20
#define STRIPREFRESHDELAY 50 //in milliseconds
#define LEDMAXINCREMENT 10


class _indicatorstrip {
  public:
   /*VARIABLES*/

    int boxSize = 3; //the size of the box centered around boxposition (must be at least 3)
    int boxStart = BOXSTART; //the position of the bounding box as an LED number the box is centered around. 
    int boxLowerBound;
    int boxUpperBound;
    // int indicatorPosition;
    float squeezeIndicatorPosition = DEFAULTINDICATORPOSITION; 
    float crankIndicatorPosition = DEFAULTINDICATORPOSITION;
    double prevSqueezeIndPos = -1;
    double prevCrankIndPos = -1;
    /*FUNCTIONS*/
    void initialize(void);
    void update(void);

    float calculatePosition(int);

    void setIndicatorPosition(float position, int devnum);
    void clearIndicator(void); //clears the indicator from the strip
    int setBoundingBox(int boxstart, int boxsize);

    void setRates(float productionrate, float consumptionrate, int devnum); //this is used to set the rates of production and consumption for homeostasis. 
    void setProductionRate(float productionRate, int devnum);
    void setConsumptionRate(float consumptionRate);

    bool indicatorsWithinBounds(void);
    
  private:
    /* VARIABLES */
    float consumptionRate = DEFAULTCONSUMPTIONRATE; //stays the same for all players
    float squeezeProductionRate = DEFAULTPRODUCTIONRATE;
    float crankProductionRate = DEFAULTPRODUCTIONRATE;
    unsigned long losingColor;

    bool GAMEON = false;
};

#endif
