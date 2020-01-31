
#ifndef ENCODER_CPP
#define ENCODER_CPP 

#include "superclasses.cpp"
// #include "lcd.cpp"
// #include "indicatorstrip.cpp"
//encoder defines 
#define CRANKRATECALCDELAY 50 //value is in milliseconds
#define CRANKRATEMAX 24
#define CRANKRATESCALER 67


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
        if(movingAverage > 60) movingAverage = 60;
        //prevent excessively small carryover
        if(movingAverage < 0.5) movingAverage = 0;
        overallRate = (movingAverage/30 * maxProductionRate) - consumptionRate;
        //reset the sum because it has just been incorporated into a moving avg
        crankSum = 0;
        Serial.print(overallRate);
        Serial.print("\n");
      }
      return;
    }
    //return the current rate (a number between 100 and -100) on demand
    // int calculateProductionRate(int crankSum)
    int returnRate(void){
      return(overallRate);
    }
    //upon object creation, set up the pins ands store the current values for the encoder
    //this alsoo stores pointerse to the lcd and incicatorstrup objects
    _encoder(const int port, _device *ptr)//_indicatorstrip* indicatorstrip)
    {
      main_ptr = ptr;
      portNum = port;

      switch(portNum)
      {
        case DCON1_PORTNUM:
          encoderPinA = 3;
          encoderPinB = 5;
          main_ptr->DCON1_mode = HANDCRANK_TYPE;
          break;
        case DCON2_PORTNUM:
          encoderPinA = 9;
          encoderPinB = 10;
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
          break;
        case DCON2_PORTNUM:
          encoderPinA = -1;
          encoderPinB = -1;
          main_ptr->DCON2_mode = 0;
          break;      
      }

    }
  private:
    _device *main_ptr;
    // char quadratureLookupTable[16] = {0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0};
    //removed extraneous values to help prevent bouncing, and inverted the polarity
    // http://makeatronics.blogspot.com/2013/02/efficiently-reading-quadrature-with.html
    char quadratureLookupTable[16] = {0,0,0,0,0,0,0,-1,0,0,0,0,0,1,0,0};
    float overallRate = 0;
    float movingAverage = 0; //holds the moving average for the production of the hand crank. 
    float movingAveragePeriod = 1000/CRANKRATECALCDELAY; 
    int maxProductionRate = 100; //used in the rate calculation
    int consumptionRate = 80; ///used in the rate calculation
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