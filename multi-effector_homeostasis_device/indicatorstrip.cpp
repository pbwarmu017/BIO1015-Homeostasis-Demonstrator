
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
    float deviceIndicatorPosition[6] = {-2,-2,-2,-2,-2,-2}; //array to track indicator positions for the different ports
    int LED_PIN = 6;
    int LED_COUNTa = 60; //the number of LEDs on the strip

    //this function is responsible for pushing the currentindicator values out to the strip and drawing in the 
    //bounding box. The objects themselves cause their positions to be updated with updatePosition(). 
    void refreshStrip(void) 
    {
      //clear all assignments on the strip
      strip->clear();

      //set all indicators on the strip to their current position and color
      //position can be from -2 to 60, and colors are defined above
      //portnums are defined in superclasses.cpp
      strip->setPixelColor(deviceIndicatorPosition[DCON1_PORTNUM], DCON1COLOR);
      strip->setPixelColor(deviceIndicatorPosition[ACON1_PORTNUM], ACON1COLOR);
      strip->setPixelColor(deviceIndicatorPosition[DACON1_PORTNUM], DACON1COLOR);
      strip->setPixelColor(deviceIndicatorPosition[DCON2_PORTNUM], DCON2COLOR);
      strip->setPixelColor(deviceIndicatorPosition[ACON2_PORTNUM], ACON2COLOR);
      strip->setPixelColor(deviceIndicatorPosition[DACON2_PORTNUM], DACON2COLOR); 

      //draw out the current bounding box. Needs to be the last thing done before sending it to the strip

      if(gameStatus == started && indicatorsWithinBounds())
      {
        strip->setPixelColor(boxStart, COLORGREEN);
        strip->setPixelColor(boxStart+boxSize+1, COLORGREEN);
      }
      if(gameStatus == started && !indicatorsWithinBounds())
      {
        gameStatus = lost;
        strip->setPixelColor(boxStart, losingColor);
        strip->setPixelColor(boxStart+boxSize+1, losingColor);
      }

      if(gameStatus == notstarted && indicatorsWithinBounds())
      {
        gameStatus = started;
        strip->setPixelColor(boxStart, COLORGREEN);
        strip->setPixelColor(boxStart+boxSize+1, COLORGREEN);
      }
      if(gameStatus == notstarted && !indicatorsWithinBounds())
      {
        strip->setPixelColor(boxStart, COLORRED);
        strip->setPixelColor(boxStart+boxSize+1, COLORRED);
      }

      if(gameStatus == lost)
      {
        strip->setPixelColor(boxStart, losingColor);
        strip->setPixelColor(boxStart+boxSize+1, losingColor);
      }

      //send it to the strip
      strip->show();
    }
    bool indicatorsWithinBounds()
    {

      bool status = false;
      for(int i = 0; i < 6; i++)
      {
        //if an indicator is inside the box, change the status to true
        if( (deviceIndicatorPosition[i] >= boxStart) && (deviceIndicatorPosition[i] <= (boxStart + boxSize+1)))
        {
          status |= 1; 
        }
        //don't change the status if it's an uninitialized indicator
        else if(deviceIndicatorPosition[i] + 2 < 0.1) status |= 0; 
        else
        {
          status &= 0; //if it is initialized and its outside the box, change status to false. 
          //set the indicator color to the losing player's color. The first one to go outside the box loses.
          //the indicator box is reset on a 5 second timer with a flag once the gamestatus is set to lost
          if(i == 0) losingColor = DCON1COLOR;
          if(i == 1) losingColor = ACON1COLOR;
          if(i == 2) losingColor = DACON1COLOR;
          if(i == 3) losingColor = DCON2COLOR;
          if(i == 4) losingColor = ACON2COLOR;
          if(i == 5) losingColor = DACON2COLOR;
        }
      }
      return(status);
      // Serial.print((int)status);
      // Serial.print("\n");
    }

    _indicatorstrip()
    {
      strip = new Adafruit_NeoPixel(LED_COUNTa, LED_PIN, NEO_GRB + NEO_KHZ800);
      strip->begin();
      strip->clear();
      strip->show();
    }

    ~_indicatorstrip()
    {
      //must define a destructor, but I have no use for it here. `
    }

    //updates the position of the assotiated indicator in deviceIndicatorPosition[]
    //this function is called by each active affector. 
    void updatePosition(float rate, int portnum)
    {
      // Serial.print(deviceIndicatorPosition[portnum]);
      // Serial.print("\n");
      deviceIndicatorPosition[portnum] += (rate * LEDMAXINCREMENT);
      if(deviceIndicatorPosition[portnum] < -1) deviceIndicatorPosition[portnum] = -1;
      if(deviceIndicatorPosition[portnum] > 60) deviceIndicatorPosition[portnum] = 60;
      // return;
    }
    //this function is responsble for setting up where and how large the bounding box is. 
    //this can be changed on the fly. 
    void setBoundingBox(int start, int size) 
    {
      boxStart = start;
      boxSize = start+size+1;
      return;
    }
    
  private:
    unsigned long losingColor;
    int boxSize = 5; //the size of the box centered around boxposition (must be at least 3)
    int boxStart = 10;  //the position of the bounding box as an LED number the box is centered around. 
};

#endif