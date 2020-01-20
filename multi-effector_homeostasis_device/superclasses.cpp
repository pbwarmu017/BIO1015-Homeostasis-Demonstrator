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
    int DCON1_status = 0;
    int ACON1_status = 0;
    int DACON1_status = 0;
    int DCON2_status = 0;
    int ACON2_status = 0;
    int DACON2_status = 0;
    // virtual ~_device(); //virtualized destructor
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