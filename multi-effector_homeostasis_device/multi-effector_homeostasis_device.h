#include "includes.h"
#ifndef _MULTIHOMEO
#define _MULTIHOMEO

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

//port number defines for readability 
#define DCON1_PORTNUM 1
#define ACON1_PORTNUM 2
#define DACON1_PORTNUM 3
#define DCON2_PORTNUM 4
#define ACON2_PORTNUM 5
#define DACON2_PORTNUM 6
#define HARDCODED_PORTNUM 7 //this is used when the port is hardcoded in the library (lcd)

#define indicatorStrip_type 1
#define handGrip_type 2
#define handCrank_type 3
#define lcd_type 4
#define menu_type 5

extern enum GAMESTATUS gameStatus;
extern enum SYSTEMMODE systemMode;

#define HANDGRIPPIN A0
#define ENCODERPINA 3
#define ENCODERPINB 5

#define HANDGRIPDEVNUM 0 //device number for handgrip
#define CRANKDEVNUM 1

#define HANDGRIPACTIVE 1
#define CRANKACTIVE 1

#define STRIPTESTMODE 0 //used for internal testing

#define GAMERESETDELAY 5000 //in milliseconds

#define LCDREFRESHDELAY 205 //in milliseconds

#define SELECTTIMERROLLOVER 5

#define RED 0x1
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7

// unsigned int createObject(byte objtype, byte portnum);
#endif