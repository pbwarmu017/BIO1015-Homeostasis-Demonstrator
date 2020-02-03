
#ifndef ENCODER_CPP
#define ENCODER_CPP 

#include "superclasses.cpp"
#include "indicatorstrip.cpp"
#include "menu.cpp"
// #include "lcd.cpp"
// #include "indicatorstrip.cpp"
//encoder defines 
// #define CRANKRATECALCDELAY 50 //value is in milliseconds
#define CRANKRATESCALAR 80.0


class _encoder: public _affector
{
  public:

    // _lcd* lcd_ptr;
    //check https://playground.arduino.cc/Main/RotaryEncoders/#OnInterrupts
    
    //polls the encoder pins and evaluates if the encoder has moved forward, backward, or stayed the same. 
    //returns 0 if no change, 1 if a CW movement was detected, -1 if a CCW movement was detected. 
    //using this in conjuction with a low pass filter helps clean up switch bounce
    //http://makeatronics.blogspot.com/2013/02/efficiently-reading-quadrature-with.html to see how it works
    char returnDelta(void)
    {
      pinAVal = digitalRead(encoderPinA);
      pinBVal = digitalRead(encoderPinB);
      unsigned char lookupVal = (prevAVal << 3) | (prevBVal << 2) | (pinAVal << 1) | pinBVal;
      prevAVal = pinAVal;
      prevBVal = prevBVal;
      return quadratureLookupTable[lookupVal];
    }

    //this is called (through a flag checked for in main)
    //calculates a production rate and sends it to the strip. 
    void calculateRate(int modifier)
    {
      if(modifier == CRANKSUM_RATETYPE)
      {
        int currentOut = returnDelta();
        //make sure it's not an invalid state change
        if(currentOut)
        { 
        crankSum += currentOut;
        }
      }
      if(modifier == GENERAL_RATETYPE)
      {
        // calculates current moving average efficiently
        movingAverage += (-movingAverage/movingAveragePeriod + crankSum*1.1);

        //make it pointless to spin the crank too fast
        if(movingAverage > 80) movingAverage = 80;
        //prevent excessively small carryover
        if(movingAverage < 1) movingAverage = 0;
        overallRate = ((movingAverage/80 * maxProductionRate)*2.0 - consumptionRate) * 0.0001 * CRANKRATESCALAR;// * CRANKRATESCALAR;
        //reset the sum because it has just been incorporated into a moving avg
        crankSum = 0;
        //send the rate to the strip so that it can update the position of this indicator
        indicatorstrip_ptr->updatePosition(overallRate, portNum);
        // Serial.print(overallRate);
        // Serial.print("\n");
      }
      return;
    }

    //upon object creation, set up the pins ands store the current values for the encoder
    //this alsoo stores pointerse to the lcd and incicatorstrup objects
    _encoder(const int port, _device *mainptr, _indicatorstrip *indptr, _lcd *lcdptr, _menu *menuptr)//_indicatorstrip* indicatorstrip)
    {
      //these pointers are stored in the object so that they can be used outside of the constructor
      main_ptr = mainptr; //pointer to the main _device object (used to store some globally needed variables)
      indicatorstrip_ptr = indptr; //pointer for the indicatorstrip object
      lcd_ptr = lcdptr;
      menu_ptr = menuptr;

      portNum = port;

      switch(portNum)
      {
        case DCON1_PORTNUM:
          encoderPinA = DCON1_PIND1;
          encoderPinB = DCON1_PIND2;
          main_ptr->DCON1_mode = HANDCRANK_TYPE;
          break;
        case DCON2_PORTNUM:
          encoderPinA = DCON2_PIND1;
          encoderPinB = DCON2_PIND2;
          main_ptr->DCON2_mode = HANDCRANK_TYPE;

          break;      
      }

      prevAVal = digitalRead(encoderPinA);
      prevBVal = digitalRead(encoderPinB);

      // clear any outstanding pin change interrupt flags
      PCIFR  |= bit (digitalPinToPCICRbit(encoderPinA)); 
      PCIFR  |= bit (digitalPinToPCICRbit(encoderPinB));

      // enable interrupt for the GROUP (digital pins 1-7, digtial pins 8-13)
      PCICR  |= bit (digitalPinToPCICRbit(encoderPinA)); 
      PCICR  |= bit (digitalPinToPCICRbit(encoderPinB));

      // enable pin change interrupt for encoder pins
      *digitalPinToPCMSK(encoderPinA) |= bit(digitalPinToPCMSKbit(encoderPinA)); 
      *digitalPinToPCMSK(encoderPinB) |= bit(digitalPinToPCMSKbit(encoderPinB)); 
      menu_ptr->printMenu(lcd_ptr);

    }
    ~_encoder()
    {//destructor (virtualized in _device)
      // dispable pin change interrupt for encoder pins
      *digitalPinToPCMSK(encoderPinA) ^= bit(digitalPinToPCMSKbit(encoderPinA)); 
      *digitalPinToPCMSK(encoderPinB) ^= bit(digitalPinToPCMSKbit(encoderPinB));

      switch(portNum)
      {
        case DCON1_PORTNUM:
          encoderPinA = -1;
          encoderPinB = -1;
          main_ptr->DCON1_mode = 0;
          indicatorstrip_ptr->deviceIndicatorPosition[DCON1_PORTNUM] = -2;
          break;
        case DCON2_PORTNUM:
          encoderPinA = -1;
          encoderPinB = -1;
          main_ptr->DCON2_mode = 0;
          indicatorstrip_ptr->deviceIndicatorPosition[DCON2_PORTNUM] = -2;
          break;      
      }
      menu_ptr->printMenu(lcd_ptr);
    }
  private:
    _device *main_ptr;
    _indicatorstrip *indicatorstrip_ptr;
    _lcd *lcd_ptr;
    _menu *menu_ptr;

    char quadratureLookupTable[16] = {0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0};
    //removed extraneous values to help prevent bouncing, and inverted the polarity
    // http://makeatronics.blogspot.com/2013/02/efficiently-reading-quadrature-with.html
    // char quadratureLookupTable[16] = {0,0,0,0,0,0,0,-1,0,0,0,0,0,1,0,0};
    float overallRate = 0;
    float movingAverage = 0; //holds the moving average for the production of the hand crank. 
    float movingAveragePeriod = 1000/STRIPREFRESHDELAY; 
    float maxProductionRate = 60; //used in the rate calculation
    float consumptionRate = 70; ///used in the rate calculation
    int portNum = -1; //used to save the port number that this object is instantiated on.
    int crankSum = 0; //sums the number of valid pulses from the encoder
    int encoderPinA = -1; //stores object pin configuration
    int encoderPinB = -1; //stores object pin configuration
    bool prevAVal;
    bool prevBVal;
    bool pinAVal;
    bool pinBVal;
};

#endif