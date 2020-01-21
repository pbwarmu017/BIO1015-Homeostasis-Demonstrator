#ifndef SUPERCLASSES_CPP
#define SUPERCLASSES_CPP

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>

enum GAMESTATUS {
  notstarted,
  started,
  lost
};

enum SYSTEMMODE {
  calibrate,
  running,
  config
};

extern enum GAMESTATUS gameStatus;
extern enum SYSTEMMODE systemMode;

#define HANDGRIPDEVNUM 0 //device number for handgrip
#define CRANKDEVNUM 1

//port number defines for readability 
#define DCON1_PORTNUM 1
#define ACON1_PORTNUM 2
#define DACON1_PORTNUM 3
#define DCON2_PORTNUM 4
#define ACON2_PORTNUM 5
#define DACON2_PORTNUM 6
#define INDICATORSTRIP_PORTNUM 7

#define HARDCODED_PORTNUM 8 //used when the port is hardcoded in the library (lcd)

#define ACON1_PINNUM 0
#define ACON2_PINNUM 2
#define DACON1_PINNUM 1
#define DACON2_PINNUM 2
 //device types
#define INDICATORSTRIP_TYPE 1
#define LCD_TYPE 2
#define MENU_TYPE 3
#define HANDGRIP_TYPE 4
#define HANDCRANK_TYPE 5

//rate calculation types
#define GENERAL_RATETYPE 0
#define CRANKSUM_RATETYPE 1



#define STRIPTESTMODE 0 //used for internal testing

#define GAMERESETDELAY 5000 //in milliseconds

#define LCDREFRESHDELAY 205 //in milliseconds

#define SELECTTIMERROLLOVER 5

class _device {
  public:
    int DCON1_mode = 0;
    int ACON1_mode = 0;
    int DACON1_mode = 0;
    int DCON2_mode = 0;
    int ACON2_mode = 0;
    int DACON2_mode = 0;
    
    _device(){};
    // virtual _device(const int, _device*){}
    virtual ~_device(){}
    virtual void calculateRate(int modifier){}
    virtual int returnRate(void){}
};

class _affector: public _device
{
  public: 

    float returnPosition(float currentPosition)
    {
      return(currentPosition);
    }
    virtual ~_affector(){}
};
#endif