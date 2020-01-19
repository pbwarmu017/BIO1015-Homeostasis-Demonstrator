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
//this is used when the port is hardcoded in the library (lcd)
#define HARDCODED_PORTNUM 8 
#define INDICATORSTRIP_TYPE 1
#define LCD_TYPE 2
#define MENU_TYPE 3
#define HANDGRIP_TYPE 4
#define ENCODER_TYPE 5



#define STRIPTESTMODE 0 //used for internal testing

#define GAMERESETDELAY 5000 //in milliseconds

#define LCDREFRESHDELAY 205 //in milliseconds

#define SELECTTIMERROLLOVER 5

class _device {
  public:
    // !_device(){
    //   return(0);
    // }
    // return(0);`
};

class _affector: public _device {
  public: 
    float returnPosition(float currentPosition){
      return(currentPosition);
    }
};
#endif