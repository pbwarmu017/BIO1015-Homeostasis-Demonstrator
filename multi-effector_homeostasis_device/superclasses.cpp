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
#define HANDGRIPACTIVE 1
#define CRANKDEVNUM 1
#define CRANKACTIVE 1
//port number defines for readability 
#define DCON1_PORTNUM 1
#define ACON1_PORTNUM 2
#define DACON1_PORTNUM 3
#define DCON2_PORTNUM 4
#define ACON2_PORTNUM 5
#define DACON2_PORTNUM 6
#define INDICATORSTRIP_PORTNUM 7
#define HARDCODED_PORTNUM 8 //this is used when the port is hardcoded in the library (lcd)

#define indicatorStrip_type 1
#define handGrip_type 2
#define handCrank_type 3
#define lcd_type 4
#define menu_type 5



#define STRIPTESTMODE 0 //used for internal testing

#define GAMERESETDELAY 5000 //in milliseconds

#define LCDREFRESHDELAY 205 //in milliseconds

#define SELECTTIMERROLLOVER 5

class _device {
  public:
    // return(0);`
};

class _affector: public _device {
  public:
    float returnPosition(float currentPosition){
      return(currentPosition);
    }
};
#endif