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

// //port number defines
// #define DCON1 0
// #define DCON2 1
// #define ACON1 2
// #define ACON2 3
// #define DACON1 4
// #define DACON2 5

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
#endif