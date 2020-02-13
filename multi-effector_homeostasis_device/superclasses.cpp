#ifndef SUPERCLASSES_CPP
#define SUPERCLASSES_CPP

#include <Arduino.h>
#include <avr/pgmspace.h> //needed for PROGMEM access. 
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

#define VERSIONNUMBER F("2.1.0")

#define HANDGRIPDEVNUM 0 //device number for handgrip
#define CRANKDEVNUM 1

//port number defines for readability 
#define DCON1_PORTNUM 0
#define ACON1_PORTNUM 1
#define DACON1_PORTNUM 2
#define DCON2_PORTNUM 3
#define ACON2_PORTNUM 4
#define DACON2_PORTNUM 5
#define INDICATORSTRIP_PORTNUM 6
#define HARDCODED_PORTNUM 7 //used when the port is hardcoded in the library (lcd)

//pin defines
#define ACON1_PINA1 A0
#define ACON1_PINA2 A1
#define DCON1_PIND1 3
#define DCON1_PIND2 5
#define DACON1_PIND1 4
#define DACON1_PINA1 ACON1_PINA1 //DACON channels share analog channels the ACON ports

#define ACON2_PINA1 A2
#define ACON2_PINA2 A3
#define DCON2_PIND1 9
#define DCON2_PIND2 10
#define DACON2_PIND1 11
#define DACON2_PINA1 ACON2_PINA2

 //device types for readability
#define INDICATORSTRIP_TYPE 1
#define LCD_TYPE 2
#define MENU_TYPE 3
#define HANDGRIP_TYPE 4
#define HANDCRANK_TYPE 5

//rate calculation types
#define GENERAL_RATETYPE 0
#define CRANKSUM_RATETYPE 1



#define GAMERESETDELAY 5000 //in milliseconds

#define LCDREFRESHDELAY 205 //in milliseconds

class _device {
  public:
    uint8_t DCON1_mode = 0;
    uint8_t ACON1_mode = 0;
    uint8_t DACON1_mode = 0;
    uint8_t DCON2_mode = 0;
    uint8_t ACON2_mode = 0;
    uint8_t DACON2_mode = 0;
    
    _device(){};
    
    virtual ~_device(){} // must have a virtualized destructor

    //these virtual functions will be overwritten by derived class objects. 
    virtual void calculateRate(int8_t modifier){}

};

class _affector: public _device
{
  public: 
    virtual ~_affector(){} //must have a virtualized destructor
};
#endif