
#ifndef INDICATORSTRIP_CPP
#define INDICATORSTRIP_CPP

#include "superclasses.cpp"
#include "menu.cpp"
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
#define STRIPREFRESHDELAY 50 //in milliseconds
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
      boxEnd = boxStart + menu_ptr->boundingboxsize + 1;
      if(menu_ptr->boundingboxmode > 0)
      {
        if(boundingBoxDirection == 1)
        {
          if(boxEnd < 59)
          {
            boxStart += 0.2 * menu_ptr->boundingboxmode;
            boxEnd = boxStart + menu_ptr->boundingboxsize + 1;
          }
          else
          {
            boundingBoxDirection = -1; //change direction
          }
        }
        if(boundingBoxDirection == -1)
        {
          if(boxStart > 1)
          {
            boxStart -= 0.2 * menu_ptr->boundingboxmode;
            boxEnd = boxStart + menu_ptr->boundingboxsize + 1;
          }
          else
          {
            boundingBoxDirection = 1; //change direction
          }
        }
      }
      if(boxEnd > 60) //here to catch events where the user tries to make the box grow beyond the strip
      {
        boxEnd--;
        boxStart--;
      }
      if(gameStatus == started && indicatorsWithinBounds())
      {
        strip->setPixelColor(boxStart, COLORGREEN);
        strip->setPixelColor(boxEnd, COLORGREEN);
      }
      if(gameStatus == started && !indicatorsWithinBounds())
      {
        gameStatus = lost;
        strip->setPixelColor(boxStart, losingColor);
        strip->setPixelColor(boxEnd, losingColor);
      }

      if(gameStatus == notstarted && indicatorsWithinBounds())
      {
        gameStatus = started;
        strip->setPixelColor(boxStart, COLORGREEN);
        strip->setPixelColor(boxEnd, COLORGREEN);
      }
      if(gameStatus == notstarted && !indicatorsWithinBounds())
      {
        strip->setPixelColor(boxStart, COLORRED);
        strip->setPixelColor(boxEnd, COLORRED);
      }

      if(gameStatus == lost)
      {
        strip->setPixelColor(boxStart, losingColor);
        strip->setPixelColor(boxEnd, losingColor);
      }

      //send it to the strip
      strip->show();
    }
    //checks to see if all active indicators are in the bounding box.
    //returns true if all active indicators are within bounds
    //returns false if an active indicator is out of bounds oor all indicators are inactive.
    bool indicatorsWithinBounds()
    {

      int status = 3; //starting state of the state machine

 
      for(int i = 0; i < 6; i++) //check each indicator's position
      {
        //is it an active position? If active, will be at minimum -1. (floating point, so don't use equality)
        if(deviceIndicatorPosition[i] + 2 > 0.5) 
        {
          //is it in bounds?
          if( (deviceIndicatorPosition[i] >= boxStart) && (deviceIndicatorPosition[i] <= (boxStart + menu_ptr->boundingboxsize + 1)))
          {
            /*there are two possiblilities if it arrives here. Either the previous status indication is inactive (3)
            or active (1), either way we want to set status to 1*/
            status = 1; 
            
          }
          else //nope, its out of bounds
          {
            //set the proper losing color
            if(i == 0) losingColor = DCON1COLOR;
            if(i == 1) losingColor = ACON1COLOR;
            if(i == 2) losingColor = DACON1COLOR;
            if(i == 3) losingColor = DCON2COLOR;
            if(i == 4) losingColor = ACON2COLOR;
            if(i == 5) losingColor = DACON2COLOR; 
            return false; 
          }
        }
        // Serial.print(deviceIndicatorPosition[i]);
        // Serial.print("\n");
        //if an indicator is inside the box, change the status to true
      }
      if(status == 1) return true; //if all active indicators are in bounds
      if(status == 3) return false; //will only happen if all indicators are inactive
      // Serial.print("-\n");
      // return(status);
    }

    _indicatorstrip(_menu *ptr)
    {
      menu_ptr = ptr;
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
      deviceIndicatorPosition[portnum] += (rate * LEDMAXINCREMENT);
      if(deviceIndicatorPosition[portnum] < -1) deviceIndicatorPosition[portnum] = -1;
      if(deviceIndicatorPosition[portnum] > 60) deviceIndicatorPosition[portnum] = 60;
      // return;
    }
    //this function is responsble for setting up where and how large the bounding box is. 
    //this can be changed on the fly. 

    // void setBoundingBox(int start, int size) 
    // {
    //   boxStart = start;
    //   boxSize = start+size+1;
    //   boxEnd = start+size+1;
    //   return;
    // }
    
  private:
    _menu *menu_ptr;
    char boundingBoxDirection = 1;
    unsigned long losingColor; //I think this has to be a long. 
    float boxSize ; //the size of the box centered around boxposition (must be at least 3)
    float boxStart = 30;  //the position of the bounding box as an LED number the box is centered around. 
    float boxEnd;// = boxStart + boxSize + 1.;
};

#endif