// #include <math.h>
#ifndef _LEDSTRIP
#define _LEDSTRIP

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
    float calculatePosition(int);
    bool indicatorsWithinBounds(void);
    void initialize(void);
    void setBoundingBox(int boxstart, int boxsize);
    void setConsumptionRate(float consumptionRate);
    void setIndicatorPosition(float position, int devnum);
    float setProductionRate(float productionRate, int devnum);
    float returnConsumptionRate();
    //this is used to set the rates of production and consumption for homeostasis. 
    void setRates(float productionrate, float consumptionrate, int devnum); 
    void update(void);

    
  private:
    /* VARIABLES */
    float consumptionRate = DEFAULTCONSUMPTIONRATE/100.; //stays the same for all players
    float crankProductionRate = DEFAULTPRODUCTIONRATE/100.;
    unsigned long losingColor;
    float squeezeProductionRate = DEFAULTPRODUCTIONRATE/100.;

};

#endif
